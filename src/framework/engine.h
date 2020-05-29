/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once

//TODO: Check that all of those includes cannot be removed and use forward declaration instead.

#include "graphic/display.h"
#include "core/config.h"
#include "audio/audiomixer.h"
#include "engine/entitymanager.h"
#include "engine/gamecontroller.h"
#include "engine/inputmanager.h"
#include "engine/objectfactory.h"
#include "engine/managerfactory.h"
#include "engine/containerfactory.h"
#include "engine/prefabmanager.h"
#include "engine/resourcemanager.h"
#include "engine/script.h"
#include "engine/timemanager.h"
#include "engine/fsm/statemachine.h"
#include "engine/languagemanager.h"
#include "engine/CmpManagerRegistry.h"

namespace Framework
{
    class Engine
    {
        NON_COPYABLE_CLASS( Engine );
    public:
        static Engine* Instance() { return s_Instance; }

        Engine(const std::string& aConfigFileName);
        ~Engine();      

        void Run();

        const Framework::BaseManager& Manager(const std::string& aManagerName) const;
        Framework::BaseManager& Manager(const std::string& aManagerName);
        const Framework::EntityManager& EntityManager() const { return mEntityManager; }
        Framework::EntityManager& EntityManager() { return mEntityManager; }
        const Framework::InputManager& Input() const { return mInputManager; }
        Framework::InputManager& Input() { return mInputManager; }
        //const Framework::Renderer& Renderer() const { return mRenderer; }
        Framework::Display& Display() { return mDisplay; }
        const Framework::GameController& GameController() const { return mGameController; }
        Framework::GameController& GameController() { return mGameController; }
        const Framework::Config& Config() const { return mConfig; }
        Framework::Config& Config() { return mConfig; }
        const Framework::AudioMixer& Audio() const { return mAudio; }
        Framework::AudioMixer& Audio() { return mAudio; }
        const Framework::ObjectFactory& ObjectFactory() const { return mObjectFactory; }
        Framework::ObjectFactory& ObjectFactory() { return mObjectFactory; }
        const Framework::ManagerFactory& ManagerFactory() const { return mManagerFactory; }
        Framework::ManagerFactory& ManagerFactory() { return mManagerFactory; }
        const Framework::PrefabManager& PrefabManager() const { return mPrefabManager; }
        Framework::PrefabManager& PrefabManager() { return mPrefabManager; }
        const Framework::ResourceManager& ResourceManager() const { return mResourceManager; }
        Framework::ResourceManager& ResourceManager() { return mResourceManager; }
        const Framework::Script& Script() const { return mScript; }
        Framework::Script& Script() { return mScript; }
        //const Framework::StateMachine& StateMachine() const { return mStateMachine; }
        Framework::StateMachine& StateMachine() { return mStateMachine; }
        Framework::CmpManagerRegistry& CmpManagerRegistry() { return mCmpManagerRegistry; }
        const Framework::CmpManagerRegistry& CmpManagerRegistry() const { return mCmpManagerRegistry; }
        //const Framework::TimeManager& TimeManager() const { return mTimer; }
        //Framework::TimeManager& TimeManager() { return mTimer; }

        const Framework::ContainerFactory& ContainerFactory() const { return mContainerFactory; }
        Framework::ContainerFactory& ContainerFactory() { return mContainerFactory; }
        const Framework::Container& Container(const std::string& aContainerName) const;
        Framework::Container& Container(const std::string& aContainerName);

        void BindInterface(LuaIntf::LuaState& aLuaState);
        void HandleInputEvent(const EventData& aInputEvent);

        Signal<> Quit;

    private:
        Engine() = default;

        void Update();

        static Engine*                     s_Instance;
            
        Framework::CmpManagerRegistry      mCmpManagerRegistry;
        Framework::EntityManager           mEntityManager;
        Framework::InputManager            mInputManager;
        Framework::Display                 mDisplay;
        Framework::GameController          mGameController;
        Framework::Config                  mConfig;
        Framework::AudioMixer              mAudio;
        Framework::ObjectFactory           mObjectFactory;
        Framework::ManagerFactory          mManagerFactory;
        Framework::ContainerFactory        mContainerFactory;
        Framework::PrefabManager           mPrefabManager;
        Framework::ResourceManager         mResourceManager;
        Framework::Script                  mScript;
        Framework::StateMachine            mStateMachine; 
        
        //Framework::TimeManager*          mTimer;
        
        std::vector<BaseManager*>          mManagers; // custom managers
        std::vector<Framework::Container*> mContainers;
    };
}
