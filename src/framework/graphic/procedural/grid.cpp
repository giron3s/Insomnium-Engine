/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : a stub structure for gpu-generated grid
*******************************************************************************/

#include "precompiled.h"
#include "graphic/procedural/grid.h"


namespace Framework
{
    namespace Procedural
    {

        Grid::Grid(float aMinorStep2D, float aMajorStep2D,
                   float aMinorStep3D, float aMajorStep3D,
                   float aSize3D,
                   float aRescaleDistance,
                   float aMinorLineWidth, float aMajorLineWidth,
                   const glm::vec3 &aColor)

        : Model2D(nullptr, nullptr) // no asset required
        , mMinorStep2D(aMinorStep2D)
        , mMajorStep2D(aMajorStep2D)
        , mMinorStep3D(aMinorStep3D)
        , mMajorStep3D(aMajorStep3D)
        , mSize3D(aSize3D)
        , mRescaleDistance(aRescaleDistance)
        , mMinorLineWidth(aMinorLineWidth)
        , mMajorLineWidth(aMajorLineWidth)
        , mColor(aColor)
        {
            ASSERT(0.0 < mMinorStep2D);
            ASSERT(mMinorStep2D <= mMajorStep2D);
            ASSERT(0.0 < mMinorStep3D);
            ASSERT(mMinorStep3D <= mMajorStep3D);
            ASSERT(0.0f < mRescaleDistance);
            ASSERT(1.0f <= mMinorLineWidth);
            ASSERT(1.0f <= mMajorLineWidth);
            ASSERT(0.0f <= mColor.r && mColor.r <= 1.0 &&
                   0.0f <= mColor.g && mColor.g <= 1.0 &&
                   0.0f <= mColor.b && mColor.b <= 1.0);

              mName.assign("#grid"); // no rename allowed
        }
    }
}
