/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*  Brief       : Interface for a render target. A render target allows to render objects to it
*                instead of to the main screen. Then the target can be rendered to the main screen as
*                a texture
*******************************************************************************/
#pragma once

#include <stdint.h>
#include "graphic/gui/common.h"
#include "graphic/shader.h"

using namespace Framework::GUI;

namespace Framework
{
    constexpr uint32_t MAX_COLOR_BUFFERS = 16;

    class RenderTarget
    {
    public:

        struct Attachment
        {
            uint16_t ImplementationType; // GL_TEXTURE_2D or GL_RENDERBUFFER
            uint16_t DataFormat; // for color buffers: {GL_R8, GL_RG8, ..., GL_R16F, ..., GL_RGBA32F};
                                 // for depth buffers: {GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT32F};
                                 // for stencil buffers: {GL_STENCIL_INDEX1, GL_STENCIL_INDEX4, GL_STENCIL_INDEX8, GL_STENCIL_INDEX16};
                                 // for depth-stencil buffers: {GL_DEPTH24_STENCIL8, GL_DEPTH32F_STENCIL8}.
            float    ClearValue[4]; // note: all values must be normalized (in [0,1] range);
            uint32_t Id;         // reserved, must be 0
        };
        using Attachments = std::map<uint16_t/*attachment type: color[i], depth, stencil or depth-stencil*/, Attachment>;
        
        /* Default clear values for different attachment types: */
        #define DEFAULT_COLOR_CLEAR_VALUE {0.0f/*r*/, 0.0f/*g*/, 0.0f/*b*/, 1.0f/*a*/}
        #define DEFAULT_DEPTH_CLEAR_VALUE {1.0f/*depth*/, 0.0f/*-*/, 0.0f/*-*/, 0.0f/*-*/}
        #define DEFAULT_STENCIL_CLEAR_VALUE {0.0f/*stencil*/, 0.0f/*-*/, 0.0f/*-*/, 0.0f/*-*/}
        #define DEFAULT_DEPTH_STENCIL_CLEAR_VALUE {1.0f/*depth*/, 0.0f/*stencil*/, 0.0f/*-*/, 0.0f/*-*/}

        RenderTarget()
            : mSize(0, 0)
            , mBlendingMode(BLENDING_NONE)
            , mShader(nullptr)
        {}

        virtual ~RenderTarget() = default;

        /**
         * Name Get/Set
         */
        inline const std::string& GetName() const { return mName; }

        void SetName(const std::string& aName)
        {
            if ( aName.empty() )
            {
                static uint32_t sID = 1;
                mName.assign("RenderTarget_").append(std::to_string(sID));
                ++sID;
            }
            else
                mName = aName;
        }

        /**
         * Initializes the render target with the given configuration
         *
         * @width       Width of the render target
         * @height      Height of the render target
         * @params      Implementation-dependent parameters
         *
         * @return true or false
         */
        virtual bool Init(uint32_t aWidth, uint32_t aHeight, const void* aParams = nullptr) = 0;

        /**
        * Resizes the render target
        *
        * @width       Width of the render target
        * @height      Height of the render target
        */
        virtual bool SetSize(uint32_t aWidth, uint32_t aHeight) = 0;

        /**
         * Sets the clear color/depth/stencil/depth-stencil value for given attachment
         */
        void SetClearValue(uint32_t aAttachment, const float* aClearValue);

        /**
         * Binds the render target to the current render context so it receives the
         * retsult of the rendering commands. It binds all buffers belonging to the
         * target (color, depth, stencil)
         */
        virtual void BindForDrawning() const = 0;

        virtual void BindForReading() const = 0;

        /**
         * Unbinds the render target from the current rendering context
         */
        virtual void Unbind() const = 0;

        /**
        * Returns OpenGL texture id for attachment given in parameter.
        If attachment is not found or not a texture the return value is zero.
        */
        virtual uint32_t GetTextureId(uint32_t aAttachment = 0/*default*/) const = 0;

        virtual uint32_t GetColor(uint32_t x, uint32_t y, uint32_t colorBufferIndex = 0) const = 0;

        /**
         * Blits the contents of the render target onto the main frambuffer
         * or onto another render target.
         *
         * In case of a multi-target render target, one of the multi-targets must
         * be selected for the blitting operation using the 'target' parameter
         *
         * @param dstX       X coordinate in the destination buffer where the X origin
         *                   of the target being blitted will be copied to
         * @param dstY       Y coordinate in the destination buffer where the Y origin
         *                   of the target being blitted will be copied to
         * @param width      Width of the source target to copy onto the final buffer
         * @param height     Height of the source target to copy onto the final buffer
         * @param target     In a multi-target configuration, the number of the source
         *                   target to use for blitting, use 0 for a single target
         * @param bindMainFB If 'true' it binds the main framebuffer by defaults. Use this
         *                   to blit into the final buffer. To blit into another render
         *                   target you must pass 'false'
         *
         * @return true or false
         */
        virtual void Blit(uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height, uint32_t srcColorBufferIndex = 0, bool dstIsMainFB = true) const = 0;

        /**
         * Convenience function with defaults to blit the whole render target
         * into the main framebuffer
         */
         //virtual bool Blit() { return Blit(0, 0, mSize.x, mSize.y); }

         /**
          * Clears the contents of the buffers
          */
        virtual void Clear(uint32_t aMask = 0/*default*/) const = 0;

        /**
         * Blending mode used in blitting operations
         */
        enum BlendingMode {
            BLENDING_NONE = 0,     /**> No blending mode */
            BLENDING_ADDITIVE = 1  /**> Additive blending mode */
        };

        /**
         * Sets the blending mode for the next blitting operation
         *
         * @mode  The new blending mode. @see BlendingMode
         */
        void SetBlendingMode(BlendingMode mode) { mBlendingMode = mode; }

        /**
         * Gets the width and the height of the render target
         */
        inline glm::uvec2 GetSize() const { return mSize; }

        static RenderTarget* New();

        int GetNumColorBuffers() { return mNumColorBuffers; }

    protected:
        std::string  mName;
        glm::uvec2   mSize;         /**> Size of the render target */
        Attachments  mAttachments;
        uint16_t     mNumColorBuffers = 0,
                     mNumDepthBuffers = 0,
                     mNumStencilBuffers = 0;
        BlendingMode mBlendingMode; /**> Blending mode for the blitting operation */
        std::unique_ptr<Shader> mShader;
    };
}
