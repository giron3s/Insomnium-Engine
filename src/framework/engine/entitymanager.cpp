/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/
#include "precompiled.h"
#include "entitymanager.h"
#include "engine.h"
#include "objectfactory.h"
#include "core\logger.h"
#include "CmpManager.h"

#include "components/entity.h"
#include "components/basecmp.h"
#include "components/motioncmp.h"
#include "components/transformcmp.h"
#include "components/catalogcmp.h"
#include "components/cameraCmp.h"
#include "components/renderablecmp.h"
#include "components/directlightcmp.h"
#include "components/spotlightcmp.h"
#include "components/pointlightcmp.h"


namespace Framework
{
     EntityManager::EntityManager()
        : mEntities()
    {
    }

    EntityManager::~EntityManager()
    {
        ASSERT( mEntities.empty() );
    }

    void EntityManager::Initialize()
    {
        //REGISTER THE COMPONENTS ORDER THAT YOU WANT TO UPDATE THE COMPONENTS
        INIT_CMP_MANAGER(CameraCmp,      typeid(CameraCmp).name(),      64);
        INIT_CMP_MANAGER(CatalogCmp,     typeid(CatalogCmp).name(),     64);
        INIT_CMP_MANAGER(TransformCmp,   typeid(TransformCmp).name(),   64);
        INIT_CMP_MANAGER(MotionCmp,      typeid(MotionCmp).name(),      64);

        INIT_CMP_MANAGER(Object3DCmp,    typeid(Object3DCmp).name(),    64);
        INIT_CMP_MANAGER(RenderableCmp,  typeid(RenderableCmp).name(),  64);
        INIT_CMP_MANAGER(DirectLightCmp, typeid(DirectLightCmp).name(), 64);
        INIT_CMP_MANAGER(SpotLightCmp,   typeid(SpotLightCmp).name(),   64);
        INIT_CMP_MANAGER(PointLightCmp,  typeid(PointLightCmp).name(),  64);

        SUBSCRIBE(CatalogCmp, eMSG_ENTITY_CREATED, OnNewEntity);
    }

    void EntityManager::DeInitialize()
    {
        mEntities.clear();
    }

    void EntityManager::Update()
    {
        PROFILER_EVENT();

        DeleteDestroyedEntities();
    }

    void EntityManager::DeleteDestroyedEntities()
    {
        if (mDestroyedEntities.size() == 0)
            return;

        for ( const shared_ptr<Entity>& lEntity : mDestroyedEntities )
        {
            for (auto i = mEntities.begin(); i != mEntities.end(); ++i)
            {
                if ( *i == lEntity )
                {
                    mEntities.erase(i);
                    break;
                }
            }
        }
        mDestroyedEntities.clear();
    }

    weak_ptr<Framework::Entity> EntityManager::CreateEntity()
    {
        shared_ptr<SerializableObject> lObj = Engine::Instance()->ObjectFactory().Create("class Framework::Entity");
        shared_ptr<Entity> lEntity(dynamic_pointer_cast<Entity>(lObj));
        AddEntity(lEntity);
        return weak_ptr<Entity>( lEntity );
    }

    weak_ptr<Entity> EntityManager::CreateEntity( const std::string& aName )
    {
        weak_ptr<Entity> lEntity = CreateEntity();
        lEntity.lock()->SetName( aName );
        return lEntity;
    }

    weak_ptr<Framework::Entity> EntityManager::CreateEntityFromData(const Json::Value& aSerializer)
    {
        std::string lEntityType = aSerializer["type"].asString();

        shared_ptr<SerializableObject> lObj = Engine::Instance()->ObjectFactory().Create(lEntityType);
        shared_ptr<Entity> lEntity(dynamic_pointer_cast<Entity>(lObj));
        lEntity->Deserialize(aSerializer);
        AddEntity(lEntity);
        return weak_ptr<Entity>(lEntity);
    }

    weak_ptr<Framework::Entity> EntityManager::CreateEntityFromPrefab(const std::string& prefabName)
    {
        Json::Value prefab = Engine::Instance()->PrefabManager().GetPrefab(prefabName);
        return CreateEntityFromData(prefab);
    }

    void EntityManager::AddEntity( const shared_ptr<Entity>& aEntity )
    {
        for (auto i = mEntities.begin(); i != mEntities.end(); ++i)
            if ( *i == aEntity )
                CRASH("EntityManager::AddEntity : Attempt to add the same entity twice");
        mEntities.push_back( aEntity );
    }

    void EntityManager::DestroyEntity( size_t aEntityID )
    {
        for ( auto& lEntity : mEntities )
            if ( lEntity->GetEntityID() == aEntityID )
                mDestroyedEntities.push_back(lEntity);
    }

    std::shared_ptr<Entity> EntityManager::GetEntityByID(size_t aEntityID) const
    {
        for ( const shared_ptr<Entity>& lEntity : mEntities )
        {
            if ( lEntity->GetEntityID() == aEntityID )
            {
                return lEntity;
            }
        }
        return std::shared_ptr<Entity>();
    }

    shared_ptr<Entity> EntityManager::GetEntityByName( const std::string& aName ) const
    {
        for ( const shared_ptr<Entity>& lEntity : mEntities )
        {
            if ( lEntity->GetName().compare(aName) == 0 )
            {
                return lEntity;
            }
        }
        return std::shared_ptr<Entity>();
    }

    void EntityManager::GetEntitiesByName( const std::string& aName, std::vector<weak_ptr<Entity>>& entities ) const
    {
        for ( const shared_ptr<Entity>& lEntity : mEntities )
        {
            if ( lEntity->GetName().compare(aName) == 0 )
            {
                entities.push_back( weak_ptr<Entity>( lEntity ) );
            }
        }
    }

    shared_ptr<Entity> EntityManager::GetEntityByNameSubString( const std::string& aName ) const
    {
        for ( const shared_ptr<Entity>& lEntity : mEntities )
        {
            if ( lEntity->GetName().find(aName) != std::string::npos )
            {
                return lEntity;
            }
        }
        return std::shared_ptr<Entity>();
    }

    void EntityManager::GetEntitiesByNameSubString( const std::string& aName, std::vector<weak_ptr<Entity>>& entities ) const
    {
        for ( const shared_ptr<Entity>& lEntity : mEntities )
        {
            if ( lEntity->GetName().find( aName ) != std::string::npos )
            {
                entities.push_back( weak_ptr<Entity>( lEntity ) );
            }
        }
    }

    void EntityManager::Serialize(Json::Value& aSerializer) const
    {

    }

    void EntityManager::Deserialize(const Json::Value& aSerializer)
    {
        shared_ptr<Entity> lEntitysp;

        const Json::Value& entities = aSerializer["entities"];
        for (const auto& lEntity : entities)
        {
            Json::Value finalentity;

            std::string prefab = lEntity.get("prefab", "").asString();
            if (!prefab.empty())
            {
                finalentity = Engine::Instance()->PrefabManager().GetPrefab(prefab);
                Json::Value overrideParams = lEntity["override"];
                Json::JsonUtils::OverrideEntity(finalentity, overrideParams);
            }
            else
            {
                finalentity = lEntity;
            }
            CreateEntityFromData(finalentity);
        }
    }

}
