/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Torus procedural generation
*******************************************************************************/

#pragma once

#include <stdint.h>
#include "graphic/asset3d.h"

namespace Framework
{
    namespace Procedural
    {

        /**
         * Torus class to generate a procedural Torus
         */
        class Torus : public Asset3D
        {
          public:

            /**
             * Constructor
             *
             * @param numVertices  Number of vertices to use for the cube's faces
             *                     generation. Each face will have numVertices*numVertices
             *                     vertices
             * @param color        Color for the default material
             */
            Torus(float aOuterRadius = 10.0f, float aInnerRadius = 1.0f, const glm::vec3 &aColor = glm::vec3(1.0f, 1.0f, 1.0f),
                  uint32_t aNumVertsToroidal = 20, uint32_t aNumVertsPoloidal = 20);

            float            GetOuterRadius() const { return mOuterRadius; }
            float            GetInnerRadius() const { return mInnerRadius; }
            const glm::vec3& GetColor() const { return mColor; }
            uint32_t         GetNumVertsToroidal() const { return mNumVertsToroidal; }
            uint32_t         GetNumVertsPoloidal() const { return mNumVertsPoloidal; }

          private:
            float     mOuterRadius;
            float     mInnerRadius;
            glm::vec3 mColor;
            uint32_t  mNumVertsToroidal;
            uint32_t  mNumVertsPoloidal;
        };
    }
}
