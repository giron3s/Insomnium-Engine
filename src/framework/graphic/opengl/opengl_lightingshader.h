/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 ********************************************************************************/
#pragma once

#include "graphic/lightingshader.h"
#include "graphic/opengl/opengl.h"
#include "graphic/opengl/opengl_shader.h"
#include "graphic/opengl/opengl_shaderdirectlight.h"
#include "graphic/opengl/opengl_shaderpointlight.h"
#include "graphic/opengl/opengl_shaderspotlight.h"

#pragma warning(disable : 4250)

/* Even though OpenGLLightingShader is the end of the diamond for
 * the following diagram:
 *
 *                    Shader
 *                    /    \
 *                   /      \
 *       LightingShader   <API>Shader
 *                   \      /
 *                    \    /
 *              <API>LightingShader
 *
 * and it should not need virtual inheritance anymore, it happens to be
 * involved in another diamond, which relates it to any specialization
 * of the LightingShader, like BlinnPhongShader, thus it must inherit
 * virtually from LightingShader.
 */

namespace Framework
{
    class OpenGLLightingShader : public virtual LightingShader
                               , public OpenGLShader
    {
        uint32_t mMaxLights;

      public:

        OpenGLLightingShader(uint32_t aMaxLights = 4)
            : mMaxLights(aMaxLights)
        {
            if ( mMaxLights )
            {
                mPointLights.resize(mMaxLights);
                mSpotLights.resize(mMaxLights);
            }
        }

        bool Init() override
        {
            uint32_t bindingPoint = 1;

            Attach();

            mMaterial.Init(bindingPoint++);
            if (mMaterial.PrepareForShader(mProgramID) != true)
            {
                CRASH("ERROR preparing material for generic lighting shader\n");
                return false;
            }

            if ( mMaxLights )
            {
                mDirectLight.Init(bindingPoint++);
                if (mDirectLight.PrepareForShader(mProgramID) != true)
                {
                    CRASH("ERROR preparing direct light for generic lighting shader\n");
                    return false;
                }
            }

            for (uint32_t i = 0; i < mMaxLights; ++i)
            {
                mPointLights[i].Init(bindingPoint++, i);
                if (mPointLights[i].PrepareForShader(mProgramID) != true)
                {
                    CRASH("ERROR preparing point light %d for generic lighting shader\n", i);
                    return false;
                }
            }

            for (uint32_t i = 0; i < mMaxLights; ++i)
            {
                mSpotLights[i].Init(bindingPoint++, i);
                if (mSpotLights[i].PrepareForShader(mProgramID) != true)
                {
                    CRASH("ERROR preparing spot light %d for generic lighting shader\n", i);
                    return false;
                }
            }

            Detach();
            return true;
        }

        uint32_t GetMaxLights() const override { return mMaxLights; }
        
        void SetDirectLight(const DirectLight &directLight) const override
        { mDirectLight.CopyLight(directLight); }
        
        void SetPointLight(uint32_t numLight, const PointLight &pointLight) const override
        {
            if (numLight >= mMaxLights) {
                CRASH("ERROR light number %d higher than max. %d in generic lighting shader\n", numLight, mMaxLights);
                return;
            }
            mPointLights[numLight].CopyLight(pointLight);
        }

        void SetSpotLight(uint32_t numLight, const SpotLight &spotLight) const override
        {
            if (numLight >= mMaxLights) {
                CRASH("ERROR light number %d higher than max. %d in generic lighting shader\n", numLight, mMaxLights);
                return;
            }
            mSpotLights[numLight].CopyLight(spotLight);
        }

        void SetMaterial(const Material &aMaterial) const override 
        { 
            mMaterial.CopyMaterial(aMaterial);
        }
        //virtual void SetCustomParams() const override = 0;

      private:
        mutable OpenGLShaderMaterial                 mMaterial;
        mutable OpenGLShaderDirectLight              mDirectLight;
        mutable std::vector<OpenGLShaderPointLight>  mPointLights;
        mutable std::vector<OpenGLShaderSpotLight>   mSpotLights;
    };

}
