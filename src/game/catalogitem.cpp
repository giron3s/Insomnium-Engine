/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "catalogitem.h"
#include "cataloggroup.h"

#include "engine.h"
#include "json/json.h"

using namespace Framework;
using namespace Framework::Procedural;

namespace Game
{

    CatalogItem::CatalogItem()
        : mId("")
        , mName("")
        , mGroup(CatalogGroup::eINVALID)
        , mPrefabFile("")
    {

    }


    CatalogItem::CatalogItem(string aId, string aName, CatalogGroup aGroup, int aIcon, std::string aPrefabFile)
        : mId(aId)
        , mName(aName)
        , mGroup(aGroup)
        , mIcon(aIcon)
        , mPrefabFile(aPrefabFile)
    {

    }

    void CatalogItem::Serialize(Json::Value& aSerialize) const
    {

    }

    void CatalogItem::Deserialize(const Json::Value& aSerializer)
    {
        mId = aSerializer.get("id", mId).asString();
        mName = aSerializer.get("name", mName).asString();
        mPrefabFile = aSerializer["prefabfile"].asString();
        string lGroupStr = aSerializer.get("group", CatalogGroupToString(mGroup)).asString();
        mGroup = StringToCatalogGroup(lGroupStr.c_str());

        const string lImageName = aSerializer["icon"].asString();
        mIcon = Engine::Instance()->ResourceManager().FindImage(lImageName);
        if (mIcon == -1)
        {
            CRASH("Error ocurred during parsing '%s' catalog item!", mId.c_str())
        }
    }
}
