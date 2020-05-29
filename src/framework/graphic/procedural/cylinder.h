/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Cylinder procedural generation
*******************************************************************************/
#pragma once

#include <stdint.h>
#include "graphic/asset3d.h"

namespace Framework
{
    namespace Procedural
    {
        /**
         * Cylinder class to generate a cube with axis aligned planes
         */
        class Cylinder : public Asset3D
        {
          public:
            /**
             * Constructor
             *
             * @param numSides  Number of sides of the cylinder. Minimum is 4 sides, which
             *                  would yield a cube
             * @param color     Color for the default material
             */
            Cylinder(float aRadius = 1.0f, float aHeight = 1.0f, const glm::vec3 &aColor = glm::vec3(1.0f, 1.0f, 1.0f), uint32_t aNumVertsCap = 20,
                     uint32_t aNumVertsHeight = 2);

            float            GetRadius() const { return mRadius; }
            float            GetHeight() const { return mHeight; }
            const glm::vec3& GetColor() const { return mColor; }
            uint32_t         GetNumVertsCap() const { return mNumVertsCap; }
            uint32_t         GetNumVertsHeight() const { return mNumVertsHeight; }

          private:
            void BuildBody();

            float     mRadius;
            float     mHeight;
            glm::vec3 mColor;
            uint32_t  mNumVertsCap;
            uint32_t  mNumVertsHeight;
        };
    };
}
