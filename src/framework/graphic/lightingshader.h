/**
 * @class	LightingShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "graphic/directlight.h"
#include "graphic/material.h"
#include "graphic/pointlight.h"
#include "graphic/shader.h"
#include "graphic/spotlight.h"

/* virtual inheritance is used here because on one hand we want
 * LightingShader to be an instantiable Shader for generic
 * lighting shaders, and on the other hand because the
 * implementation dependant <API>LightingShader will inherit
 * from <API>Shader that inherits itself from Shader:
 *
 *                    Shader
 *                    /   \
 *                   /     \
 *       LightingShader    <API>Shader
 *                   \      /
 *                    \    /
 *              <API>LightingShader
 */
namespace Framework
{
    class LightingShader : public virtual Shader
    {
      public:
        virtual uint32_t GetMaxLights() const = 0;
        virtual void     SetDirectLight(const DirectLight &aDirectLight) const = 0;
        virtual void     SetPointLight(uint32_t aNumLight, const PointLight &aPointLight) const = 0;
        virtual void     SetSpotLight(uint32_t aNumLight, const SpotLight &aPointLight) const = 0;
        virtual void     SetMaterial(const Material &aMaterial) const = 0;
    };
}
