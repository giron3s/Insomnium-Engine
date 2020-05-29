/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "core/config.h"

#include "catalogmanager.h"
#include "catalogitem.h"


using namespace Framework;

namespace Game
{
    CatalogManager::CatalogManager()
        : BaseManager(Utils::Demangling(typeid(CatalogManager).name()))
    {

    }

    CatalogManager::~CatalogManager()
    {
       
    }

    void CatalogManager::Initialize()
    {
        LoadCatalog(Engine::Instance()->Config().GetCatalogFile(string("roomshapes")));
        LoadCatalog(Engine::Instance()->Config().GetCatalogFile(string("furniture")));
    }

    void CatalogManager::DeInitialize()
    {
        ClearCatalog();
    }

    void CatalogManager::LoadCatalog(const string& aResourceFile)
    {
        Json::Value lRes;
        Json::JsonUtils::OpenAndParseJsonFromFile(lRes, aResourceFile);

        for (size_t i = 0, lSize = lRes.size(); i < lSize; ++i)
        {
            AddCatalogItem(lRes[static_cast<int>(i)]);
        }
    }

    vector<CatalogItem*> CatalogManager::GetCatalogItems()
    {
        return mCatalogItems;
    }

    vector<CatalogItem*> CatalogManager::GetCatalogItems(CatalogGroup aGroup)
    {
        vector<CatalogItem*> lResult;
        for (auto lCatalogItem : mCatalogItems)
        {
            if (lCatalogItem->GetGroup() == aGroup)
            {
                lResult.push_back(lCatalogItem);
            }
        }
        return lResult;
    }

    CatalogItem* CatalogManager::AddCatalogItem(const Json::Value& aSerializer)
    {
        mCatalogItems.push_back(new CatalogItem());
        mCatalogItems.back()->Deserialize(aSerializer);
        return mCatalogItems.back();
    }

    void CatalogManager::ClearCatalog()
    {
        mCatalogItems.clear();
    }
}