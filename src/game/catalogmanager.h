/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once

#include "core/utils.h"
#include "core/serialization/jsoncpputils.h"

#include "engine/basemanager.h"
#include "cataloggroup.h"

namespace Game
{
    class CatalogItem;
}

namespace Game
{
    class CatalogManager : public Framework::BaseManager
    {
    public:
                                  CatalogManager();
                                  ~CatalogManager() override;
                                  
        void                      Initialize() override;
        void                      DeInitialize() override;
                                  
        void                      LoadCatalog(const std::string& aResourceFile);
        std::vector<CatalogItem*> GetCatalogItems();
        std::vector<CatalogItem*> GetCatalogItems(CatalogGroup aGroup);

    private:
        CatalogItem*              AddCatalogItem(const Json::Value& aSerializer);
        void                      ClearCatalog();

    protected:
        std::vector<CatalogItem*> mCatalogItems;
    };
}
