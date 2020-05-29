/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*  Brief       : SpotLight Component
*******************************************************************************/

#pragma once

#include "engine/components/object3dcmp.h"


namespace Framework
{
    class SpotLight;

    class SpotLightCmp : public Object3DCmp
    {

    public:
        void Serialize(Json::Value& aSerializer) const override;
        void Deserialize(const Json::Value& aSerializer) override;

        Object3D*  GetObject3D() const override;
        
        SpotLight* GetSpotLight() const { return mSpotLight.get(); }

    protected:
        std::unique_ptr<SpotLight> mSpotLight;
    };
}
