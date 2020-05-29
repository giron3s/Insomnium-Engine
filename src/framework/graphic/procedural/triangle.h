/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Triangle procedural generation
*******************************************************************************/

#pragma once

#include <stdint.h>
#include "graphic/asset3d.h"

namespace Framework
{
    namespace Procedural
    {
        /**
         * Triangle class to generate a cube with axis aligned planes
         */
        class Triangle : public Asset3D
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
            Triangle(const glm::vec3 &aVertex0, const glm::vec3 &aVertex1, const glm::vec3 &aVertex2,
                     const glm::vec3 &aColor = glm::vec3(1.0f, 1.0f, 1.0f));

            const glm::vec3 &GetColor() const { return mColor; }

          private:
            glm::vec3 mColor;
        };
    };
}
