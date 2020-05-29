/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Circle procedural generation
*******************************************************************************/
#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include "graphic/asset3d.h"

namespace Framework
{
    namespace Procedural
    {
        /**
         * Circle class to generate a flat circle
         */
        class Circle : public Asset3D
        {
          public:

            Circle(float aRadius = 1.0f, const glm::vec3 &aColor = glm::vec3(1.0f, 1.0f, 1.0f), uint32_t aNumVertesSide = 20);

            float            GetRadius() const { return mRadius; }
            const glm::vec3& GetColor() const { return mColor; }
            uint32_t         GetNumVertsSide() const { return mNumVertsSide; }

          private:
            float     mRadius;
            glm::vec3 mColor;
            uint32_t  mNumVertsSide;
        };
    };
}
