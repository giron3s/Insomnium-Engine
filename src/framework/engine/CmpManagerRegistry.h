/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once
typedef unsigned int ComponentID;
static const ComponentID sMinComponentId = 1;
static const ComponentID sMaxComponentId = 64;

namespace Framework
{
    class ICmpManager;

    class CmpManagerRegistry
    {
    private:

        ComponentID                           mCmpID;
        std::vector< ICmpManager* >           mCmpManagers;
        std::map< std::string, ICmpManager*>  mCmpManagerByName;

    public:

        CmpManagerRegistry();
        ~CmpManagerRegistry();

        void         Update();

        ICmpManager* GetByCompID(ComponentID aCmpID);
        ICmpManager* GetByCompName(const std::string &aCmpName);

        unsigned RegisterCmpManager(ICmpManager* aNewManager);

    };
}
