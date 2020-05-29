/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Sphere procedural generation
*******************************************************************************/

#include "precompiled.h"
#include "graphic/procedural/sphere.h"
#include "graphic/procedural/circle.h"
#include "graphic/procedural/proceduralutils.h"
#include "core/graphic/assettransform.h"

namespace Framework
{
    namespace Procedural
    {
        #define PI 3.14159265358979323846

        Sphere::Sphere(float aRadius, const glm::vec3 &aColor, uint32_t aNumVertsLongitude, uint32_t aNumVertsLatitude)
            : Asset3D("#sphere", "")
            , mRadius(aRadius)
            , mColor(aColor)
            , mNumVertsLongitude(aNumVertsLongitude)
            , mNumVertsLatitude(aNumVertsLatitude)
        {
            AppendBentPlane(*this, (float)(2.0f * PI * mRadius), (float)(2.0f * mRadius), (float)(2.0f * PI), 0.0f, (float)(2.0f * PI),
                            mNumVertsLongitude + 1, mNumVertsLatitude);
            AssetTransform::Translate(*this, glm::vec3(0.0, -mRadius, 0.0));
            AssetTransform::SetUniqueMaterialFromColor(*this, mColor);
        }
    }
}
