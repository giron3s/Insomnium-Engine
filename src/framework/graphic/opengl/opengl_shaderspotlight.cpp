/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : OpenGL spot aLight implemented as a block uniform to be used in
 *                a shader
 ********************************************************************************/
#include "precompiled.h"
#include "graphic/opengl/opengl_shaderspotlight.h"
#include <glm/glm.hpp>

namespace Framework
{
    void OpenGLShaderSpotLight::Init(uint32_t aBindingPoint, uint32_t aLightIndex)
    {
        SetBlockName("SpotLight");
        SetBindingPoint(aBindingPoint);
        SetBlockArrayIndex(aLightIndex);
        AddParamName("position");
        AddParamName("direction");
        AddParamName("ambient");
        AddParamName("diffuse");
        AddParamName("specular");
        AddParamName("coneAngle");
        AddParamName("conePenumbra");
        AddParamName("attenuation");
        AddParamName("cutoff");
    }

    void OpenGLShaderSpotLight::CopyLight(const SpotLight &aLight)
    {
        SetParamValue("position", aLight.GetPosition());
        SetParamValue("direction", aLight.GetDirection());
        SetParamValue("ambient", aLight.GetAmbient());
        SetParamValue("diffuse", aLight.GetDiffuse());
        SetParamValue("specular", aLight.GetSpecular());
        SetParamValue("coneAngle", aLight.GetConeAngle());
        SetParamValue("conePenumbra", aLight.GetConePenumbra());
        SetParamValue("attenuation", aLight.GetAttenuation());
        SetParamValue("cutoff", aLight.GetCutoff());
        Bind();
    }
}
