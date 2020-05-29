/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/
#pragma once

#include <string.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

#include "core/logger.h"


namespace Framework
{
    class Object
    {
      public:

        /**
         * Constructor
         */
        Object(size_t aEntityID)
            : mPosition(0.0f, 0.0f, 0.0f)
            , mOrientation(glm::mat4(1.0f))
            , mScale(glm::vec3(1.0))
            , mMatrix(glm::mat4(1.0f))
            , mMatrixState(MatrixState::NotUpdated)
            , mEnabled(true)
            , mEntityID(aEntityID)
        {}

        /**
         * Destructor (virtual)
         */
        virtual ~Object()
        {}

        /**
         * Get object name
         */
        inline const std::string& GetName() const { return mName; }


        /**
         * Set object name
         */
        void SetName(const std::string& aName)
        { 
            // preserve procedural object names
            if ( !mName.empty() && mName.front() == '#' )
            {
                ASSERT(0);
                return;
            }
            if ( !aName.empty() && aName.front() == '#' )
            {
                ASSERT(0);
                SetName("");
                return;
            }
            // generate a name if aName is empty
            if ( aName.empty() )
            {
                static uint32_t sID = 1;
                mName.assign("Object_").append(std::to_string(sID));
                ++sID;
            }
            else
                mName = aName;
        }

        /**
         * Moves the postition of the 3D object according to the given amount for each axis
         *
         * Forces recalculation of the model and view matrices
         *
         * @param amount  Amount of movement on each axis as a vec3
         */
        void Move(const glm::vec3 &aAmount)
        {
            mPosition += aAmount;
            mMatrixState = MatrixState::NotUpdated;
        }

        /**
         * Rotates the current orientation according to the given rotation as a quaternion
         *
         * Forces recalculation of the model and view matrices
         *
         * @param rotation  Rotation to apply to the current orientation as a mat4 quaternion
         */
        void Rotate(const glm::mat4 &aRotation)
        {
            mOrientation = aRotation * mOrientation;
            for (int i = 0; i < 3; ++i)
            {
                glm::vec4& v = mOrientation[i]; v.w = 0.0;
                v = glm::normalize(v);
            }
            ASSERT( mOrientation[3] == glm::vec4(0,0,0,1) );
            mMatrixState = MatrixState::NotUpdated;
        }

        /**
         * Multiplies the current scale factor by the given scale factor and sets
         * the result as the new scale factor
         *
         * Forces recalculation of the model and view matrices
         *
         * @param factor  Scale factor to apply to the 3D object current scale factor
         */
        void Scale(const glm::vec3 &aFactor)
        {
            mScale *= aFactor;
            mMatrixState = MatrixState::NotUpdated;
        }

        /**
         * Recalculates the view and model matrices so the object's forward vector
         * goes from the object's position to the given 'at' position
         *
         * Forces recalculation of the model and view matrices
         *
         * @param at  Position in space where the forward vector will point to
         */
        void LookAt(const glm::vec3 &aAT) const
        {
            glm::vec3 up(0.0f, 1.0f, 0.0f);

            // TODO: possible problem with lights not rendering shadow map correctly
            // when view vector is aligned with up vector
            mMatrix = glm::lookAt(mPosition, aAT, up);

            mMatrixState = MatrixState::NotUpdated;
        }

        /**
         * Setters
         *
         * All setters force recalculation of the model and view matrices
         */
        void SetPosition(const glm::vec3 &aPosition)
        {
            mPosition = aPosition;
            mMatrixState = MatrixState::NotUpdated;
        }

        void SetOrientation(const glm::mat4 &aOrientation)
        {
            mOrientation = aOrientation;
            for (int i = 0; i < 3; ++i)
            {
                glm::vec4& v = mOrientation[i]; v.w = 0.0;
                v = glm::normalize(v);
            }
            mMatrixState = MatrixState::NotUpdated;
        }

        void SetScaleFactor(const glm::vec3 &aFactor)
        {
            mScale = aFactor;
            mMatrixState = MatrixState::NotUpdated;

            if (mScale.x < 1.0e-3f)
                mScale.x = 1.0e-3f;

            if (mScale.y < 1.0e-3f)
                mScale.y = 1.0e-3f;

            if (mScale.z < 1.0e-3f)
                mScale.z = 1.0e-3f;
        }

        /**
         * Getters
         */
        const glm::vec3 &GetPosition() const { return mPosition; }
        const glm::mat4 &GetOrientation() const { return mOrientation; }
        const glm::vec3 &GetScaleFactor() const { return mScale; }

        /**
         * Returns the current direction of the model, which is the forward vector
         *
         * @return A vec3 with the forward vector
         */
        glm::vec3 GetDirection() const
        {
            return -glm::vec3(glm::row(GetModelMatrix(), 2));
        }

        /**
         * Returns model matrix of the object
         *
         * The matrix is cached and only recalculated if any of the setters or model
         * matrix modifiers are called
         *
         * @return A mat4 with the current model matrix
         */
        const glm::mat4& GetModelMatrix(void) const
        {
            if ( mMatrixState != MatrixState::Model )
            {
                mMatrix = glm::scale(glm::translate(glm::mat4(), mPosition) * mOrientation, mScale);
                mMatrixState = MatrixState::Model;
            }
            return mMatrix;
        }


        /**
         * Returns view matrix as if the object is a camera
         *
         * The matrix is cached and only recalculated if any of the setters or model
         * matrix modifiers are called
         *
         * @return A mat4 with the current model matrix
         */
        const glm::mat4& GetViewMatrix() const
        {
            if ( mMatrixState != MatrixState::View )
            {
                mMatrix = glm::translate(glm::scale(glm::mat4(), mScale) * mOrientation, -mPosition);
                mMatrixState = MatrixState::View;
            }
            return mMatrix;
        }

        /**
        * Returns the associated entity ID
        *
        * @return the entity ID
        */
        size_t GetEntityID() const { return mEntityID; }

         /**
         * Sets the entity ID for this object
         * This method is intended to be used by RenderableCmp when creating an instance/deserializing
         * @param aEntityID  entity ID
         */
         void SetEntityID(size_t aEntityID) { mEntityID = aEntityID; }

        /**
         * Enables/disables this object in the pipeline
         *
         * When disabled the object is not taken into account during the rendering
         * process
         */
        void Enable() { mEnabled = true; }
        void Disable() { mEnabled = false; }
        bool IsEnabled() const { return mEnabled; }

      protected:

        // note: all cache-related members must be declared as mutable
        glm::vec3         mPosition;         /**< Position of the object in world coordinates */
        glm::mat4         mOrientation;      /**< Orientation of the object as a quaternion */
        glm::vec3         mScale;            /**< Scale factors for each axis XYZ */

        mutable glm::mat4 mMatrix;            /**< Model Or view matrix */
        enum class MatrixState { NotUpdated = 0, Model = 1, View = 2 };
        mutable MatrixState mMatrixState;       /**< If true, current matrix is cached and does not need recalculation */

        bool              mEnabled;          /**< Indicates if this object is taken into account in the pipeline */
        size_t            mEntityID;         /**< reference to the entity containing this object */

        std::string       mName;
    };
}

