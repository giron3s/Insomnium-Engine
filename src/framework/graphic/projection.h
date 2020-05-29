/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*  Brief       : Orthogonal or perspective projection for any object. This can be typically
*                inherited by cameras to add projection functionality to the camera, or to
*                lights to calculate a shadow map
*******************************************************************************/
#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"
#include "graphic/object3d.h"

constexpr float DEG_TO_RADf = 0.0174533f;

namespace Framework
{
    class Projection
    {
    public:
        enum
        {
            PERSPECTIVE = 1,
            ORTHOGRAPHIC = 2
        }; // projection type constants


        Projection(uint32_t aType = PERSPECTIVE)
            : mProjectionType(aType)
        {

            switch ( mProjectionType )
            {
                case PERSPECTIVE:  mFOV = 45.0f;  break;
                case ORTHOGRAPHIC: mNearHeight = 1.0f; break;
                default: ASSERT(0);
            }
        }

        ~Projection() {}

        /* Common */
        void SetAspect(float aAspect)
        {
            ASSERT(aAspect > 0.0f);
            mAspect = aAspect;
            mValid = false;
        }

        void SetZRange(float aZNear, float aZFar)
        {
            ASSERT(aZNear > 0.0 && aZFar > aZNear);
            mZNear = aZNear;
            mZFar = aZFar;
            mValid = false;
        }

        /* Projection type - specific */
        void SetOrthographicParam(float aNearHeight)
        {
            ASSERT(mProjectionType == ORTHOGRAPHIC);
            ASSERT(aNearHeight > 0.0);
            mNearHeight = aNearHeight;
            mValid = false;
        }

        void SetPerspectiveParam(float aFOV)
        {
            ASSERT(mProjectionType == PERSPECTIVE);
            ASSERT(0.0f < aFOV && aFOV < 180.0f);
            mFOV = aFOV;
            mValid = false;
        }

        /**
         * Getters for the projections fields
         */
        uint32_t GetProjectionType() const { return mProjectionType; }
        float GetAspect() const { return mAspect; }
        float GetZNear() const { return mZNear; }
        float GetZFar() const { return mZFar; }
		// return 'near height' for orthographic projection or 'fov_y' for perspective projection:
		float GetParam() const { return mNearHeight; }
        /**
         * Retrieves perspective/orthographic projection matrix
         *
         * It is important to note that the projection class assumes only one
         * projection type will be used along the life of the projection object,
         * so the mValid attribute is used for both orthogonal and perspective
         * projections. To switch to other projection type the forceRecalculate
         * parameter can be set to true to retrieve the new matrix
         *
         * @param forceRecalculate  Forces recalculation of the projection matrix
         *                          regardless of the internal cache
         *
         * @return The perspective/orthographic projection matrix
         */
        const glm::mat4& GetProjectionMatrix(bool aForceRecalculate = false) const
        {
            if ( !mValid || aForceRecalculate )
            {
                switch ( mProjectionType )
                {
                    case PERSPECTIVE:
                    const_cast<glm::mat4&>(mProjectionMatrix) = glm::perspective(mFOV * DEG_TO_RADf, mAspect, mZNear, mZFar);
                    break;
                    case ORTHOGRAPHIC: {
                    float lNearWidth = mNearHeight * mAspect;
                    const_cast<glm::mat4&>(mProjectionMatrix) = glm::ortho(-lNearWidth / 2.0f, lNearWidth / 2.0f, -mNearHeight / 2.0f, mNearHeight / 2.0f, mZNear, mZFar);
                    break; }
                    default:
                    ASSERT(0);
                }
                const_cast<bool&>(mValid) = true;
            }
            return mProjectionMatrix;
        }

    protected:
        uint32_t  mProjectionType = PERSPECTIVE;
        float mAspect = 1.0f, // width/height
              mZNear = 0.1f, mZFar = 100.0f; /* Common for perspective and orthographic projections */
        union
        {
            float mFOV;        /* For perspective projection: field of view in y direction */
            float mNearHeight; /* For orthographic projection */
        };
        
        glm::mat4 mProjectionMatrix; /* Cached projection matrix */
        bool mValid = false; /* Flag to indicate if a cached matrix is available */
    };
}
