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

#pragma once


#include "graphic/rendertarget.h"
#include "graphic/opengl/opengl.h"

namespace Framework
{
    class OpenGLRenderTarget : public virtual RenderTarget
    {
      public:
        ~OpenGLRenderTarget() override;
        bool Init(uint32_t aWidth, uint32_t aHeight, const void* aParams = nullptr) override;
        bool SetSize(uint32_t aWidth, uint32_t aHeight) override;
        void BindForDrawning() const override;
        void BindForReading() const override;
        void Unbind() const override;
        // Blit implementation here relies on glBlitFramebuffer with blending enabled (I think blending has no effect here)
        void Blit(uint32_t aDstX, uint32_t aDstY, uint32_t aWidth, uint32_t aHeight, uint32_t srcColorBufferIndex = 0, bool dstIsMainFB = true) const override;
        void Clear(uint32_t aMask = 0/*all available*/) const override;

        uint32_t GetTextureId(uint32_t aAttachment = 0/*default: color0 or depth buffer*/) const override;
       
        uint32_t GetColor(uint32_t x, uint32_t y, uint32_t colorBufferIndex) const override;

      protected:
        GLuint mFramebuffer = GL_NONE;
    };
}
