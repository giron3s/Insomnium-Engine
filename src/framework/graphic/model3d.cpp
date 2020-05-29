/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohiblItered
 *                Proprietary and confidential
 *
 * Brief        : Basic model 3D representation
 *
********************************************************************************/

#include "precompiled.h"
#include "graphic/model3d.h"

namespace Framework
{

    bool Model3D::ConstrainPosition(const BoundingBox& aParentAABB)
    {
        if ( mFlags & Model::ModelFlags::eDISABLE_TRASFORM_3D )
            return false;
        const auto& lModelAABB = GetAABB();
        ASSERT(mMatrixState == MatrixState::Model);
       
        if ( lModelAABB.GetMin().x < aParentAABB.GetMin().x )
        {
            mPosition.x += (aParentAABB.GetMin().x - lModelAABB.GetMin().x);
            mMatrixState = MatrixState::NotUpdated;
        }
        else if ( lModelAABB.GetMax().x > aParentAABB.GetMax().x )
        {
            mPosition.x -= (lModelAABB.GetMax().x - aParentAABB.GetMax().x);
            mMatrixState = MatrixState::NotUpdated;
        }
        
        if ( lModelAABB.GetMin().y < aParentAABB.GetMin().y )
        {
            mPosition.y += (aParentAABB.GetMin().y - lModelAABB.GetMin().y);
            mMatrixState = MatrixState::NotUpdated;
        }
        else if ( lModelAABB.GetMax().y > aParentAABB.GetMax().y )
        {
            mPosition.y -= (lModelAABB.GetMax().y - aParentAABB.GetMax().y);
            mMatrixState = MatrixState::NotUpdated;
        }
        
        if ( lModelAABB.GetMin().z < aParentAABB.GetMin().z )
        {
            mPosition.z += (aParentAABB.GetMin().z - lModelAABB.GetMin().z);
            mMatrixState = MatrixState::NotUpdated;
        }
        else if ( lModelAABB.GetMax().z > aParentAABB.GetMax().z )
        {
            mPosition.z -= (lModelAABB.GetMax().z - aParentAABB.GetMax().z);
            mMatrixState = MatrixState::NotUpdated;
        }
        return mMatrixState == MatrixState::NotUpdated;
    }


    void Model3D::CalculateBoundingVolumes() const
    {

        /* Loop through all the vertices and find the maximum radius and the
         * axis-aligned bounding box. At this moment the object-oriented bounding box
         * will be the same as the axis-aligned bound box. Once calculated the user can
         * call updateBoundingVolumes() to update this values accordingly using the model's
         * rotation.
         *
         * The loop assumes the center of mass of the object is (0.0f, 0.0f, 0.0f) in local
         * coordinates. If lIter is not the user must call normalize() prior to this function
         */
        mOOBB = mAsset->GetBoundingBox();
        mMaxLengthVertex = mAsset->GetMaxLengthVertex();
        UpdateBoundingVolumes();
    }
}
