/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Render target for OpenGL. A render target allows to render objects to it
*                instead of to the main screen. Then the target can be rendered to the main screen as a texture
*
*                The FXAA2 render target applies Fast Approximate Anti-Aliasing from Timothy Lottes paper t Nvidia
*******************************************************************************/

#pragma once


#include "graphic/fxaa2rendertarget.h"
#include "graphic/shader.h"
#include "graphic/opengl/opengl.h"
#include "graphic/opengl/opengl_filterrendertarget.h"

#pragma warning(disable : 4250)

namespace Framework
{
    class OpenGLFXAA2RenderTarget : public FXAA2RenderTarget, public OpenGLFilterRenderTarget
    {
    public:
       void SetSize(uint32_t aWidth, uint32_t aHeight) override
       {
          CRASH("not implemented");
       }

      private:
        bool CustomInit()
        {
            std::string lError;
            if( mShader->Load("anti-aliasing/fxaa", lError) == false)
            {
                CRASH("ERROR loading shader anti-aliasing/fxaa: %s\n", lError.c_str());
                return false;
            }
            return true;
        }
        void SetCustomParams(void)
        {
            glm::vec2 lRpcFrame(1.0f / mWidth, 1.0f / mHeight);
            mShader->SetUniformVec2("f_rpcFrame",lRpcFrame);
            __(glDisable(GL_BLEND));
            __(glDisable(GL_DEPTH_TEST));
        }

        void UnsetCustomParams(void)
        {
            glEnable(GL_DEPTH_TEST);
        }
    };

}
