/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "core/serialization/serializableobject.h"
#include "graphic/texture.h"

#include "cataloggroup.h"

namespace Game
{
    class CatalogItem : public Framework::SerializableObject
    {
    public:
                                            CatalogItem();
                                            CatalogItem(std::string aId, std::string aName, CatalogGroup aGroup, int aIcon, std::string aPrefabFile);

        const std::string&                  GetId() const { return mId; }
        const std::string&                  GetName() const { return mName; }
        CatalogGroup                        GetGroup() const { return mGroup; }
        int                                 GetIcon() const { return mIcon; }
        const std::string&                  GetPrefabFile() const { return mPrefabFile; }

        void                                Serialize(Json::Value& aSerialize) const override;
        void                                Deserialize(const Json::Value& aSerialize) override;

    protected:
        std::string                         mId;
        std::string                         mName;
        CatalogGroup                        mGroup;
        int                                 mIcon;
        std::string                         mPrefabFile;
    };
}
