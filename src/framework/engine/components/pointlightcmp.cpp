/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*  Brief       : PointLight Component
*******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "pointlightcmp.h"
#include "graphic/pointlight.h"

namespace Framework
{
    void PointLightCmp::Serialize(Json::Value& aSerializer) const
    {
        mPointLight->Serialize(aSerializer);
        aSerializer["attenuation"] = mPointLight->GetAttenuation();
        aSerializer["cutoff"] = mPointLight->GetCutoff();
    }

    void PointLightCmp::Deserialize(const Json::Value& aSerializer)
    {
        float lAttenuation = 1.0f;
        float lCutoff = 1000.0f;
        lAttenuation = aSerializer.get("attenuation", lAttenuation).asFloat();
        lCutoff = aSerializer.get("cutoff", lCutoff).asFloat();

        mPointLight = std::make_unique<PointLight>();
        mPointLight->Deserialize(aSerializer);
        mPointLight->SetAttenuation(lAttenuation);
        mPointLight->SetCutoff(lCutoff);

        Entity* lEntity = GetOwner();
        size_t lEntityID = lEntity->GetEntityID();
        mPointLight->SetEntityID(lEntityID);
    }

    Object3D* PointLightCmp::GetObject3D() const
    { return mPointLight.get(); }

}
