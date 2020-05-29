/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*  Brief       : DirectLight Component
*******************************************************************************/

#pragma once

#include "engine/components/object3dcmp.h"


namespace Framework
{
    class DirectLight;

    class DirectLightCmp : public Object3DCmp
    {

    public:
        void Serialize(Json::Value& aSerializer) const override;
        void Deserialize(const Json::Value& aSerializer) override;

        Object3D*    GetObject3D() const override;
        
        DirectLight* GetDirectLight() const { return mDirectLight.get(); }

    protected:
        std::unique_ptr<DirectLight> mDirectLight;
    };
}
