/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*******************************************************************************/

#include "precompiled.h"
#include "graphic/procedural/cylinder.h"
#include "graphic/procedural/circle.h"
#include "core/graphic/assettransform.h"

namespace Framework
{
    namespace Procedural
    {

        #define PI 3.14159265358979323846

        Cylinder::Cylinder(float aRadius, float aHeight, const glm::vec3 &aColor, uint32_t aNumVertsCap, uint32_t aNumVertsHeight)
            : Asset3D("#cylinder", "")
            , mRadius(aRadius)
            , mHeight(aHeight)
            , mColor(aColor)
            , mNumVertsCap(aNumVertsCap)
            , mNumVertsHeight(aNumVertsHeight)
        {
            Circle lBottomCap(mRadius, mColor, mNumVertsCap);
            Circle lTopCap(mRadius, mColor, mNumVertsCap);

            /* Bottom cap */
            AssetTransform::Rotate(lBottomCap, glm::vec3(PI, 0.0f, 0.0f));
            AssetTransform::AppendGeometryOnly(*this, lBottomCap);

            /* Upper cap */
            AssetTransform::Translate(lTopCap, glm::vec3(0.0f, mHeight, 0.0f));
            AssetTransform::AppendGeometryOnly(*this, lTopCap);

            /* Body */
            Asset3D lBody("","");
            AppendBentPlane(lBody, (float)(2.0f * PI * mRadius), mHeight, (float)(2.0 * PI), 0.0f, 0.0f, mNumVertsCap + 1, mNumVertsHeight);

            AssetTransform::Translate(lBody, glm::vec3(0.0f, -mRadius, 0.0f));
            AssetTransform::Rotate(lBody, glm::vec3(-PI / 2.0f, 0.0f, 0.0f));
            AssetTransform::Translate(lBody, glm::vec3(0.0f, mRadius, 0.0f));
            AssetTransform::Rotate(lBody, glm::vec3(0.0f, PI / mNumVertsCap, 0.0f));
            AssetTransform::AppendGeometryOnly(*this, lBody);

            /* Generate the material */
            AssetTransform::SetUniqueMaterialFromColor(*this, mColor);
        }
    }
}

