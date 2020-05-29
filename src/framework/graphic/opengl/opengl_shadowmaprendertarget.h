/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential

 * Brief        : Render target for OpenGL. A render target allows to render objects to it
 *                instead of to the main screen. Then the target can be rendered to the main screen as
 *                a texture
 *
 *                The ShadowMap render target applies no anti-aliasing
*******************************************************************************/
#pragma once

#include "graphic/shadowmaprendertarget.h"
#include "graphic/opengl/opengl_rendertarget.h"

namespace Framework
{
    class OpenGLShadowMapRenderTarget : public ShadowMapRenderTarget, public OpenGLRenderTarget
    {
      public:
        // Blit implementation here relies on custom shader
        void Blit(uint32_t aDstX, uint32_t aDstY, uint32_t aWidth, uint32_t aHeight, uint32_t srcColorBufferIndex = 0, bool dstIsMainFB = true) const override;
    };

}
