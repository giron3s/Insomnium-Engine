/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Camera Component
 *******************************************************************************/

#pragma once

#include "engine/components/object3dcmp.h"

namespace Framework
{
    class Camera;
}

namespace Framework
{
    class CameraCmp : public Object3DCmp
    {

    public:
                     CameraCmp();
                     ~CameraCmp();


        Object3D*    GetObject3D() const override;
        Camera*      GetCamera() const { return mCamera; }

        virtual void Serialize(Json::Value& aSerializer) const override;
        virtual void Deserialize(const Json::Value& aSerializer) override;

    protected:
        Camera*      mCamera;
    };


}

