/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/

#pragma once

#include "core/logger.h"
#include "graphic/normalshadowmapshader.h"
#include "graphic/shader.h"
#include "graphic/opengl/opengl.h"
#include "graphic/opengl/opengl_shader.h"
#include "graphic/opengl/opengl_lightingshader.h"

#pragma warning(disable : 4250)

namespace Framework
{
    class OpenGLNormalShadowMapShader : public NormalShadowMapShader
                                      , public OpenGLShader
    {
      public:
        bool Init() override
        {
            std::string lError;

            if (Load("shadows/shadowmap_normal", lError) != true)
            {
                CRASH("ERROR loading shader shadows/shadowmap_normal: %s\n", lError.c_str());
                return false;
            }
            return true;
        }
    };
}
