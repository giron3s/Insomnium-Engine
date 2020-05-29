/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Textured procedural quad generation
*******************************************************************************/

#include "precompiled.h"
#include "graphic/procedural/texturedquad.h"
#include "graphic/procedural/proceduralutils.h"
#include "core/graphic/assettransform.h"

//#include <glm.hpp>

namespace Framework
{
    namespace Procedural
    {
        TexturedQuad::TexturedQuad(float aWidth, float aHeight, Texture&& aTexture, uint32_t aNumVertsWidth, uint32_t aNumVertsHeight)
            : Asset2D("#texquad", "")
            , mWidth(aWidth)
            , mHeight(aHeight)
            , mNumVertsWidth(aNumVertsWidth)
            , mNumVertsHeight(aNumVertsHeight)
        {
            const float lHalfWidth = aWidth * 0.5f;
            const float lHalfHeight = aHeight * 0.5f;

            /* Vertices */
            /*

            ccw-oriented triangle strip
            2--3
            |\ |
            | \|
            0--1
            
            */
            mVertexData.resize(4);
            Asset2D::VertexData *lData = &mVertexData[0];

            lData[0].mVertex  = glm::vec2(-0.5f * lHalfWidth, -0.5f * lHalfHeight);
            lData[0].mUVCoord = glm::vec2(0.0f, 0.0f);

            lData[1].mVertex  = glm::vec2(0.5f * lHalfWidth, -0.5f  * lHalfHeight);
            lData[1].mUVCoord = glm::vec2(1.0f, 0.0f);

            lData[2].mVertex  = glm::vec2(-0.5f * lHalfWidth, 0.5f * lHalfHeight);
            lData[2].mUVCoord = glm::vec2(0.0f, 1.0f);

            lData[3].mVertex  = glm::vec2(0.5f * lHalfWidth, 0.5f  * lHalfHeight);
            lData[3].mUVCoord = glm::vec2(1.0f, 1.0f);

            AssetTransform::SetUniqueMaterialFromTexture(*this, std::move(aTexture));
        }

        TexturedQuad::TexturedQuad(float aWidth, float aHeight)
           : Asset2D("#texquad", "")
           , mWidth(aWidth)
           , mHeight(aHeight)
           , mNumVertsWidth(2)
           , mNumVertsHeight(2)
        {

           const float lHalfWidth = aWidth * 0.5f;
           const float lHalfHeight = aHeight * 0.5f;

           /* Vertices */
            /*

            ccw-oriented triangle strip
            2--3
            |\ |
            | \|
            0--1

            */
           mVertexData.resize(4);
           Asset2D::VertexData *lData = &mVertexData[0];

           lData[0].mVertex = glm::vec2(-0.5f * lHalfWidth, -0.5f * lHalfHeight);
           lData[0].mUVCoord = glm::vec2(0.0f, 0.0f);

           lData[1].mVertex = glm::vec2(0.5f * lHalfWidth, -0.5f  * lHalfHeight);
           lData[1].mUVCoord = glm::vec2(1.0f, 0.0f);

           lData[2].mVertex = glm::vec2(-0.5f * lHalfWidth, 0.5f * lHalfHeight);
           lData[2].mUVCoord = glm::vec2(0.0f, 1.0f);

           lData[3].mVertex = glm::vec2(0.5f * lHalfWidth, 0.5f  * lHalfHeight);
           lData[3].mUVCoord = glm::vec2(1.0f, 1.0f);

           AssetTransform::SetUniqueMaterialFromColor(*this, glm::vec3(1, 0, 0));
        }
    }
}

