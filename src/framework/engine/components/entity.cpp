/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#include "precompiled.h"

#include "core/serialization/jsoncpputils.h"

#include "engine.h"
#include "engine/components/entity.h"
#include "engine/components/basecmp.h"
#include "engine/components/transformcmp.h"
#include "engine/components/motioncmp.h"
#include "engine/CmpManager.h"
#include "engine/message.h"
#include "engine/entitymanager.h"

namespace Framework
{
    static uint32_t              sId = 0;
    static std::vector<uint32_t> sReservedIds;

    uint32_t Entity::GetDefaultID()
    {
        return UINT32_MAX;
    }

    uint32_t Entity::AqcuireEntityId()
    {
       do
       {
           sId += 1;
       }
       while ( std::find(sReservedIds.cbegin(), sReservedIds.cend(), static_cast<uint32_t>(sId)) != sReservedIds.cend() );
       return sId;
    }

    Entity::Entity()
        : mName()
        , mActive(true)
        , mEntityID( AqcuireEntityId( ) )
        , mComponents()
    {
        mComponents.reserve( 2 );
    }

    Entity::Entity( const string& aName )
        : mName( aName )
        , mActive(true)
        , mEntityID( AqcuireEntityId() )
        , mComponents()
    {
        mComponents.reserve( 2 );
    }

    Entity::~Entity()
    {
        for(shared_ptr<BaseCmp> lComponent : mComponents )
        {
            ICmpManager *lCmpBase = Engine::Instance()->CmpManagerRegistry().GetByCompID(lComponent->GetCmpId());
            lCmpBase->RemoveCmp(lComponent.get());
        }
        mComponents.clear();
    }

    void Entity::AddComponent(shared_ptr<BaseCmp> lComponent)
    {
        lComponent->mCreationPriority = (uint32_t)mComponents.size();
        mComponents.push_back(lComponent);
        lComponent->SetOwner(this);
        ComponentAdded.Emit(lComponent);
        SortComponents();
    }

    void Entity::SortComponents()
    {
       // some components, like MotionCmp update other components, like TransformCmp and so must run before them
       // one way to ensure this is to sort and update them using a priority index (lower value means lower priority)
       // another way would be to have dedicated "systems" for updating each component type
       // the system update order would be determined at compile time, so no need for sorting in that case
       std::function<bool(const shared_ptr<BaseCmp> lLeftCmp, const shared_ptr<BaseCmp> lRightCmp)> lSortPred =
          [](const shared_ptr<BaseCmp> lLeftCmp, const shared_ptr<BaseCmp> lRightCmp)
       {
          return lLeftCmp->GetUpdatePriority() > lRightCmp->GetUpdatePriority();
       };

       std::sort(mComponents.begin(), mComponents.end(), lSortPred);
    }


    void Entity::SetName( const string& aName )
    {
        mName = aName;
    }

    void Entity::SetActive(const bool aActive)
    {
        mActive = aActive;
    }

    void Entity::Send(const Msg& aMsg)
    {
        // For the all callbacks that registed for a following message
        MsgsSubscriptions::const_iterator lSubscriptionIter = Engine::Instance()->EntityManager().mMsgsSubscriptions.find(aMsg.mMsgType);
        while (lSubscriptionIter != Engine::Instance()->EntityManager().mMsgsSubscriptions.end() && lSubscriptionIter->first == aMsg.mMsgType)
        {
            vector<shared_ptr<BaseCmp>>::iterator lCmpIter = mComponents.begin();
            while (lCmpIter != mComponents.end())
            {
                // If the entity has that component
                if (lCmpIter->get()->GetCmpId() == lSubscriptionIter->second.mComponentId)
                {
                    // Call the virtual Execute methode, wherewith we can make the upcast and send the message
                    BaseCmp* lBaseCmp = lCmpIter->get();
                    lSubscriptionIter->second.mMethod->Execute(lBaseCmp, aMsg);
                }
            }
            
            lSubscriptionIter++;
        }
    }

    void Entity::Serialize(Json::Value& aSerializer) const
    {
        aSerializer["type"] = Framework::Utils::Demangling(typeid(*this).name());
        aSerializer["name"] = mName;
        if ( !mComponents.empty() )
        {
            Json::Value& v = aSerializer["components"];
            v = Json::Value(Json::ValueType::arrayValue);
            std::vector< std::shared_ptr<BaseCmp> > lComponents(mComponents); // copy
            auto lSortPred = [](const shared_ptr<BaseCmp> lLeftCmp, const shared_ptr<BaseCmp> lRightCmp)
            { return lLeftCmp->mCreationPriority < lRightCmp->mCreationPriority; };
            std::sort(lComponents.begin(), lComponents.end(), lSortPred);
            for ( Json::ArrayIndex i = 0; i < lComponents.size(); ++i )
            {
                v[i]["type"] = Framework::Utils::Demangling(typeid(*lComponents[i]).name());
                lComponents[i]->Serialize(v[i]);
            }
        }
    }

    void Entity::Deserialize( const Json::Value& aSerializer )
    {
        const string& lEntityName = aSerializer["name"].asString();
        SetName( lEntityName );

        const auto& lComponents = aSerializer["components"];
        for ( const auto& lComponent : lComponents )
        {
            const string& lComponentType = lComponent["type"].asString();
            ICmpManager* lCmpManager = Engine::Instance()->CmpManagerRegistry().GetByCompName(lComponentType);
            ASSERT(lCmpManager != nullptr);

            shared_ptr<BaseCmp> lNewCmp = lCmpManager->AddCmp();
            ASSERT(lNewCmp != nullptr);

            AddComponent(lNewCmp);
            lNewCmp->Deserialize(lComponent);
        }
    }
}
