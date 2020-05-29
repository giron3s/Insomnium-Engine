/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *   Brief      : Render aTarget for OpenGL. A render aTarget allows to render objects to it
 *                instead of to the main screen. Then the aTarget can be rendered to the main screen as
 *                a texture
 *
 *                The SSAA render aTarget implements super sampled anti-aliasing
 ********************************************************************************/

#include "precompiled.h"
#include "graphic/opengl/opengl.h"
#include "graphic/opengl/opengl_ssaarendertarget.h"

#include "graphic/renderer.h"

namespace Framework
{
    OpenGLSSAARenderTarget::~OpenGLSSAARenderTarget()
    {
        __(glDeleteBuffers(1, &mVertexBuffer));
        __(glDeleteVertexArrays(1, &mVertexArray));

        for (unsigned int i = 0; i < mNumTargets; ++i) {
            __(glDeleteTextures(1, &mColorBuffer[i]));
        }
        delete[] mColorBuffer;
        mColorBuffer = NULL;

        delete[] mAttachments;
        mAttachments = NULL;

        __(glDeleteRenderbuffers(1, &mDepthBuffer));
        __(glDeleteRenderbuffers(1, &mDepthBuffer));
        __(glDeleteFramebuffers(1, &mFrameBuffer));
    }

    bool OpenGLSSAARenderTarget::Init(uint32_t aWidth, uint32_t aHeight, uint32_t aFactor, uint32_t aNumTargets)
    {
        if (aNumTargets <= 0 || aNumTargets > GL_MAX_COLOR_ATTACHMENTS)
        {
            CRASH("Number of aTargets for render aTarget (%d) not supported. Max. is %d\n", aNumTargets, GL_MAX_COLOR_ATTACHMENTS);
        }

        mNumTargets = aNumTargets;

        aWidth *= aFactor;
        aHeight *= aFactor;

        /* Allocate the color buffers IDs */
        mColorBuffer = new GLuint[mNumTargets];

        /* Texture buffer */
        __(glGenTextures(mNumTargets, mColorBuffer));
        for (unsigned int i = 0; i < mNumTargets; ++i) {
            __(glBindTexture(GL_TEXTURE_2D, mColorBuffer[i]));
            {
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
                __(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aWidth, aHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
            }
        }
        __(glBindTexture(GL_TEXTURE_2D, 0));

        /* Depth buffer */
        __(glGenRenderbuffers(1, &mDepthBuffer));
        __(glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer));
        {
            __(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, aWidth, aHeight));
        }
        __(glBindRenderbuffer(GL_RENDERBUFFER, 0));

        /* Framebuffer to link everything together */
        mAttachments = new GLuint[mNumTargets];

        __(glGenFramebuffers(1, &mFrameBuffer));
        __(glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer));
        {
            /* Attach all color buffers */
            for (unsigned int i = 0; i < mNumTargets; ++i) {
                mAttachments[i] = GL_COLOR_ATTACHMENT0 + i;
                __(glFramebufferTexture2D(GL_FRAMEBUFFER, mAttachments[i], GL_TEXTURE_2D, mColorBuffer[i], 0));
            }
            __(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer));

            GLenum status;
            if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
            {
                CRASH("ERROR OpenGLRenderTarget::init %d\n", status);
                return false;
            }
        }
        __(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        /* Generate the render aTarget surface */
        GLfloat verticesData[8] = {
            -1, -1, 1, -1, -1, 1, 1, 1,
        };

        __(glGenVertexArrays(1, &mVertexArray));
        __(glBindVertexArray(mVertexArray));
        {
            __(glGenBuffers(1, &mVertexBuffer));
            __(glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer));
            {
                __(glBufferData(GL_ARRAY_BUFFER, sizeof verticesData, verticesData, GL_STATIC_DRAW));

                __(glEnableVertexAttribArray(0));
                __(glVertexAttribPointer(0,         // attribute
                                         2,         // number of elements per vertex, here (x,y)
                                         GL_FLOAT,  // the type of each element
                                         GL_FALSE,  // take our values as-is
                                         0,         // no extra data between each position
                                         0          // offset of first element
                                         ));
            }
            __(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }
        __(glBindVertexArray(0));

        mWidth = aWidth;
        mHeight = aHeight;
        mFactor = aFactor;

        return true;
    }

    void OpenGLSSAARenderTarget::SetSize(uint32_t aWidth, uint32_t aHeight)
    {
       CRASH("not implemented");
    }

    void OpenGLSSAARenderTarget::Bind()
    {
        __(glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer));
        __(glDrawBuffers(mNumTargets, mAttachments));
        __(glViewport(0, 0, mWidth, mHeight));
    }

    void OpenGLSSAARenderTarget::BindDepth()
    {
        __(glBindTexture(GL_TEXTURE_2D, mDepthBuffer));
    }

    void OpenGLSSAARenderTarget::Unbind()
    {
        __(glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE));
    }

    bool OpenGLSSAARenderTarget::Blit(uint32_t aDstX, uint32_t aDstY, uint32_t aWidth, uint32_t aHeight, uint32_t aTarget, bool aBindMainFB)
    {
        if (aTarget >= mNumTargets)
        {
            CRASH("ERROR wrong aTarget number %d in OpenGLFilterRenderTarget::blit, max. is %d\n", aTarget, mNumTargets - 1);
            return false;
        }

        __(glBindFramebuffer(GL_READ_FRAMEBUFFER, mFrameBuffer));
        __(glReadBuffer(GL_COLOR_ATTACHMENT0 + aTarget));

        if (aBindMainFB) {
            __(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
        }
        __(glBlitFramebuffer(0, 0, mWidth, mHeight, aDstX, aDstY, aWidth, aHeight, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_LINEAR));
        __(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
        return true;
    }

    void OpenGLSSAARenderTarget::Clear()
    {
        __(glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer));
        __(glDrawBuffers(mNumTargets, mAttachments));
        __(glClearColor(mClearColor.r(), mClearColor.g(), mClearColor.b(), mClearColor.a()));
        __(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }
}
