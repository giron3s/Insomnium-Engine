/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once

#include "engine/container.h"

namespace Framework
{
    class ContainerFactory
    {
        NON_COPYABLE_CLASS(ContainerFactory);

    private:

        class ContainerCreator
        {
        public:
            ContainerCreator(const string& aName) : mName(aName) {}
            inline const string& GetName() const { return mName; }
            Container* Create() const { return CreateT(); }
        protected:
            string mName;
            virtual Container* CreateT() const = 0;
        };

        template <typename T>
        class ContainerCreatorT : public ContainerCreator
        {
        public:
            ContainerCreatorT(const string& aName) : ContainerCreator(aName) {}
            // this is pure interface so no destructor required
        protected:
            Container* CreateT() const override { return new T; }
        };

        ContainerFactory() : mInitialized(false) {}
        ~ContainerFactory() = default;

        friend class Engine;

    protected:
        bool Create(__out std::vector<Container*>& aContainers)
        {
            if (mInitialized)
            {
                CRASH("The container already exists. You can not factory the container again!");
                return false;
            }

            for (const auto& aContainerCreator : mContainerCreators)
            {
                aContainers.push_back(aContainerCreator->Create());
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
                CRASH("Can not register a new container after running the engine")
            }

            string lName = Framework::Utils::Demangling(typeid(T).name());
            for (const auto& lContainerCreator : mContainerCreators)
                if (lContainerCreator->GetName() == lName)
                {
                    WARNING("Duplicate container name detected")
                        return;
                }
            mContainerCreators.push_back(shared_ptr<ContainerCreator>(new ContainerCreatorT<T>(lName)));
        }

    private:
        vector< shared_ptr<ContainerCreator> >  mContainerCreators;
        bool mInitialized;
    };
}
