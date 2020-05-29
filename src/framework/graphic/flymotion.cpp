/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *   Brief      : FlyMotion object
 *******************************************************************************/

#include "precompiled.h"
#include "graphic/flymotion.h"
#include "engine/components/transformcmp.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Framework
{
    FlyMotion::FlyMotion()
        : mForward(0.0f, 0.0f, -1.0f, 0.0f)
        , mUp(0.0f, 1.0f, 0.0f, 0.0f)
        , mRight(1.0f, 0.0f, 0.0f, 0.0f)
        , mYaw(0.0f)
        , mPitch(0.0f)
        , mForwardAmount(0.0f)
        , mRightAmount(0.0f)
    {}

    void FlyMotion::ApplyTo(Object3D &aObj)
    {
        glm::quat lQuatYaw = glm::angleAxis(glm::radians(mYaw), glm::vec3(mUp));
        glm::quat lQuatPitch = glm::angleAxis(glm::radians(mPitch), glm::vec3(mRight));

        glm::mat4 lOrientation = glm::toMat4(lQuatPitch) * glm::toMat4(lQuatYaw);

        glm::vec4 lForward = mForward * lOrientation;
        glm::vec4 lRight = mRight * lOrientation;

        mPosition += glm::vec3(mForwardAmount * lForward + mRightAmount * lRight);

        aObj.SetOrientation(lOrientation);
        aObj.SetPosition(mPosition);

        mForwardAmount = mRightAmount = 0.0f;

        //INFO(LogLevel::eLEVEL2, "[Position] x:%f, y:%f, z:%f [Yaw]:%f, [Pitch]:%f", mPosition.x, mPosition.y, mPosition.z, mYaw, mPitch);
    }

    void FlyMotion::ApplyTo(std::shared_ptr<TransformCmp> aTransformCmp)
    {
       glm::quat lQuatYaw = glm::angleAxis(glm::radians(mYaw), glm::vec3(mUp));
       glm::quat lQuatPitch = glm::angleAxis(glm::radians(mPitch), glm::vec3(mRight));

       glm::quat lOrientation = lQuatPitch * lQuatYaw;

       glm::vec4 lForward = mForward * lOrientation;
       glm::vec4 lRight = mRight * lOrientation;

       mPosition += glm::vec3(mForwardAmount * lForward + mRightAmount * lRight);

       aTransformCmp->SetOrientation(lOrientation);
       aTransformCmp->SetPosition(mPosition);

       mForwardAmount = mRightAmount = 0.0f;

       //INFO(LogLevel::eLEVEL2, "[Position] x:%f, y:%f, z:%f [Yaw]:%f, [Pitch]:%f", mPosition.x, mPosition.y, mPosition.z, mYaw, mPitch);
    }

    void FlyMotion::Reset()
    {
        mForwardAmount = 0.0;
        mRightAmount = 0.0;
        mYaw = 0.0;
        mPitch = 0.0;
    }

    void FlyMotion::Forward(float aAmount)
    { mForwardAmount += aAmount; }
    
    void FlyMotion::Right(float aAmount)
    { mRightAmount += aAmount; }
    
    void FlyMotion::Up(float aAmount)
    { mPosition += glm::vec3(mUp.x, mUp.y, mUp.z) * aAmount; }
    
    void FlyMotion::RotatePitch(float aAngle)
    { mPitch = RestrictAngle(mPitch + aAngle); }
    
    void FlyMotion::RotateYaw(float aAngle)
    { mYaw = RestrictAngle(mYaw + aAngle); }
}
