/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Textured quad procedural generation. This class can create normal flat planes or
 *               bent planes around the z-axis that can be used to generate, for example,
 *               cylinders without the caps
*******************************************************************************/

#pragma once

#include <glm/glm.hpp>
#include "graphic/asset2d.h"


namespace Framework
{
    namespace Procedural
    {
        /**
         * TexturedQuad class to generate axis aligned planes
         */
        class TexturedQuad : public Asset2D
        {
          public:
            /**
             * Constructor
             *
             * @param width Width of the plane along the x-axis
             * @param height Height of the plane along the z-axis
             * @param texture Texture of the quad
             * @param numVertsWidth  Number of vertices to generate along the x-axis
             * @param numVertsHeight Number of vertices to generate along the y-axis
             */
            TexturedQuad(float aWidth, float aHeight, Texture&& aTexture, uint32_t aNumVertsWidth = 2, uint32_t aNumVertsHeight = 2);
            TexturedQuad(float aWidth, float aHeight);

            float            GetWidth() const { return mWidth; }
            float            GetHeight() const { return mHeight; }
            uint32_t         GetNumVertsWidth() const { return mNumVertsWidth; }
            uint32_t         GetNumVertsHeight() const { return mNumVertsHeight; }
            const glm::vec2& GetImageScale() const { return mImageScale; }
            const glm::vec2& GetImageOffset() const { return mImageOffset; }

          private:
            float     mWidth;
            float     mHeight;
            uint32_t  mNumVertsWidth;
            uint32_t  mNumVertsHeight;
                      
            glm::vec2 mImageScale;
            glm::vec2 mImageOffset;
        };
    };
}
