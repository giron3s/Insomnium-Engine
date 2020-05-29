/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once
#include "ICmpManager.h"
#include "CmpManagerRegistry.h"

namespace Framework
{

#define INIT_CMP_MANAGER(aType, aName, aNumInstances) \
  CmpManager<aType>::Instance()->Init(aName, aNumInstances);

    template< class BaseCmpT >
    class CmpManager : public ICmpManager
    {
    private:

        std::vector<BaseCmp*>               mComponents;
        static ComponentID                  mCompId;
        size_t                              mMaxNumComponents;
        bool                                mUpdatingComponents;
        static CmpManager<BaseCmpT>         mInstance;
        std::string                         mName;

    public:

        static CmpManager <BaseCmpT>* Instance() { return &mInstance; }

        virtual int GetCmpID() { return mCompId; }
        virtual const char* GetName() const { return mName.c_str(); }
        virtual size_t Size() const { return 0; }
        virtual void Serialize(Json::Value& aSerializer) const {}
        virtual void Deserialize(Json::Value& aSerializer) const {}

        // Reserv the memory to the instances
        void Init(std::string aName, size_t aNumInstances)
        {
            ASSERT(mCompId == 0);   // Doesn't call this method more than one times!
            mName = aName;
            mCompId = Engine::Instance()->CmpManagerRegistry().RegisterCmpManager(this);
            mMaxNumComponents = aNumInstances;
            mComponents.reserve(aNumInstances);
        }

        // Return with the new component
        shared_ptr<BaseCmp> AddCmp() 
        {
            ASSERT(mComponents.size() < mMaxNumComponents);  //Too many components - Increase the size of the components in the entity manager
            shared_ptr<SerializableObject> lComponentInstance = Engine::Instance()->ObjectFactory().Create(mName);
            shared_ptr<BaseCmp> lBaseCmpInstance = dynamic_pointer_cast<BaseCmp>(lComponentInstance);
            lBaseCmpInstance->SetCmpId(mCompId);
            mComponents.push_back(lBaseCmpInstance.get());
            return lBaseCmpInstance;
        }

        void RemoveCmp(BaseCmp* aBaseCmp)
        {
            std::vector<BaseCmp*>::iterator lIter = std::find(mComponents.begin(), mComponents.end(), aBaseCmp);
            if (lIter != mComponents.end())
                mComponents.erase(lIter);
        }

        void Update()
        {
            for (auto lComponent : mComponents)
                lComponent->Update();
        }
    };

    template< class BaseCmpT >
    ComponentID CmpManager<BaseCmpT>::mCompId = 0;

    template< class BaseCmpT >
    CmpManager<BaseCmpT> CmpManager<BaseCmpT>::mInstance;
}

