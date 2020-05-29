/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "CmpManager.h"

/*
namespace Framework
{
    // Reserve the memory to the instances
    void CmpManager<BaseCmp>::Init(size_t aNumInstances)
    {
        ASSERT(mCompId == 0);   // Doesn't call this method more than one times!
        mCompId = Engine::Instance()->CmpManagerRegistry().RegisterCmpManager(this);
        mComponents.reserve(aNumInstances);
        mMaxNumComponents = aNumInstances;
    }

    // Return with the new component
    BaseCmp* CmpManager<class BaseCmp>::AddCmp()
    {
        ASSERT(mComponents.size() < mMaxNumComponents);  //Too many components - Increase the size of the components in the entity manager
        mComponents.push_back(BaseCmp());
        return &mComponents.back();
    }

    // Remove the component
    void CmpManager<BaseCmp>::RemoveCmp(BaseCmp* aBaseCmp)
    {
        //Todo  
    }

    void CmpManager<BaseCmp>::Update(float aDelta)
    {
        for (auto lComponent : mComponents)
            lComponent.Update();
    }
}*/