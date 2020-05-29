/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 * Brief	    : Motion object. Applies certain type of motion to an Object3D
 *******************************************************************************/
#pragma once

#include "graphic/object3d.h"

namespace Framework
{
   const std::string MOTION_WALKING = "walking";
   const std::string MOTION_FREEFLY = "freefly";
   const std::string MOTION_FLY = "fly";
   const std::string MOTION_PAN_AND_ZOOM = "pan&zoom";

   class TransformCmp;

   class Motion
    {
      protected:
        glm::vec3 mPosition; // current position
      
      public:
         /**
         * Sets the position of the motion
         *
         *  The position will be directly transferred to the given
         *  Object3D when using ApplyTo()
         *
         *  @param pos  The new position for the motion
         */
        void SetPosition(const glm::vec3 &aPos){ mPosition = aPos; }

        inline const glm::vec3& GetPosition() const
        { return mPosition; }

        virtual void ApplyTo(Object3D &aObj) = 0;
        virtual void ApplyTo(std::shared_ptr<TransformCmp> aTransformCmp) = 0;
        virtual void Reset() = 0;
        virtual void Forward(float amount) = 0;
        virtual void Right(float amount) = 0;
        virtual void Up(float amount) = 0;

    };
}
