/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Procedural plane generation
*******************************************************************************/

#include "precompiled.h"
#include "graphic/procedural/plane.h"
#include "graphic/procedural/proceduralutils.h"
#include "core/graphic/assettransform.h"

#include <glm/glm.hpp>

namespace Framework
{
    namespace Procedural
    {
        #define PI 3.14159265358979323846

        Plane::Plane(float aWidth, float aHeight, const glm::vec3 &aColor, uint32_t aNumVertsWidth, uint32_t aNumVertsHeight)
            : Asset3D("#plane", "")
            , mWidth(aWidth)
            , mHeight(aHeight)
            , mColor(aColor)
            , mNumVertsWidth(aNumVertsWidth)
            , mNumVertsHeight(aNumVertsHeight)
        {
            AppendBentPlane(*this, mWidth, mHeight, 0.0f, 0.0f, 0.0f, mNumVertsWidth, mNumVertsHeight);
            AssetTransform::SetUniqueMaterialFromColor(*this, mColor);
        }
    }
}

