/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 * Brief        : FreeFlyMotion object
 ********************************************************************************/
#include "precompiled.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "graphic/freeflymotion.h"

namespace Framework
{
    FreeFlyMotion::FreeFlyMotion()
        : mForward(0.0f, 0.0f, -1.0f, 0.0f)
        , mUp(0.0f, 1.0f, 0.0f, 0.0f)
        , mRight(1.0f, 0.0f, 0.0f, 0.0f)
        , mYaw(0.0f)
        , mPitch(0.0f)
        , mRoll(0.0f)
        , mForwardAmount(0.0f)
        , mRightAmount(0.0f)
        , mUpAmount(0.0f)
        , mRecalculate(false)
    {
    }

    void FreeFlyMotion::ApplyTo(Object3D &aObj)
    {
        if (mRecalculate) {
            glm::quat qyaw = glm::angleAxis(mYaw, glm::vec3(mUp));
            glm::quat qpitch = glm::angleAxis(mPitch, glm::vec3(mRight));
            glm::quat qroll = glm::angleAxis(mRoll, glm::vec3(mForward));

            glm::mat4 lOrientation = glm::toMat4(qpitch) * glm::toMat4(qyaw) * glm::toMat4(qroll) * aObj.GetOrientation();

            glm::vec4 forward = mForward * lOrientation;
            glm::vec4 right = mRight * lOrientation;
            glm::vec4 up = mUp * lOrientation;

            mPosition += glm::vec3(mForwardAmount * forward + mRightAmount * right + mUpAmount * up);

            aObj.SetOrientation(lOrientation);
            aObj.SetPosition(mPosition);

            mForwardAmount = mRightAmount = mUpAmount = 0.0f;
            mYaw = mPitch = mRoll = 0.0;
        }
    }

    void FreeFlyMotion::Reset()
    {
        mForwardAmount = 0.0f;
        mRightAmount = 0.0f;
        mUpAmount = 0.0f;
        mYaw = 0.0f;
        mPitch = 0.0f;
        mRoll = 0.0f;
        mRecalculate = true;
    }

    /*void FreeFlyMotion::SetPosition(const glm::vec3 &pos)
    {
        mPosition = pos;
        mRecalculate = true;
    }*/

    void FreeFlyMotion::Forward(float amount)
    {
        mForwardAmount += amount;
        mRecalculate = true;
    }

    void FreeFlyMotion::Right(float amount)
    {
        mRightAmount += amount;
        mRecalculate = true;
    }

    void FreeFlyMotion::Up(float amount)
    {
        mUpAmount += amount;
        mRecalculate = true;
    }

    void FreeFlyMotion::RotatePitch(float angle)
    {
        mPitch = RestrictAngle(mPitch + angle);
        mRecalculate = true;
    }

    void FreeFlyMotion::RotateYaw(float angle)
    {
        mYaw = RestrictAngle(mYaw + angle);
        mRecalculate = true;
    }

    void FreeFlyMotion::RotateRoll(float angle)
    {
        mRoll = RestrictAngle(mRoll + angle);
        mRecalculate = true;
    }
}
