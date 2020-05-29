/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Motion for walking movement. This motion uses yaw and pitch and
 *                the rotations are always aligned to the world axes, not the
 *                object own axes. The y position of the motion is constant
********************************************************************************/

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "graphic/motion.h"

namespace Framework
{
    class WalkingMotion : public Motion
    {
      public:
        WalkingMotion();

        void ApplyTo(Object3D &aObj);
        void Reset();

        void SetPosition(const glm::vec3 &aPos);
        void Forward(float aAmount);
        void Right(float aAmount);
        void RotatePitch(float aAngle);
        void RotateYaw(float aAngle);

      private:
        glm::vec4 mForward;
        glm::vec4 mUp;
        glm::vec4 mRight;
        float     mYaw, mPitch;
        float     mForwardAmount, mRightAmount;
        glm::vec3 mPosition;
    };
}
