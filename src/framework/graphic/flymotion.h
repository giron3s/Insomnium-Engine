/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief	    : Motion for fly movement. This motion uses yaw and pitch and
 *                the rotations are always aligned to the world axes, not the
 *                object own axes.
 *
 *                Typical motion for a 3D modeling software fly camera, where the
 *                user can freely navigate the world but the floor is always down
*******************************************************************************/
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "graphic/motion.h"

namespace Framework
{

    class FlyMotion : public Motion
    {
      public:
        /**
         * Constructor
         */
        FlyMotion();

        /**
         * Inherited from Motion class
         */
        void ApplyTo(Object3D &obj) override;

        void ApplyTo(std::shared_ptr<TransformCmp> aTransformCmp) override;

        void Reset() override;

        /**
         * Moves the position of the motion along the forward vector
         *
         * The forward vector is the (0, 0, -1) vector in local coordinates
         *
         * @param amount  Amount of movement along the forward vector. It can
         *                be positive or negative, in which case it moves backwards
         */
        void Forward(float amount) override;

        /**
         * Moves the position of the motion along the right vector
         *
         * The right vector is the (1, 0, 0) vector in local coordinates
         *
         * @param amount  Amount of movement along the right vector. It can
         *                be positive or negative, in which case it moves left
         */
        void Right(float amount) override;
        void Up(float amount) override;

        /**
         * Rotates the motion along the right vector
         *
         * @see forward
         *
         * @param angle  Angle in degrees for the rotation around the right vector
         */
        void RotatePitch(float angle);

        /**
         * Rotates the motion along the up vector
         *
         * The up vector is the (0, 1, 0) vector in local coordinates
         *
         * @param angle  Angle in degrees for the rotation around the up vector
         */
        void RotateYaw(float angle);

        inline float GetPitch() const { return mPitch; }
        inline float GetYaw() const { return mYaw; }

      private:
        glm::vec4 mForward;       /**< Forward vector in local coordinates */
        glm::vec4 mUp;            /**< Up vector in local coordinates */
        glm::vec4 mRight;         /**< Right vector in local coordinates */
        float     mYaw;           /**< Current yaw angle in degrees */
        float     mPitch;         /**< Current pitch angle in degrees */
        float     mForwardAmount; /**< Current accumulated forward amount. This is reset after reset() or applyTo() are called */
        float     mRightAmount;   /**< Current accumulated right amount. This is reset after reset() or applyTo() are called */
    };
}
