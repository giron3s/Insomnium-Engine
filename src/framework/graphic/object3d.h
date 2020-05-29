/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/
#pragma once

#include "graphic/object.h"
#include "graphic/boundingbox.h"
#include "graphic/boundingsphere.h"


namespace Framework
{
    class Object2D;

    class Object3D : public Object
    {
      public:

        Object2D* mBuddy = nullptr;

        /**
         * Constructor
         */
        Object3D(size_t aEntityID)
            : Object(aEntityID)
            , mOOBBValid(false)
            , mBoundingVolumesValid(false)
            , mRenderBoundingSphere(false)
            , mRenderAABB(false)
            , mRenderOOBB(false)
        {}

        const BoundingSphere &GetBoundingSphere() const
        {
            if (mOOBBValid == false) {
                CalculateBoundingVolumes();
                mOOBBValid = true;
            }
            if (mBoundingVolumesValid == false || mMatrixState != MatrixState::Model) {
                UpdateBoundingVolumes();
                mBoundingVolumesValid = true;
            }
            return mBoundingSphere;
        }

        const BoundingBox &GetAABB() const
        {
            if (mOOBBValid == false) {
                CalculateBoundingVolumes();
                mOOBBValid = true;
            }
            if (mBoundingVolumesValid == false || mMatrixState != MatrixState::Model) {
                UpdateBoundingVolumes();
                mBoundingVolumesValid = true;
            }
            return mAABB;
        }

        const BoundingBox &GetOOBB() const
        {
            if (mOOBBValid == false) {
                CalculateBoundingVolumes();
                mOOBBValid = true;
            }
            if (mBoundingVolumesValid == false || mMatrixState != MatrixState::Model) {
                UpdateBoundingVolumes();
                mBoundingVolumesValid = true;
            }
            return mOOBB;
        }

        /**
         * Debug information
         */
        void SetRenderBoundingVolumes(bool flag)
        {
            mRenderBoundingSphere = flag;
            mRenderAABB = flag;
            mRenderOOBB = flag;
        }

        void SetRenderBoundingSphere(bool flag) { mRenderBoundingSphere = flag; }
        void SetRenderAABB(bool flag) { mRenderAABB = flag; }
        void SetRenderOOBB(bool flag) { mRenderOOBB = flag; }
        bool GetRenderBoundingSphere() const { return mRenderBoundingSphere; }
        bool GetRenderAABB() const { return mRenderAABB; }
        bool GetRenderOOBB() const { return mRenderOOBB; }


      protected:
        /**
         * Calculates the bounding volumes for a Model3D
         *
         * The bounding volumes include the BoundingSphere, the OOBB and the AABB.
         * This performs a full recalculation of all the volumes assuming no prior
         * information can be reused
         *
         * This method must be overriden in inheriting classes to calculate the
         * proper bounding volumes based on the inheriting class data
         */
        virtual void CalculateBoundingVolumes() const { /* empty */ }

        /**
         * Updates the bounding volumes of a model. In this case the OOBB
         * is used to speed up the calculation of the bounding sphere and the
         * AABB
         *
         * Fast calculation idea copied from
         *    http://zeuxcg.org/2010/10/17/aabb-from-obb-with-component-wise-abs/
         */
        void UpdateBoundingVolumes() const
        {
            glm::mat4 lModel = GetModelMatrix();
            glm::mat4 absModel(glm::abs(lModel[0][0]), glm::abs(lModel[0][1]), glm::abs(lModel[0][2]), glm::abs(lModel[0][3]),
                               glm::abs(lModel[1][0]), glm::abs(lModel[1][1]), glm::abs(lModel[1][2]), glm::abs(lModel[1][3]),
                               glm::abs(lModel[2][0]), glm::abs(lModel[2][1]), glm::abs(lModel[2][2]), glm::abs(lModel[2][3]),
                               glm::abs(lModel[3][0]), glm::abs(lModel[3][1]), glm::abs(lModel[3][2]), glm::abs(lModel[3][3]));

            glm::vec3 lCenter = (mOOBB.GetMin() + mOOBB.GetMax()) / 2.0f;
            glm::vec3 lExtent = (mOOBB.GetMax() - mOOBB.GetMin()) / 2.0f;

            glm::vec3 lNewCenter = glm::vec3(lModel * glm::vec4(lCenter, 1.0f));
            glm::vec3 lNewExtent = glm::vec3(absModel * glm::vec4(lExtent, 0.0f));

            glm::vec3 lMin = lNewCenter - lNewExtent;
            glm::vec3 lMax = lNewCenter + lNewExtent;

            mAABB.SetMin(lMin);
            mAABB.SetMax(lMax);

            mBoundingSphere.SetRadius(glm::length(mMaxLengthVertex * GetScaleFactor()));
        }

        mutable glm::vec3      mMaxLengthVertex;     /**< Maximum length among the vertices set, used to calculate mBoundingSphere */
        mutable BoundingSphere mBoundingSphere;      /**< Bounding sphere containing all model's vertices */
        mutable BoundingBox    mOOBB;                /**< Object-oriented bounding box containing all model's vertices */
        mutable BoundingBox    mAABB;                /**< Axis-aligned bounding box containing all model's vertices */
        mutable bool           mOOBBValid;           /**< Indicates if the cached information for the base OOBB is still valid */
        mutable bool           mBoundingVolumesValid;/**< Indicates if the cached information for the OOBB and AABB is still valid */

        bool           mRenderBoundingSphere;/**< Flag to enable model bounding sphere rendering */
        bool           mRenderAABB;          /**< Flag to enable model AABB rendering */
        bool           mRenderOOBB;          /**< Flag to enable model OOBB rendering */
    };
}

