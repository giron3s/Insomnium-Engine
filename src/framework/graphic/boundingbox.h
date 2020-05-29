/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

#include "glm/glm.hpp"

namespace Framework
{
    class BoundingBox
    {
    public:
        BoundingBox()
            : mMin(glm::vec3(-1.0f, -1.0f, -1.0f))
            , mMax(glm::vec3(1.0f, 1.0f, 1.0f))
        {

        }

        BoundingBox(const glm::vec3 &aMin, const glm::vec3 &aMax)
            : mMin(aMin)
            , mMax(aMax)
        {
            ASSERT(mMin.x <= mMax.x && mMin.y <= mMax.y && mMin.z <= mMax.z);
        }

        const glm::vec3 &GetMin() const { return mMin; }
        const glm::vec3 &GetMax() const { return mMax; }
        void             SetMin(const glm::vec3 &aMin) { mMin = aMin; }
        void             SetMax(const glm::vec3 &aMax) { mMax = aMax; }

        BoundingBox operator*(const glm::mat3 &aTransform)
        {
            BoundingBox lTmp;
            lTmp.mMin = aTransform * mMin;
            lTmp.mMax = aTransform * mMax;
            return lTmp;
        }

    private:
        glm::vec3 mMin; /**< Minimum values of the bounding box */
        glm::vec3 mMax; /**< Maximum values of the bounding box */
    };
}
