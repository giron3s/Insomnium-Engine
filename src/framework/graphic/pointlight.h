/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*  Brief       : This type of light is defined by a point in space and radiates in a spherical
*                manner. It can have attenuation.
*
*******************************************************************************/
#pragma once

#include "light.h"

namespace Framework
{
    class PointLight : public Light
    {
    public:
        /**
         * Constructor
         *
         * @param Ambient: ambient component of the light
         * @param Diffuse: diffuse component of the light
         * @param Specular: specular component of the light
         * @param Position: position in world coordinates of this light
         * @param Attenuation: attenuation factor to be used for power attenuation
         * @param Cutoff: cutoff distance from the position of the light where
         *                the light does not affect the  geometry anymore
         */
        PointLight(const glm::vec3& aAmbient = glm::vec3(1.0f, 1.0f, 1.0f),
            const glm::vec3& aDiffuse = glm::vec3(1.0f, 1.0f, 1.0f),
            const glm::vec3& aSpecular = glm::vec3(1.0f, 1.0f, 1.0f),
            const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f),
            float aAttenuation = 1.0f, float aCutoff = 1e6f,
            size_t aEntityID = Entity::GetDefaultID())
            : Light(aAmbient, aDiffuse, aSpecular, aPosition, aEntityID)
            , mAttenuation(aAttenuation)
            , mCutoff(aCutoff)
        {}

        void UpdateViewProjectionMatrix() const override;

        /**
          * Setters
          */
        void SetAttenuation(float aAttenuation)
        {
            ASSERT(0.0f < aAttenuation);
            mAttenuation = aAttenuation;
        }

        void SetCutoff(float aCutoff)
        {
            ASSERT(0.0f < aCutoff);
            mCutoff = aCutoff;
        }
        /**
          * Getters
          */
        inline float GetAttenuation() const { return mAttenuation; }
        inline float GetCutoff() const { return mCutoff; }

        /**
        * Inherited from Object3D to calculate the bounding volumes of this light
        * based on its position and cutoff factor
        */
        void CalculateBoundingVolumes() const override
        {
            float cutoff = mCutoff / glm::sqrt(3.0f);

            mMaxLengthVertex = glm::vec3(cutoff, cutoff, cutoff);

            mOOBB.SetMin(-mMaxLengthVertex);
            mOOBB.SetMax(mMaxLengthVertex);

            UpdateBoundingVolumes();
        }

    protected:
        float mAttenuation; /**< Attenuation factor of this light used for power attenuation */
        float mCutoff;      /**< Cutoff factor to determine at which distance the light does
                                 not affect geometry anymore */
    };
}
