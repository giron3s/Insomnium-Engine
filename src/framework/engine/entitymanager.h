/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

#include <list>
#include <vector>

#include "core/utils.h"
#include "core/serialization/jsoncpputils.h"
#include "core/serialization/serializableobject.h"
#include "message.h"

namespace Framework
{
    class Entity;

    typedef std::multimap< MsgType, ComponentCallbackInfo > MsgsSubscriptions;

    class EntityManager : public SerializableObject
    {
        NON_COPYABLE_CLASS( EntityManager );

        friend class Entity;

    public:

    #define SUBSCRIBE(aComponent, aMessage, aMethod) \
            SubscribeTo<aComponent>( aMessage, new Functor<aComponent>( &aComponent::aMethod ) );

                                EntityManager();
        virtual                 ~EntityManager();

        void                    Initialize();
        void                    DeInitialize();
        void                    Update();

        std::weak_ptr<Entity>   CreateEntity();
        std::weak_ptr<Entity>   CreateEntity( const std::string& aName );
        std::weak_ptr<Entity>   CreateEntityFromData( const Json::Value& aSerializer);
        std::weak_ptr<Entity>   CreateEntityFromPrefab(const std::string& aPrefabName);

        void                    AddEntity( const std::shared_ptr<Entity>& aEntity );
        void                    DestroyEntity( size_t aEntityID );

        std::shared_ptr<Entity> GetEntityByID(size_t aEntityID) const;
        std::shared_ptr<Entity> GetEntityByName( const std::string& aName ) const;
        void                    GetEntitiesByName( const std::string& aName, std::vector<std::weak_ptr<Entity>>& aEntities ) const;
        std::shared_ptr<Entity> GetEntityByNameSubString( const std::string& aName ) const;
        void                    GetEntitiesByNameSubString( const std::string& aName, std::vector<std::weak_ptr<Entity>>& aEntities ) const;

        void                    Serialize(Json::Value& aSerializer) const override;
        void                    Deserialize(const Json::Value& aSerializer) override;
        
    private:

        template< class Component >
        void SubscribeTo(MsgType aMsgType, IFunctorBase* aMethod);

        void                    DeleteDestroyedEntities();

        std::list< std::shared_ptr<Entity> >    mEntities;
        std::vector< std::shared_ptr<Entity> >  mDestroyedEntities;

        // A multimap to store who should be called when a msg type arrives
        MsgsSubscriptions  mMsgsSubscriptions;
    };

    template< class Component >
    void EntityManager::SubscribeTo(MsgType aMsgType, IFunctorBase* aMethod)
    {
        std::pair<MsgType, ComponentCallbackInfo> lNewSubscription;
        lNewSubscription.first = aMsgType;
        lNewSubscription.second.mComponentId = CmpManager<Component>::Instance()->GetCmpID();
        lNewSubscription.second.mMethod = aMethod;
        mMsgsSubscriptions.insert(lNewSubscription);
    }
}
