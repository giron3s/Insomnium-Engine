/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : OpenGL material implemented as a block uniform to be used in a shader
 ********************************************************************************/

#include "precompiled.h"
#include "graphic/opengl/opengl_shaderdirectlight.h"

#include <glm/glm.hpp>

namespace Framework
{
    void OpenGLShaderDirectLight::Init(uint32_t aBindingPoint)
    {
        SetBlockName("DirectLight");
        SetBindingPoint(aBindingPoint);
        AddParamName("direction");
        AddParamName("ambient");
        AddParamName("diffuse");
        AddParamName("specular");
    }

    void OpenGLShaderDirectLight::CopyLight(const DirectLight &aLight)
    {
        SetParamValue("direction", aLight.GetDirection());
        SetParamValue("ambient", aLight.GetAmbient());
        SetParamValue("diffuse", aLight.GetDiffuse());
        SetParamValue("specular", aLight.GetSpecular());
        Bind();
    }
}
