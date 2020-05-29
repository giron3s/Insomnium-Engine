/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Cube procedural generation
*******************************************************************************/

#pragma once

#include <stdint.h>
#include "graphic/asset3d.h"

namespace Framework
{
    namespace Procedural
    {
        /**
         * Cube class to generate a cube with axis aligned planes
         */
        class Cube : public Asset3D
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
            Cube(float aWidth = 1.0f, float aHeight = 1.0f, float aDepth = 1.0f, const glm::vec3 &aColor = glm::vec3(1.0f, 1.0f, 1.0f),
                 uint32_t aNumVertsWidth = 2, uint32_t aNumVertsHeight = 2, uint32_t aNumVertsDepth = 2);

            float            GetWidth() const { return mWidth; }
            float            GetHeight() const { return mHeight; }
            float            GetDepth() const { return mDepth; }
            const glm::vec3& GetColor() const { return mColor; }
            uint32_t         GetNumVertsWidth() const { return mNumVertsWidth; }
            uint32_t         GetNumVertsHeight() const { return mNumVertsHeight; }
            uint32_t         GetNumVertsDepth() const { return mNumVertsDepth; }

          private:
            float     mWidth;
            float     mHeight;
            float     mDepth;
            glm::vec3 mColor;
            uint32_t  mNumVertsWidth;
            uint32_t  mNumVertsHeight;
            uint32_t  mNumVertsDepth;
        };
    };
}
