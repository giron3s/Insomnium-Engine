/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Bent plane procedural generation
*******************************************************************************/
#include "precompiled.h"
#include "graphic/procedural/bentplane.h"
#include "graphic/procedural/proceduralutils.h"
#include "core/graphic/assettransform.h"

namespace Framework
{
    namespace Procedural
    {

        #define PI 3.14159265358979323846

        BentPlane::BentPlane(float aWidth, float aHeight, const glm::vec3 &aColor, float aAngle, uint32_t aNumVertsWidth, uint32_t aNumVertsHeight)
            : Asset3D("#bentplane", "")
            , mWidth(aWidth), mHeight(aHeight), mColor(aColor), mAngle(aAngle), mNumVertsWidth(aNumVertsWidth), mNumVertsHeight(aNumVertsHeight)
        {
            AppendBentPlane(*this, mWidth, mHeight, mAngle, 0.0f, 0.0f, mNumVertsWidth, mNumVertsHeight);
            AssetTransform::SetUniqueMaterialFromColor(*this, mColor);
        }
    }
}
