/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*  Brief       : Class to abstract a material. A material
*                contains ambient, diffuse, specular, alpha and shininess
 *               components
*******************************************************************************/

#pragma once

#include <stdint.h>
#include "glm/glm.hpp"

namespace Framework
{

#pragma pack(push)
#pragma pack(1)
    class Material
    {
      public:

        /* The default constructor sets some wierd material
         * to identify it visually if some materials are not
         * set properly */
        Material(bool aStrange = false)
            : mAmbient(glm::vec3(0.2f, 0.2f, 0.2f))
            , mDiffuse(glm::vec3(1.0f, 1.0f, 1.0f))
            , mSpecular(glm::vec3(0.0f, 0.0f, 0.0f))
            , mAlpha(1.0)
            , mShininess(0.0f)
        {
            if (aStrange) {
                mAmbient = glm::vec3(1.0f, 1.0f, 0.0f);
                mDiffuse = glm::vec3(0.0f, 1.0f, 1.0f);
                mSpecular = glm::vec3(1.0f, 0.0f, 1.0f);
                mAlpha = 1.0;
                mShininess = 0.5f;
            }
        }

// TODO: add checks that all values except for shininess are in [0,1] range
// and shininess value is in [0,128] range
        Material(const glm::vec3 &aAmbient,
                 const glm::vec3 &aDiffuse,
                 const glm::vec3 &aSpecular,
                 float aAlpha,
                 float aShininess)
            : mAmbient(aAmbient)
            , mDiffuse(aDiffuse)
            , mSpecular(aSpecular)
            , mAlpha(aAlpha)
            , mShininess(aShininess)
        {

        }

        void         SetAmbient(const glm::vec3 &aAmbient) { mAmbient = aAmbient; }
        void         SetDiffuse(const glm::vec3 &aDiffuse) { mDiffuse = aDiffuse; }
        void         SetSpecular(const glm::vec3 &aSpecular) { mSpecular = aSpecular; }
        void         SetAlpha(float aAlpha) { mAlpha = aAlpha; }
        void         SetShininess(float aShininess) { mShininess = aShininess; }

        const glm::vec3&  GetAmbient() const { return mAmbient; }
        const glm::vec3&  GetDiffuse() const { return mDiffuse; }
        const glm::vec3&  GetSpecular() const { return mSpecular; }
        float             GetAlpha() const { return mAlpha; }
        float             GetShininess() const { return mShininess; }

      protected:
        glm::vec3  mAmbient;
        glm::vec3  mDiffuse;
        glm::vec3  mSpecular;
        //glm::vec3  mTransmittance;
        //glm::vec3  mEmission;
        float      mAlpha;
        //float      mIndexOfRefraction;  // index of refraction
        float      mShininess;
    };
#pragma pack(pop)
}
