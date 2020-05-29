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
    class BoundingSphere
    {
    public:
        BoundingSphere(float aRadius = 1.0f) : mRadius(aRadius){ }
        float GetRadius() const { return mRadius; }
        void  SetRadius(float aRadius) { mRadius = aRadius; }

      protected:
        float mRadius; /**< Radius of the sphere from the center of the model that
                            contains all the model's vertices */
    };
}


