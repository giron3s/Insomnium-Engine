/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Render target for OpenGL. A render target allows to render objects to it
 *                instead of to the main screen. Then the target can be rendered to the main screen as
 *                a texture
 *
 *                The FB render target uses no effect-shader and performs no blending, it
 *                just does a copy-pixel operation
 ********************************************************************************/

#include "precompiled.h"
#include "graphic/opengl/opengl_rendertarget.h"

namespace Framework
{

    OpenGLRenderTarget::~OpenGLRenderTarget()
    {
        for ( const auto& attachment : mAttachments )
            switch ( attachment.second.ImplementationType )
            {
                case GL_RENDERBUFFER:
                __(glDeleteRenderbuffers(1, &attachment.second.Id));
                break;
                case GL_TEXTURE_2D:
                __(glDeleteTextures(1, &attachment.second.Id));
            }
        if ( mFramebuffer != GL_NONE )
            __(glDeleteFramebuffers(1, &mFramebuffer));
    }


    bool OpenGLRenderTarget::Init(uint32_t aWidth, uint32_t aHeight, const void* aParams)
    {
        if ( !(128 <= aWidth && aWidth <= 16384 &&
               128 <= aHeight && aHeight <= 16384 &&
               aParams) )
            return false; // Invalid parameter(s)
        if ( mSize.x )
            return false; // Already initialized

        uint32_t lMaxColorAttachments = 0;
        __(glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, (int*)&lMaxColorAttachments));
        // Validate lFBLayout
        auto aAttachments = reinterpret_cast<const RenderTarget::Attachments*>(aParams);
        mNumColorBuffers = mNumDepthBuffers = mNumStencilBuffers = 0;
        GLenum lNextExpectedColorAttachmentName = GL_COLOR_ATTACHMENT0;
        for ( const auto& attachment : *aAttachments )
        {
            auto f = attachment.second.DataFormat;
            if ( GL_COLOR_ATTACHMENT0 <= attachment.first && attachment.first <= GL_COLOR_ATTACHMENT0 + lMaxColorAttachments )
            {
                if ( attachment.first != lNextExpectedColorAttachmentName )
                    return false; // Invalid parameter - only consecutive color attachments are allowed
				if ( !(f == GL_R8   || f == GL_RG8   || f == GL_RGB8   || f == GL_RGBA8 ||
					   f == GL_R16F || f == GL_RG16F || f == GL_RGB16F || f == GL_RGBA16F ||
					   f == GL_R32F || f == GL_RG32F || f == GL_RGB32F || f == GL_RGBA32F) )
                    return false; // Invalid parameter
                ++mNumColorBuffers;
                ++lNextExpectedColorAttachmentName;
            }
            else if ( attachment.first == GL_DEPTH_ATTACHMENT )
            {
				if ( !(f == GL_DEPTH_COMPONENT16 || f == GL_DEPTH_COMPONENT24 || f == GL_DEPTH_COMPONENT32 ||
                       f == GL_DEPTH_COMPONENT32F) )
                    return false; // Invalid parameter
                ++mNumDepthBuffers;
            }
            else if ( attachment.first == GL_STENCIL_ATTACHMENT )
            {
				if ( !(f == GL_STENCIL_INDEX1 || f == GL_STENCIL_INDEX4 || f == GL_STENCIL_INDEX8 || f == GL_STENCIL_INDEX16) )
                    return false; // Invalid parameter
                ++mNumStencilBuffers;
            }
            else if ( attachment.first == GL_DEPTH_STENCIL_ATTACHMENT )
            {
				if ( !(f == GL_DEPTH24_STENCIL8 || f == GL_DEPTH32F_STENCIL8) )
                    return false; // Invalid parameter
                ++mNumDepthBuffers;
                ++mNumStencilBuffers;
            }
            else
                return false; // Invalid parameter

            if ( !(attachment.second.ImplementationType == GL_RENDERBUFFER ||
                   attachment.second.ImplementationType == GL_TEXTURE_2D) ||
                  attachment.second.Id /*||
                  !(0.0f <= attachment.second.ClearValue[0] && attachment.second.ClearValue[0] <= 1.0f &&
                    0.0f <= attachment.second.ClearValue[1] && attachment.second.ClearValue[1] <= 1.0f &&
                    0.0f <= attachment.second.ClearValue[2] && attachment.second.ClearValue[2] <= 1.0f &&
                    0.0f <= attachment.second.ClearValue[3] && attachment.second.ClearValue[3] <= 1.0f)*/ )
                return false; // Invalid parameter
        }

