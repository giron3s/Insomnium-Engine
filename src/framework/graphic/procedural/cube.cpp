/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/

#include "precompiled.h"
#include "graphic/procedural/cube.h"
#include "graphic/procedural/plane.h"
#include "core/graphic/assettransform.h"

namespace Framework
{
    namespace Procedural
    {
        #define PI 3.14159265358979323846

        Cube::Cube(float aWidth, float aHeight, float aDepth, const glm::vec3 &aColor, uint32_t aNumVertsWidth, uint32_t aNumVertsHeight, uint32_t aNumVertsDepth)
            : Asset3D("#cube", "")
            , mWidth(aWidth)
            , mHeight(aHeight)
            , mDepth(aDepth)
            , mColor(aColor)
            , mNumVertsWidth(aNumVertsWidth)
            , mNumVertsHeight(aNumVertsHeight)
            , mNumVertsDepth(aNumVertsDepth)
        {
            float lHalfWidth = mWidth / 2.0f;
            float lHalfHeight = mHeight / 2.0f;
            float lHalfDepth = mDepth / 2.0f;

            glm::vec3 lOffsets[] = {
                glm::vec3(0.0f, lHalfHeight, 0.0f),  /* Top */
                glm::vec3(0.0f, -lHalfHeight, 0.0f), /* Bottom */
                glm::vec3(0.0f, 0.0f, lHalfDepth),   /* Front */
                glm::vec3(0.0f, 0.0f, -lHalfDepth),  /* Back */
                glm::vec3(-lHalfWidth, 0.0f, 0.0f),  /* Left */
                glm::vec3(lHalfWidth, 0.0f, 0.0f),   /* Right */
            };

            glm::vec3 lRotations[] = {
                glm::vec3(0.0, 0.0, 0.0),       /* Top */
                glm::vec3(PI, 0.0, 0.0),        /* Bottom */
                glm::vec3(PI / 2.0, 0.0, 0.0),  /* Front */
                glm::vec3(-PI / 2.0, 0.0, 0.0), /* Back */
                glm::vec3(0.0, 0.0, PI / 2.0),  /* Left */
                glm::vec3(0.0, 0.0, -PI / 2.0), /* Right */
            };

            glm::vec2 lPlaneSizes[] = {
                glm::vec2(mWidth,  mDepth),  /* Top */
                glm::vec2(mWidth,  mDepth),  /* Bottom */
                glm::vec2(mWidth,  mHeight), /* Front */
                glm::vec2(mWidth,  mHeight), /* Back */
                glm::vec2(mHeight, mDepth), /* Left */
                glm::vec2(mHeight, mDepth)  /* Right */
            };

            struct {
                uint32_t mWidth;
                uint32_t mHeight;
            } lPlaneVerts[] = {
                { mNumVertsWidth, mNumVertsDepth },  /* Top */
                { mNumVertsWidth, mNumVertsDepth },  /* Bottom */
                { mNumVertsWidth, mNumVertsHeight }, /* Front */
                { mNumVertsWidth, mNumVertsHeight }, /* Back */
                { mNumVertsHeight, mNumVertsDepth }, /* Left */
                { mNumVertsHeight, mNumVertsDepth }  /* Right */
            };

            for (int i = 0; i < sizeof lOffsets / sizeof *lOffsets; ++i)
            {
                Plane lPlane(lPlaneSizes[i].x, lPlaneSizes[i].y, mColor, lPlaneVerts[i].mWidth, lPlaneVerts[i].mHeight);

                /* Transform the original plane */
                AssetTransform::Rotate(lPlane, lRotations[i]);
                AssetTransform::Translate(lPlane, lOffsets[i]);
                AssetTransform::AppendGeometryOnly(*this, lPlane);
            }

            AssetTransform::SetUniqueMaterialFromColor(*this, mColor);
        }
    }
}
