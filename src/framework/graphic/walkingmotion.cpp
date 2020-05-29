/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
********************************************************************************/

#include "precompiled.h"
#include "graphic/walkingmotion.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Framework
{
    WalkingMotion::WalkingMotion()
        : mForward(0.0f, 0.0f, -1.0f, 0.0f)
        , mUp(0.0f, 1.0f, 0.0f, 0.0f)
        , mRight(1.0f, 0.0f, 0.0f, 0.0f)
        , mYaw(0.0f)
        , mPitch(0.0f)
        , mForwardAmount(0.0f)
        , mRightAmount(0.0f)
        , mPosition(0.0f, 0.0f, 0.0f)
    {
    }

    void WalkingMotion::ApplyTo(Object3D &aObj)
    {
        glm::quat lQuatYaw = glm::angleAxis(mYaw, glm::vec3(mUp));
        glm::quat lQuatPitch = glm::angleAxis(mPitch, glm::vec3(mRight));

        glm::mat4 lOrientation = glm::toMat4(lQuatPitch) * glm::toMat4(lQuatYaw);

        glm::vec4 lForward = mForward * lOrientation;
        glm::vec4 lRight = mRight * lOrientation;

        lForward.y = 0.0f;
        lRight.y = 0.0f;

        mPosition += glm::vec3(mForwardAmount * lForward + mRightAmount * lRight);

        aObj.SetOrientation(lOrientation);
        aObj.SetPosition(mPosition);

        mForwardAmount = mRightAmount = 0.0f;
    }

    void WalkingMotion::Reset()
    {
        mForwardAmount = 0.0;
        mRightAmount = 0.0;
        mYaw = 0.0;
        mPitch = 0.0;
    }

    void WalkingMotion::SetPosition(const glm::vec3 &aPosition) { mPosition = aPosition; }
    void WalkingMotion::Forward(float aAmount) { mForwardAmount += aAmount; }
    void WalkingMotion::Right(float aAmount) { mRightAmount += aAmount; }
    void WalkingMotion::RotatePitch(float aAngle) { mPitch = RestrictAngle(mPitch + aAngle); }
    void WalkingMotion::RotateYaw(float aAngle) { mYaw = RestrictAngle(mYaw + aAngle); }
}
