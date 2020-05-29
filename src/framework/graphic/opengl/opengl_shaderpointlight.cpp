/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : OpenGL point aLlight implemented as a block uniform to be used in
 *                a shader
 ********************************************************************************/
#pragma once

#include "precompiled.h"
#include "graphic/opengl/opengl_shaderpointlight.h"
#include <glm/glm.hpp>

namespace Framework
{
    void OpenGLShaderPointLight::Init(uint32_t aBindingPoint, uint32_t aLightIndex)
    {
        SetBlockName("PointLight");
        SetBindingPoint(aBindingPoint);
        SetBlockArrayIndex(aLightIndex);
        AddParamName("position");
        AddParamName("ambient");
        AddParamName("diffuse");
        AddParamName("specular");
        AddParamName("attenuation");
        AddParamName("cutoff");
    }

    void OpenGLShaderPointLight::CopyLight(const PointLight &aLlight)
    {
        SetParamValue("position", aLlight.GetPosition());
        SetParamValue("ambient", aLlight.GetAmbient());
        SetParamValue("diffuse", aLlight.GetDiffuse());
        SetParamValue("specular", aLlight.GetSpecular());
        SetParamValue("attenuation", aLlight.GetAttenuation());
        SetParamValue("cutoff", aLlight.GetCutoff());
        Bind();
    }
}
