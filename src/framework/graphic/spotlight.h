/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*  Brief       :
*******************************************************************************/

#pragma once

#include "pointlight.h"

namespace Framework
{
    class SpotLight : public PointLight
    {
    public:
        /**
         * Constructor
         *
         * @param Ambient: ambient component of the light
         * @param Diffuse: diffuse component of the light
         * @param Specular: specular component of the light
         * @param Position: position in world coordinates of this light
         * @param Attenuation: Attenuation factor to be used for power attenuation
         * @param Cutoff: Cutoff distance from the position of the light where
         *                the light does not affect the  geometry anymore, in world units
         * @param ConeAngle: Angle of the light cone where the light hits with normal attenuation
         * @param ConePenumbra: Angle extension from 'ConeAngle' where the light attenuation is stronger
         *                      until it fades to black, in degrees
         */
        SpotLight(const glm::vec3& aAmbient = glm::vec3(1.0f, 1.0f, 1.0f),
            const glm::vec3& aDiffuse = glm::vec3(1.0f, 1.0f, 1.0f),
            const glm::vec3& aSpecular = glm::vec3(1.0f, 1.0f, 1.0f),
            const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f),
            float aAttenuation = 1.0f,
            float aCutoff = 1e6f,
            float aConeAngle = 45.0f,
            float aConePenumbra = 3.0f,
            size_t aEntityID = Entity::GetDefaultID())
            : PointLight(aAmbient, aDiffuse, aSpecular, aPosition,
                         aAttenuation, aCutoff, aEntityID)
            , mConeAngle(aConeAngle)
            , mConePenumbra(aConePenumbra)
        {}

        void UpdateViewProjectionMatrix() const override;

        /**
         * Setters
         */
        void SetConeAngle(float aConeAngle)
        {
            ASSERT(0.0f < aConeAngle && aConeAngle < 180.0f);
            mConeAngle = aConeAngle;
        }

        void SetConePenumbra(float aConePenumbra)
        {
            ASSERT(0.0f < aConePenumbra && aConePenumbra < 90.0f);
            mConePenumbra = aConePenumbra;
        }


        /**
         * Getters
         */
        inline float GetConeAngle() const { return mConeAngle; }
        inline float GetConePenumbra() const { return mConePenumbra; }

    protected:
        float mConeAngle;    /**< Angle of the light cone where light attenuates normally */
        float mConePenumbra; /**< Angle extension from the 'coneAngle' where light attenuation
                              is stronger until it fades to black */
    };
}
