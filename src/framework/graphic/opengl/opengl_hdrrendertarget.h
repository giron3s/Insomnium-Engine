/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
 * Brief       : Render target for OpenGL. A render target allows to render objects in HDR
 *               with a color buffer of 16-bits floating point. When the render target is resolved
 *               then tone mapping is applied
 ********************************************************************************/

#pragma once

#include "core/logger.h"
#include "graphic/hdrrendertarget.h"
#include "graphic/shader.h"
#include "graphic/opengl/opengl_filterrendertarget.h"

#pragma warning(disable : 4250)

namespace Framework
{
    class OpenGLHDRRenderTarget : public HDRRenderTarget, public OpenGLFilterRenderTarget
    {
        public:
            OpenGLHDRRenderTarget() 
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
                if (mShader->Load("hdr/hdr", lError) == false)
                {
                    CRASH("ERROR loading shader hdr/hdr: %s\n", lError.c_str());
                    return false;
                }
                return true;
            }
            void SetCustomParams(void)
            {
                mShader->SetUniformFloat("u_exposure", mExposure);
                mShader->SetUniformBool("u_tonemapping", mToneMapping);
            }
            void UnsetCustomParams(void) { }
    };
}