        if ( mNumDepthBuffers > 1 || mNumStencilBuffers > 1 )
            return false; // Invalid parameter

        // Create objects
        __(glGenFramebuffers(1, &mFramebuffer));
        __(glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer));

        mAttachments = *aAttachments;
        for ( auto& attachment : mAttachments )
            switch ( attachment.second.ImplementationType )
            {
                case GL_RENDERBUFFER:
                __(glGenRenderbuffers(1, &attachment.second.Id));
                break;
                case GL_TEXTURE_2D:
                __(glGenTextures(1, &attachment.second.Id));
            }

        // Setup texture and renderbuffer components and attach them to framebuffer
        if ( !SetSize(aWidth, aHeight) )
            return false;

        __(glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer));
        GLenum status;
        if ( (status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE )
            return false;
        __(glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE));

        return true;
    }


    bool OpenGLRenderTarget::SetSize(uint32_t aWidth, uint32_t aHeight)
    {
        if( !mFramebuffer )
            return false; // invalid call
        if( !(128 <= aWidth && aWidth <= 16384 &&
              128 <= aHeight && aHeight <= 16384) )
            return false; // invalid parameter(s)

        __(glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer));
        for ( const auto& attachment : mAttachments )
        {
            GLenum gl_format = 0, gl_data_type = 0;
            if ( GL_COLOR_ATTACHMENT0 <= attachment.first && attachment.first <= GL_COLOR_ATTACHMENT0 + mNumColorBuffers )
            {
                switch ( attachment.second.DataFormat )
                {
                    case GL_R8:  	 gl_format = GL_RED;  gl_data_type = GL_UNSIGNED_BYTE; break;
                    case GL_R16F:    gl_format = GL_RED;  gl_data_type = GL_HALF_FLOAT; break;
                    case GL_R32F:    gl_format = GL_RED;  gl_data_type = GL_FLOAT; break;
                    case GL_RG8:     gl_format = GL_RG;   gl_data_type = GL_UNSIGNED_BYTE; break;
                    case GL_RG16F:   gl_format = GL_RG;   gl_data_type = GL_HALF_FLOAT; break;
                    case GL_RG32F:   gl_format = GL_RG;   gl_data_type = GL_FLOAT; break;
                    case GL_RGB8:    gl_format = GL_RGB;  gl_data_type = GL_UNSIGNED_BYTE; break;
                    case GL_RGB16F:  gl_format = GL_RGB;  gl_data_type = GL_HALF_FLOAT; break;
                    case GL_RGB32F:  gl_format = GL_RGB;  gl_data_type = GL_FLOAT; break;
                    case GL_RGBA8:   gl_format = GL_RGBA; gl_data_type = GL_UNSIGNED_BYTE; break;
                    case GL_RGBA16F: gl_format = GL_RGBA; gl_data_type = GL_HALF_FLOAT; break;
                    case GL_RGBA32F: gl_format = GL_RGBA; gl_data_type = GL_FLOAT;
                }
            }
            else if ( attachment.first == GL_DEPTH_ATTACHMENT )
            {
                gl_format = GL_DEPTH_COMPONENT;
                switch ( attachment.second.DataFormat )
                {
                    case GL_DEPTH_COMPONENT16:  gl_data_type = GL_UNSIGNED_SHORT; break;
                    case GL_DEPTH_COMPONENT24:
                    case GL_DEPTH_COMPONENT32:  gl_data_type = GL_UNSIGNED_INT; break;
                    case GL_DEPTH_COMPONENT32F: gl_data_type = GL_FLOAT;
                }
            }
            else if ( attachment.first == GL_STENCIL_ATTACHMENT )
            {
                gl_format = GL_STENCIL;
                switch ( attachment.second.DataFormat )
                {
                    case GL_STENCIL_INDEX1:
                    case GL_STENCIL_INDEX4:
                    case GL_STENCIL_INDEX8:  gl_data_type = GL_UNSIGNED_BYTE; break;
                    case GL_STENCIL_INDEX16: gl_data_type = GL_UNSIGNED_SHORT;
                }
            }
            else if ( attachment.first == GL_DEPTH_STENCIL_ATTACHMENT )
            {
                gl_format = GL_DEPTH_STENCIL;
                switch ( attachment.second.DataFormat )
                {
                    case GL_DEPTH24_STENCIL8:  gl_data_type = GL_UNSIGNED_INT_24_8; break;
                    case GL_DEPTH32F_STENCIL8: gl_data_type = GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
                }
            }

            if ( !(gl_format && gl_data_type) )
                return false;

            switch ( attachment.second.ImplementationType )
            {
                case GL_RENDERBUFFER:
                __(glBindRenderbuffer(GL_RENDERBUFFER, attachment.second.Id));
                ASSERT(glIsRenderbuffer(attachment.second.Id));
                __(glRenderbufferStorage(GL_RENDERBUFFER, attachment.second.DataFormat, aWidth, aHeight));
                __(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment.first, GL_RENDERBUFFER, attachment.second.Id));
                __(glBindRenderbuffer(GL_RENDERBUFFER, GL_NONE));
                break;

                case GL_TEXTURE_2D:
                __(glBindTexture(GL_TEXTURE_2D, attachment.second.Id));
                ASSERT(glIsTexture(attachment.second.Id));
                if ( attachment.second.DataFormat == GL_DEPTH_COMPONENT32F ||
                     attachment.second.DataFormat == GL_DEPTH32F_STENCIL8 )
                { // settings for hardware-accelerated antialiased shadows
                    __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                    __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
                    __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
                    __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE));
                }
                else
                {
                    __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
                    __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
                }
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
                __(glTexImage2D(GL_TEXTURE_2D, 0, attachment.second.DataFormat, aWidth, aHeight, 0, gl_format, gl_data_type, NULL));
                __(glFramebufferTexture(GL_FRAMEBUFFER, attachment.first, attachment.second.Id, 0));
                __(glBindTexture(GL_TEXTURE_2D, GL_NONE));
                break;

                default: return false;
            }

        }
        __(glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE));
        mSize.x = aWidth;
        mSize.y = aHeight;
        return true;
    }


    void OpenGLRenderTarget::BindForDrawning() const
    {
        __(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFramebuffer));
        if ( mNumColorBuffers )
        {
            GLenum* lColorBufferNames = (GLenum*)alloca(mNumColorBuffers * sizeof(GLenum));
            for ( uint32_t i = 0; i < mNumColorBuffers; ++i )
                lColorBufferNames[i] = GL_COLOR_ATTACHMENT0 + i;
            __(glDrawBuffers(mNumColorBuffers, lColorBufferNames));
        }
        else
        { // no color buffer attachments - set that No color buffers shall be written
            __(glDrawBuffer(GL_NONE));
        }
        __(glViewport(0, 0, mSize.x, mSize.y));
    }


    void OpenGLRenderTarget::BindForReading() const
    {
        __(glBindFramebuffer(GL_READ_FRAMEBUFFER, mFramebuffer));
        //glReadBuffer(GL_COLOR_ATTACHMENTi);
    }


    void OpenGLRenderTarget::Unbind() const
    {
        __(glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE));
    }


    void OpenGLRenderTarget::Blit(uint32_t dstX, uint32_t dstY, uint32_t aWidth, uint32_t aHeight, uint32_t srcColorBufferIndex, bool dstIsMainFB) const
    {
        if ( (mNumColorBuffers && srcColorBufferIndex >= mNumColorBuffers) ||
             (!mNumColorBuffers && srcColorBufferIndex) )
        {
            ASSERT(0);
            return; // Invalid parameter
        }

        __(glBindFramebuffer(GL_READ_FRAMEBUFFER, mFramebuffer));
        GLbitfield lMask = 0;
        if ( mNumColorBuffers )
        { // select a source color buffer
            __(glReadBuffer(GL_COLOR_ATTACHMENT0 + srcColorBufferIndex));
            lMask |= GL_COLOR_BUFFER_BIT;
        }
        else
        { // set no source color buffer
            __(glReadBuffer(GL_NONE));
        }

        /* // Most probably we don't need those 2
        if ( mNumDepthBuffers )
            lMask |= GL_DEPTH_BUFFER_BIT;

        if ( mNumStencilBuffers )
            lMask |= GL_STENCIL_BUFFER_BIT;*/

        if ( dstIsMainFB )
        {
            __(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE));
        }
        // else we assume some other framebuffer is bound for writing
        // with glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dstFramebuffer)
        // and a destination color buffer is selected with
        // glDrawBuffer(GL_COLOR_ATTACHMENT0 + dstColorBufferIndex)

