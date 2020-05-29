/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*  Brief       : This type of light is defined by a direction in space and has no cutoff point
*                or attenuation, affects all things equally. Typical use is for the light radiated
*                from the sun or the moon
*******************************************************************************/

#pragma once

#include "light.h"

namespace Framework
{
    class DirectLight : public Light
    {
    public:
        /**
         * Constructor
         *
         * @param Ambient: ambient component of the light
         * @param Diffuse: diffuse component of the light
         * @param Specular: specular component of the light
         * @param Position: any_vertex-to-light vector (doesn't need to be normalized)
         */
        DirectLight(const glm::vec3& aAmbient = glm::vec3(1.0f, 1.0f, 1.0f),
            const glm::vec3& aDiffuse = glm::vec3(1.0f, 1.0f, 1.0f),
            const glm::vec3& aSpecular = glm::vec3(1.0f, 1.0f, 1.0f),
            const glm::vec3& aPosition = glm::vec3(0.0f, 1.0f, 0.0f),
            size_t aEntityID = Entity::GetDefaultID())
            : Light(aAmbient, aDiffuse, aSpecular, aPosition, aEntityID)
        {}

        mutable glm::vec3 mProjectionVolume{1000.0f, 1000.0f, 1000.0f}; // hint for ortho projection

        void UpdateViewProjectionMatrix() const override;
    };
}
