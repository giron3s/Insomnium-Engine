/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Manager class
 *******************************************************************************/


#pragma once

#include "engine/basemanager.h"

namespace Framework
{
    class ManagerFactory
    {
        NON_COPYABLE_CLASS(ManagerFactory);

    private:

            class ManagerCreator
            {
            public:
                ManagerCreator(const string& aName) : mName(aName) {}
                inline const string& GetName() const { return mName; }
                BaseManager* Create() const { return CreateT(); }
            protected:
                string mName;
                virtual BaseManager* CreateT() const = 0;
            };
        
            template <typename T>
            class ManagerCreatorT : public ManagerCreator
            {
            public:
                ManagerCreatorT(const string& aName) : ManagerCreator(aName) {}
                // this is pure interface so no destructor required
            protected:
                BaseManager* CreateT() const override { return new T; }
            };
        
        ManagerFactory() : mInitialized(false) {}
        ~ManagerFactory() = default;

        friend class Engine;

    protected:
        bool Create(__out std::vector<BaseManager*>& aManagers)
        {
            if (mInitialized)
            {
                CRASH("The manager already exists. You can not factory the managers again!");
                return false;
            }

            for (const auto& aManagerCreator : mManagerCreators)
            {
                aManagers.push_back( aManagerCreator->Create() );
            }
            mInitialized = true;
            return true;
        }

    public:
        template <typename T>
        void Register()
        {
            if (mInitialized)
            {
                CRASH("Can not register a new manager after running the engine")
            }

            string lName = Framework::Utils::Demangling(typeid(T).name());
            for (const auto& lManageCreator : mManagerCreators)
                if (lManageCreator->GetName() == lName)
                {
                    WARNING("Duplicate manager name detected")
                    return;
                }
            mManagerCreators.push_back( shared_ptr<ManagerCreator>(new ManagerCreatorT<T>(lName)) );
        }

    private:
        vector< shared_ptr<ManagerCreator> >  mManagerCreators;
        bool                                  mInitialized;


    };
}
