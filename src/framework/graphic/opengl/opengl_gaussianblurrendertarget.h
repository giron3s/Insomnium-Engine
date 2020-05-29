/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Render target for OpenGL. A render target allows to render objects to it
 *                instead of to the main screen. Then the target can be rendered to the main screen as a texture
 *
 *                The Toon render target applies Fast Approximate Anti-Aliasing from Timothy Lottes paper at Nvidia
 ********************************************************************************/

#pragma once

#include "graphic/gausianblurrendertarget.h"
#include "graphic/shader.h"

#include "graphic/opengl/opengl.h"
#include "graphic/opengl/opengl_filterrendertarget.h"

#include "core/logger.h"

#pragma warning(disable : 4250)

namespace Framework
{
    class OpenGLGaussianBlurRenderTarget : public GaussianBlurRenderTarget, public OpenGLFilterRenderTarget
    {
        public:
            /**
             * Constructor
             *
             * Uses HDR framebuffer
             */
            OpenGLGaussianBlurRenderTarget()
                : OpenGLFilterRenderTarget(true)
            {}

            void SetSize(uint32_t aWidth, uint32_t aHeight) override
            {
               CRASH("not implemented");
            }

        private:
            bool CustomInit()
            {
                std::string lError;
                if (mShader->Load("filters/gaussian_blur", lError) == false)
                {
                    CRASH("ERROR loading shader filters/gaussian_blur: %s\n", lError.c_str());
                    return false;
                }
                return true;
            }
            void SetCustomParams(void)
            {
                mShader->SetUniformBool("umHorizontal", mHorizontal);
                if (mHorizontal == true)
                {
                    mShader->SetUniformFloat("u_texelSize", 1.0f / mWidth);
                } else
                {
                    mShader->SetUniformFloat("u_texelSize", 1.0f / mHeight);
                }
            }
            void UnsetCustomParams(void) { }
    };
}
