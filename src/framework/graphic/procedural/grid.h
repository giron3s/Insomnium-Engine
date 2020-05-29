/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : a stub structure for gpu-generated grid
*******************************************************************************/

#pragma once

#include <glm/glm.hpp>
#include "graphic/model2d.h"

namespace Framework
{
    namespace Procedural
    {
        class Grid : public Model2D
        {
        public:
            Grid(float aMinorStep2D = 1.0f,
                 float aMajorStep2D = 1.0f,
                 float aMinorStep3D = 1.0f,
                 float aMajorStep3D = 1.0f,
                 float aSize3D = 1.0f, 
                 float aRescaleDistance = 10.0f,
                 float aMinorLineWidth = 1.0f,
                 float aMajorLineWidth = 1.0f,
                 const glm::vec3 &aColor = glm::vec3(1.0f, 1.0f, 1.0f));

            float            GetMinorStep2D() const { return mMinorStep2D; }
            float            GetMajorStep2D() const { return mMajorStep2D; }
            float            GetMinorStep3D() const { return mMinorStep3D; }
            float            GetMajorStep3D() const { return mMajorStep3D; }
            float            GetSize3D() const { return mSize3D; }
            float            GetRescaleDistance() const { return mRescaleDistance; }
            float            GetMinorLineWidth() const { return mMinorLineWidth; }
            float            GetMajorLineWidth() const { return mMajorLineWidth; }
            const glm::vec3& GetColor() const { return mColor; }

        private:
            float     mMinorStep2D;
            float     mMajorStep2D;
            float     mMinorStep3D;
            float     mMajorStep3D;
            float     mSize3D;
            float     mRescaleDistance;
            float     mMinorLineWidth;
            float     mMajorLineWidth;
            glm::vec3 mColor;
        };
    };
}
