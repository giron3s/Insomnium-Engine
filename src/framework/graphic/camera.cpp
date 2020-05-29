/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *
 * Brief	    : Camera object. It can be used to project objects onto it through the
 *                Projection interface, allowing for perspective and orthogonal projections. It
 *                inherits from Object3D to allow movement of the camera through interpolators
 *                or Movement objects
*******************************************************************************/

#include "precompiled.h"
#include "graphic/camera.h"


namespace Framework
{
    void Camera::RecalculateProjectionVolume()
    {
        if ( mProjectionType == Projection::ORTHOGRAPHIC )
        {
            // TODO...
            return;
        }
        glm::mat4 lMVP = GetProjectionMatrix() * GetViewMatrix();

    #if 0 /* Left here to understand how the planes are calculated \
             in the loop below */
        float planesTerms[MAX_PLANES][4] = {
            {lMVP[0][0] + lMVP[0][3], lMVP[1][0] + lMVP[1][3], lMVP[2][0] + lMVP[2][3], lMVP[3][0] + lMVP[3][3]},     /* Left plane */
            {-lMVP[0][0] + lMVP[0][3], -lMVP[1][0] + lMVP[1][3], -lMVP[2][0] + lMVP[2][3], -lMVP[3][0] + lMVP[3][3]}, /* Right plane */
            {lMVP[0][1] + lMVP[0][3], lMVP[1][1] + lMVP[1][3], lMVP[2][1] + lMVP[2][3], lMVP[3][1] + lMVP[3][3]},     /* Bottom plane */
            {-lMVP[0][1] + lMVP[0][3], -lMVP[1][1] + lMVP[1][3], -lMVP[2][1] + lMVP[2][3], -lMVP[3][1] + lMVP[3][3]}, /* Top plane */
            {lMVP[0][2] + lMVP[0][3], lMVP[1][2] + lMVP[1][3], lMVP[2][2] + lMVP[2][3], lMVP[3][2] + lMVP[3][3]},     /* Near plane */
            {-lMVP[0][2] + lMVP[0][3], -lMVP[1][2] + lMVP[1][3], -lMVP[2][2] + lMVP[2][3], -lMVP[3][2] + lMVP[3][3]}, /* Far plane */
        };
    #endif

        for (int i = 0; i < MAX_PLANES; ++i) {
            float sign = (i % 2) ? -1.0f : 1.0f;

            glm::vec3 normal = glm::vec3(sign * lMVP[0][i / 2] + lMVP[0][3], sign * lMVP[1][i / 2] + lMVP[1][3], sign * lMVP[2][i / 2] + lMVP[2][3]);

            mProjectionVolumePlanes[i] = glm::vec4(normal, sign * lMVP[3][i / 2] + lMVP[3][3]) / glm::length(normal);
        }
    }

    bool Camera::IsObjectVisible(Object3D &aObject)
    {
        float lRadius = aObject.GetBoundingSphere().GetRadius();

        for (int i = 0; i < MAX_PLANES; ++i)
        {
            /* Check the sphere */
            if (glm::dot(mProjectionVolumePlanes[i], glm::vec4(aObject.GetPosition(), 1.0f)) < -lRadius) {
                return false;
            }
        }
        return true;
    }


    bool Camera::IsObjectVisible(Object2D &aObject)
    {
        WARNING("TODO mgironesd implement it");
        return true;
    }
}

