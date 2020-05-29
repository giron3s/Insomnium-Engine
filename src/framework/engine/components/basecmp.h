/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once
#include "core/serialization/serializableobject.h"
#include "engine/CmpManagerRegistry.h"

namespace Framework
{
    class Entity;
    class Object3D;


    class BaseCmp : public SerializableObject
    {
    public:

        BaseCmp() 
            : mOwner(nullptr)
            , mCompId(UINT32_MAX)
        {

        }

        virtual ~BaseCmp()
        {

        }

        virtual uint32_t BaseCmp::GetUpdatePriority() const
        {
            return 0; // lowest update priority
        }

        Entity*      GetOwner() const { return mOwner; }
        virtual void SetOwner( Entity* aOwner ) { mOwner = aOwner; }

        ComponentID  GetCmpId() const { return mCompId; }
        void         SetCmpId(ComponentID aCompId) { mCompId = aCompId; }

        virtual void Update() { };

        virtual void Serialize(Json::Value& serializer) const override { };
        virtual void Deserialize(const Json::Value& serializer) override { };

    public:
        mutable uint32_t mCreationPriority = 0;

    protected:
        Entity*            mOwner;
        ComponentID        mCompId;
    };
}
