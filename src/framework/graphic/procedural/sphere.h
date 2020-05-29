 /*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *   Brief      : Sphere procedural generation
 *******************************************************************************/

#pragma once

#include <stdint.h>
#include "graphic/asset3d.h"

namespace Framework
{
    namespace Procedural
    {
        /**
         * Sphere class to generate a sphere
         */
        class Sphere : public Asset3D
        {
          public:
            /**
             * Constructor
             *
             * @param numSides  Number of sides of the cylinder. Minimum is 4 sides, which
             *                  would yield a cube
             * @param color     Color for the default material
             */
            Sphere(float aRadius = 1.0f, const glm::vec3 &aColor = glm::vec3(1.0f, 1.0f, 1.0f), uint32_t aNumVertsLongitude = 20, uint32_t aNumVertsLatitude = 20);

            float            GetRadius() const { return mRadius; }
            const glm::vec3& GetColor() const { return mColor; }
            uint32_t         GetNumVertsLongitude() const { return mNumVertsLongitude; }
            uint32_t         GetNumVertsLatitude() const { return mNumVertsLatitude; }

          private:
            float     mRadius;
            glm::vec3 mColor;
            uint32_t  mNumVertsLongitude;
            uint32_t  mNumVertsLatitude;
        };
    };
}
