/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Catalog Component
 *******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "engine/components/catalogcmp.h"


namespace Framework
{
    CatalogCmp::CatalogCmp()
        : mName("")
        , mGroup("")
        , mIcon("")
    {
       
    }

    CatalogCmp::~CatalogCmp()
    {
    }

    void CatalogCmp::Serialize(Json::Value& aSerializer) const
    {
        aSerializer["name"] = mName;
        aSerializer["group"] = mGroup;
        aSerializer["icon"] = mIcon;
    }

    void CatalogCmp::Deserialize(const Json::Value& aSerializer)
    {
        mName = aSerializer.get("name", mName).asString();
        mGroup = aSerializer.get("group", mGroup).asString();
        mIcon = aSerializer.get("icon", 0).asString();
    }
}
