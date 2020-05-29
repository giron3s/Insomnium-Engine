/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief	    : Motion for free fly movement. This motion uses yaw, pitch and roll and
 *                the rotations are always aligned to the motion object own axes.
*******************************************************************************/
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "graphic/motion.h"

namespace Framework
{
    class FreeFlyMotion : public Motion
    {
      public:
        FreeFlyMotion();

        void ApplyTo(Object3D &obj) override;
        void Reset() override;

        //void SetPosition(const glm::vec3 &pos);
        void Forward(float aAmount) override;
        void Right(float aAmount) override;
        void Up(float aAmount) override;
        
        void RotatePitch(float aAngle);
        void RotateYaw(float aAngle);
        void RotateRoll(float aAngle);

      private:
        glm::vec4 mForward;
        glm::vec4 mUp;
        glm::vec4 mRight;
        float     mYaw, mPitch, mRoll;
        float     mForwardAmount, mRightAmount, mUpAmount;
        bool      mRecalculate;
    };
}