#define RENDERTARGET_SINGLE_BLIT
#ifdef RENDERTARGET_SINGLE_BLIT
        __(glBlitFramebuffer(0, 0, mSize.x, mSize.y, dstX, dstY, dstX + aWidth, dstY + aHeight, lMask, GL_NEAREST));
#else
        // If linear interpolation is needed we need to blit the color buffer first with
        // linear interpolation and then the depth buffer (and stencil if present)
        // with nearest
        if ( mNumColorBuffers )
            glBlitFramebuffer(0, 0, mSize.x, mSize.y, dstX, dstY, dstX + aWidth, dstY + aHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
        if ( mNumDepthBuffers )
            glBlitFramebuffer(0, 0, mSize.x, mSize.y, dstX, dstY, dstX + aWidth, dstY + aHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
#endif

        glBindFramebuffer(GL_READ_FRAMEBUFFER, GL_NONE);
    }


    void OpenGLRenderTarget::Clear(uint32_t aMask) const
    {
        if ( !aMask )
        { // default:
            if ( mNumColorBuffers )
                aMask |= GL_COLOR_BUFFER_BIT;
            if ( mNumDepthBuffers )
                aMask |= GL_DEPTH_BUFFER_BIT;
            if ( mNumStencilBuffers )
                aMask |= GL_STENCIL_BUFFER_BIT;
        }
        
        if ( mNumColorBuffers && (aMask & GL_COLOR_BUFFER_BIT) )
        {
            auto lAttachment = mAttachments.find(GL_COLOR_ATTACHMENT0);
            for ( uint16_t i = 0; i < mNumColorBuffers; ++i )
            {
                __(glClearBufferfv(GL_COLOR, i, lAttachment->second.ClearValue));
                ++lAttachment;
            }
        }

        if ( mNumDepthBuffers && (aMask & GL_DEPTH_BUFFER_BIT) )
        {
            auto lAttachment = mAttachments.find(GL_DEPTH_ATTACHMENT);
            if ( lAttachment == mAttachments.end() )
                lAttachment = mAttachments.find(GL_DEPTH_STENCIL_ATTACHMENT);
            __(glClearBufferfv(GL_DEPTH, 0, lAttachment->second.ClearValue));
        }

        if ( mNumStencilBuffers && (aMask & GL_STENCIL_BUFFER_BIT) )
        {
            auto lAttachment = mAttachments.find(GL_DEPTH_STENCIL_ATTACHMENT);
            int i = 1; // index to retrieve clear value if buffer is depth-stencil
            if ( lAttachment == mAttachments.end() )
            {
                lAttachment = mAttachments.find(GL_STENCIL_ATTACHMENT);
                i = 0; // index to retrieve clear value if buffer is stencil
            }
            int lRange = 0;
            switch (lAttachment->second.DataFormat)
            {
                case GL_STENCIL_INDEX1 : lRange = 1; break;
                case GL_STENCIL_INDEX4 : lRange = 0xf; break;
                case GL_STENCIL_INDEX16 : lRange = 0xffff; break;
                case GL_STENCIL_INDEX8 :
                default:/*assume 8 bit stencil*/ lRange = 0xff; break;
            }
            i = (int)(lAttachment->second.ClearValue[i] * lRange); // [0.0f, 1.0f] -> [0, n]
            __(glClearBufferiv(GL_STENCIL, 0, &i));
        }
    }


    uint32_t OpenGLRenderTarget::GetTextureId(uint32_t aAttachment) const
    {
        if ( !aAttachment )
        { // default
            if ( mNumColorBuffers )
                aAttachment = GL_COLOR_ATTACHMENT0;
            else if ( mNumDepthBuffers )
            {
                if ( mNumStencilBuffers )
                    aAttachment = GL_DEPTH_STENCIL_ATTACHMENT;
                else
                    aAttachment = GL_DEPTH_ATTACHMENT;
            }
        }
        uint32_t id = GL_NONE;
        const auto lAttachment = mAttachments.find(aAttachment);
        if ( lAttachment != mAttachments.end() &&
             lAttachment->second.ImplementationType == GL_TEXTURE_2D )
            id = lAttachment->second.Id;
        ASSERT(glIsTexture(id));
        return id;
    }


    uint32_t OpenGLRenderTarget::GetColor(uint32_t x, uint32_t y, uint32_t colorBufferIndex) const
    {
        if ( colorBufferIndex >= mNumColorBuffers )
        {
            ASSERT(0); // Invalid parameter
            return 0;
        }
        uint32_t lPixel = 0xFF000000;
        __(glBindFramebuffer(GL_READ_FRAMEBUFFER, mFramebuffer));
        __(glReadBuffer(GL_COLOR_ATTACHMENT0 + colorBufferIndex));
        // from OpenGL reference: format specifies the format for the returned pixel values
        __(glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &lPixel));
        __(glBindFramebuffer(GL_READ_FRAMEBUFFER, GL_NONE));
        return lPixel;
    }
}
