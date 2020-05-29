/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*  Brief       : SpotLight Component
*******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "spotlightcmp.h"
#include "graphic/spotlight.h"

namespace Framework
{
    void SpotLightCmp::Serialize(Json::Value& aSerializer) const
    {
        mSpotLight->Serialize(aSerializer);
        aSerializer["attenuation"] = mSpotLight->GetAttenuation();
        aSerializer["cutoff"] = mSpotLight->GetCutoff();
        aSerializer["cone_angle"] = mSpotLight->GetConeAngle();
        aSerializer["cone_penumbra"] = mSpotLight->GetConePenumbra();
    }

    void SpotLightCmp::Deserialize(const Json::Value& aSerializer)
    {
        float lAttenuation = 1.0f;
        float lCutoff = 1000.0f;
        float lConeAngle = 45.0f;
        float lConePenumbra = 3.0f;
        lAttenuation = aSerializer.get("attenuation", lAttenuation).asFloat();
        lCutoff = aSerializer.get("cutoff", lCutoff).asFloat();
        lConeAngle = aSerializer.get("cone_angle", lConeAngle).asFloat();
        lConePenumbra = aSerializer.get("cone_penumbra", lConePenumbra).asFloat();

        mSpotLight = std::make_unique<SpotLight>();
        mSpotLight->Deserialize(aSerializer);
        mSpotLight->SetAttenuation(lAttenuation);
        mSpotLight->SetCutoff(lCutoff);
        mSpotLight->SetConeAngle(lConeAngle);
        mSpotLight->SetConePenumbra(lConePenumbra);

        Entity* lEntity = GetOwner();
        size_t lEntityID = lEntity->GetEntityID();
        mSpotLight->SetEntityID(lEntityID);
    }

    Object3D* SpotLightCmp::GetObject3D() const
    { return mSpotLight.get(); }
}
