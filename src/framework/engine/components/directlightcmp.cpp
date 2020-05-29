/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*  Brief       : Directlight Component
*******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "directlightcmp.h"
#include "graphic/directlight.h"

namespace Framework
{
    void DirectLightCmp::Serialize(Json::Value& aSerializer) const
    {
        mDirectLight->Serialize(aSerializer);
    }

    void DirectLightCmp::Deserialize(const Json::Value& aSerializer)
    {
        mDirectLight = std::make_unique<DirectLight>();
        mDirectLight->Deserialize(aSerializer);

        Entity* lEntity = GetOwner();
        size_t lEntityID = lEntity->GetEntityID();
        mDirectLight->SetEntityID(lEntityID);
    }

    Object3D* DirectLightCmp::GetObject3D() const
    { return mDirectLight.get(); }
}
