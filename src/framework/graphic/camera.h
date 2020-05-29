/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief	    : Camera object. It can be used to project objects onto it through the
 *                Projection interface, allowing for perspective and orthogonal projections. It
 *                inherits from Object3D to allow movement of the camera through interpolators
 *                or Movement objects
 *******************************************************************************/

#pragma once

#include "graphic/object3d.h"
#include "graphic/projection.h"
#include "engine/components/entity.h"

namespace Framework
{
class Camera : public Projection, public Object3D
{
  public:
    /**
     * Constructor
     */
    Camera(uint32_t aProjectionType, size_t aEntityID = Entity::GetDefaultID())
        : Projection(aProjectionType)
        , Object3D(aEntityID)
    {}

    /**
     * Recalculates the projection volume planes for geometry culling
     */
    void RecalculateProjectionVolume(void);

    /**
     * Checks if the given object is visible from the camera
     *
     * It determines if any part of the object is inside the camera's frustum
     *
     * @param object  Object to check against the camera's frustum
     *
     * @return true if the object is visible, false otherwise
     */
    bool IsObjectVisible(Object3D &object);

    /**
     * Checks if the given object is visible from the camera
     *
     * It determines if any part of the object is inside the camera's frustum
     *
     * @param object  Object to check against the camera's frustum
     *
     * @return true if the object is visible, false otherwise
     */
    bool IsObjectVisible(Object2D &object);


  private:
    /**
     * Enumeration to access the frustum planes
     */
    enum { PLANE_LEFT = 0, PLANE_RIGHT, PLANE_BOTTOM, PLANE_TOP, PLANE_NEAR, PLANE_FAR, MAX_PLANES };

    glm::vec4 mProjectionVolumePlanes[MAX_PLANES];
};
}
