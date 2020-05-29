/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
********************************************************************************/
#pragma once

#include "graphic/lightemitshader.h"
#include "graphic/shader.h"

#include "graphic/opengl/opengl.h"
#include "graphic/opengl/opengl_lightingshader.h"
#include "graphic/opengl/opengl_shader.h"

#pragma warning(disable : 4250)

namespace Framework
{
    class OpenGLLightEmitShader : public virtual LightEmitShader, public OpenGLLightingShader
    {
      public:
        bool Init() override
        {
            std::string lError;

            if (Load("lighting/lightemit", lError) != true) {
                CRASH("ERROR loading shader lighting/lightemit: %s\n", lError.c_str());
                return false;
            }

            return OpenGLLightingShader::Init();
        }

        void SetCustomParams() const override {}
    };
}
