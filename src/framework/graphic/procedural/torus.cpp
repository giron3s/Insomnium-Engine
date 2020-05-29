/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Torus procedural generation
*******************************************************************************/

#include "precompiled.h"
#include "graphic/procedural/torus.h"
#include "graphic/procedural/proceduralutils.h"
#include "core/graphic/assettransform.h"

namespace Framework
{
    namespace Procedural
    {
        #define PI 3.14159265358979323846

        Torus::Torus(float aOuterRadius, float aInnerRadius, const glm::vec3 &aColor, uint32_t aNumVertsToroidal, uint32_t aNumVertsPoloidal)
            : Asset3D("#torus", "")
            , mOuterRadius(aOuterRadius)
            , mInnerRadius(aInnerRadius)
            , mColor(aColor)
            , mNumVertsToroidal(aNumVertsToroidal)
            , mNumVertsPoloidal(aNumVertsPoloidal)
        {
            if (mNumVertsToroidal < 3)
                mNumVertsToroidal = 3;
            if (mNumVertsPoloidal < 3)
                mNumVertsPoloidal = 3;

            AppendBentPlane(*this, (float)((mOuterRadius - mInnerRadius) * 2.0f * PI), (float)(mOuterRadius * 2.0f * PI), (float)(2.0f * PI),
                            (float)(2.0f * PI), 0.0f, mNumVertsToroidal + 1, mNumVertsPoloidal + 1);
            AssetTransform::SetUniqueMaterialFromColor(*this, mColor);
        }
    }
}

