/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "CmpManagerRegistry.h"
#include "ICmpManager.h"
#include "components/basecmp.h"

namespace Framework
{
    CmpManagerRegistry::CmpManagerRegistry() : mCmpID(0)
    {
        mCmpManagers.resize(sMaxComponentId);
    }

    CmpManagerRegistry::~CmpManagerRegistry()
    {
        
    }

    ICmpManager* CmpManagerRegistry::GetByCompID(ComponentID aCmpID)
    {
        return mCmpManagers[aCmpID];
    }

    ICmpManager* CmpManagerRegistry::GetByCompName(const std::string &aCmpName) 
    {
        auto lResult = mCmpManagerByName.find(aCmpName);
        if (lResult == mCmpManagerByName.end())
            return NULL;
        return lResult->second;
    }

    void CmpManagerRegistry::Update() 
    {
        typedef vector< ICmpManager* >::const_iterator IT;
        IT lCmpManagerIter = mCmpManagers.begin();
        while (*lCmpManagerIter)
        {
            (*lCmpManagerIter)->Update();
            lCmpManagerIter++;
        }
    }


    unsigned int CmpManagerRegistry::RegisterCmpManager(ICmpManager* aNewManager)
    {
        ComponentID lNewId = mCmpID++;
        // too many components defined?, increase cte Entity::max_component_id
        ASSERT(lNewId < sMaxComponentId);
        mCmpManagers[lNewId] = aNewManager;

        // Register it in the dictionary by name
        mCmpManagerByName[aNewManager->GetName()] = aNewManager;
        return lNewId;
    }
}

