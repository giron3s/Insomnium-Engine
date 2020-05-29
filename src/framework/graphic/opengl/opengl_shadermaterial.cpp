/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential

 * Brief        : OpenGL aMaterial implemented as a block uniform to be used in
 *                a shader. Contains ambient, diffuse, specular, alpha and shininess
 *                components
*******************************************************************************/
#include "precompiled.h"
#include "graphic/opengl/opengl_shadermaterial.h"
#include <glm/glm.hpp>

namespace Framework
{
    void OpenGLShaderMaterial::Init(uint32_t aBindingPoint)
    {
        SetBlockName("Material");
        SetBindingPoint(aBindingPoint);
        AddParamName("ambient");
        AddParamName("diffuse");
        AddParamName("specular");
        AddParamName("alpha");
        AddParamName("shininess");
    }

    void OpenGLShaderMaterial::CopyMaterial(const Material &aMaterial)
    {
        SetParamValue("ambient", aMaterial.GetAmbient());
        SetParamValue("diffuse", aMaterial.GetDiffuse());
        SetParamValue("specular", aMaterial.GetSpecular());
        SetParamValue("alpha", aMaterial.GetAlpha());
        SetParamValue("shininess", aMaterial.GetShininess());
        Bind();
    }
}
