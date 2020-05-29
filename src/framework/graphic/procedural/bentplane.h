/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Bent plane procedural generation
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
         * BentPlane class to generate axis aligned planes
         */
        class BentPlane : public Asset3D
        {
          public:
            /**
             * Constructor
             *
             * @param width  Width of the plane along the x-axis
             * @param height Height of the plane along the z-axis
             * @param color  Default color for the plane material
             * @param angle  Bending angle for the plane. If 0.0f then a degenerate plane is,
             *               otherwise the plane is bent around the z-axis preserving its width on an
             *               arc indicated by 'angle'
             * @param numVertsWidth  Number of vertices to generate along the x-axis
             * @param numVertsHeight Number of vertices to generate along the y-axis
             */
            BentPlane(float aWidth = 1.0f, float aHeight = 1.0f, const glm::vec3 &aColor = glm::vec3(1.0f, 1.0f, 1.0f), float aAngle = 1.0f,
                      uint32_t aNumVertsWidth = 2, uint32_t aNumVertsHeight = 2);

            /**
             * Accessors
             */
            float             GetWidth() const { return mWidth; }
            float             GetHeight() const { return mHeight; }
            const glm::vec3&  GetColor() const { return mColor; }
            uint32_t          GetNumVertsWidth() const { return mNumVertsWidth; }
            uint32_t          GetNumVertsHeight() const { return mNumVertsHeight; }

          private:
            float     mWidth;
            float       mHeight;
            glm::vec3   mColor;
            float       mAngle;
            uint32_t    mNumVertsWidth;
            uint32_t    mNumVertsHeight;
        };
    };
}
