/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Camera Component
 *******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "engine/components/cameracmp.h"
#include "graphic/camera.h"

namespace Framework
{
    CameraCmp::CameraCmp()
        : mCamera(nullptr)
    {
    }

    CameraCmp::~CameraCmp()
    {
        delete mCamera;
    }


    void CameraCmp::Serialize(Json::Value& aSerializer) const
    {
        aSerializer["projection_type"] = mCamera->GetProjectionType() == Projection::PERSPECTIVE ? "perspective" : "orthographic";
        glm::vec2 z_range(mCamera->GetZNear(), mCamera->GetZFar());
        SerializeVec<glm::vec2>(aSerializer["z_range"], z_range);
        if ( mCamera->GetProjectionType() == Projection::ORTHOGRAPHIC )
            aSerializer["near_height"] = mCamera->GetParam();
        else if ( mCamera->GetProjectionType() == Projection::PERSPECTIVE )
            aSerializer["fov"] = mCamera->GetParam();
    }

    void CameraCmp::Deserialize(const Json::Value& aSerializer)
    {
        Entity* lEntity = GetOwner();
        size_t lEntityID = lEntity->GetEntityID();
        if( !aSerializer.isMember("projection_type") )
            CRASH("Camera 'projection_type' attribute is not defined");

        uint32_t lProjectionType = 0;
        string lTypeStr = aSerializer.get("projection_type", "").asString();
        if ( strcmp(lTypeStr.c_str(), "perspective") == 0 )
            lProjectionType = Projection::PERSPECTIVE;
        else if ( strcmp(lTypeStr.c_str(), "orthographic") == 0 )
            lProjectionType = Projection::ORTHOGRAPHIC;
        else
            CRASH("Unknown Camera projection type");

        mCamera = new Camera(lProjectionType, lEntityID);
        mCamera->SetName(string("camera_") + lTypeStr);
        if( aSerializer.isMember("z_range") )
        {
            glm::vec2 z_range(0,0);
            DeserializeVec<glm::vec2>(aSerializer["z_range"], z_range);
            mCamera->SetZRange(z_range.x, z_range.y);
        }
        if ( lProjectionType = Projection::PERSPECTIVE &&
             aSerializer.isMember("fov") )
            mCamera->SetPerspectiveParam(aSerializer["fov"].asFloat());
        if ( lProjectionType = Projection::ORTHOGRAPHIC &&
             aSerializer.isMember("near_height") )
            mCamera->SetOrthographicParam(aSerializer["near_height"].asFloat());
    }

    Object3D* CameraCmp::GetObject3D() const
    {
       return mCamera;
    }
}
