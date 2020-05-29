/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Catalog Component
 *******************************************************************************/

#pragma once

#include "engine/components/basecmp.h"

namespace Framework
{
    class CatalogCmp : public BaseCmp
    {
    public:

        CatalogCmp();
        ~CatalogCmp();
        
        const std::string&  GetName() const     { return mName; }
        const std::string&  GetGroup() const    { return mGroup; }
        const std::string&  GetIcon() const     { return mIcon; }

        void    SetName(std::string aName)      { mName = aName; }
        void    SetGroup(std::string aGroup)    { mGroup = aGroup; }
        void    SetIcon(std::string aIcon)      { mIcon = aIcon; }

        void    Serialize(Json::Value& aSerializer) const override;
        void    Deserialize(const Json::Value& aSerializer) override;

        void    OnNewEntity(const Msg& msg) {}

    protected:

        std::string     mName;
        std::string     mGroup;
        std::string     mIcon;
    };
}

