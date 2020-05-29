/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : OpenGL material implemented as a block uniform to be used in a shader
 ********************************************************************************/

#pragma once

#include "graphic/directlight.h"
#include "graphic/opengl/opengl.h"
#include "graphic/opengl/opengl_uniformblock.h"

namespace Framework
{
    class OpenGLShaderDirectLight : public OpenGLUniformBlock
    {
      public:
        void Init(uint32_t aBindingPoint);
        void CopyLight(const DirectLight &aLight);
    };
}
