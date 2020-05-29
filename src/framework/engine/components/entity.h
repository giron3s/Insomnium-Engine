/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once
#include <vector>
#include "core/signal.h"
#include "core/serialization/serializableobject.h"

namespace Framework
{
    class BaseCmp;
    struct Msg;
}


namespace Framework
{
    class Entity : public SerializableObject
    {
        NON_COPYABLE_CLASS(Entity);



    public:
        static uint32_t     GetDefaultID();

                            Entity();
                            Entity(const std::string& aName);
        virtual             ~Entity();

        void                SetName(const std::string& aName);
        const std::string&  GetName() const { return mName; }
        void                SetActive(const bool aActive);
        const bool          IsActive() const { return mActive; }
        size_t              GetEntityID() const { return mEntityID; }

        template <class COMPONENT_TYPE>
        std::weak_ptr<COMPONENT_TYPE> GetComponent() const;

        template <class COMPONENT_TYPE>
        bool HasComponent() const;

        void                Send(const Msg& aMsg);

        virtual void        Serialize(Json::Value& serializer) const override;
        virtual void        Deserialize(const Json::Value& serializer) override;

    private:
        static uint32_t     AqcuireEntityId();
        void                AddComponent(std::shared_ptr<BaseCmp> aComponent);
        void                SortComponents();

        Signal<std::shared_ptr<BaseCmp>>      ComponentAdded;

    protected:
        std::string                           mName;
        bool                                  mActive;
        size_t                                mEntityID;
        std::vector<std::shared_ptr<BaseCmp>> mComponents;
    };

    template <class COMPONENT_TYPE>
    std::weak_ptr<COMPONENT_TYPE> Entity::GetComponent() const
    {
        for ( std::shared_ptr<BaseCmp> lComponent : mComponents )
        {
            if ( std::shared_ptr<COMPONENT_TYPE> sComponent = std::dynamic_pointer_cast<COMPONENT_TYPE>(lComponent) )
            {
                return std::weak_ptr<COMPONENT_TYPE>( sComponent );
            }
        }

        return std::weak_ptr<COMPONENT_TYPE>();
    }

    template <class COMPONENT_TYPE>
    bool Entity::HasComponent() const
    {
        for (std::shared_ptr<BaseCmp> lComponent : mComponents)
        {
            if (std::shared_ptr<COMPONENT_TYPE> sComponent = std::dynamic_pointer_cast<COMPONENT_TYPE>(lComponent))
            {
                return true;
            }
        }
        return false;
    }
}
