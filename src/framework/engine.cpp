/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#include "precompiled.h"

#include "engine.h"
#include "core/serialization/serializableobject.h"
#include "core/serialization/jsoncpputils.h"

using namespace LuaIntf;

namespace Framework
{
    Engine* Engine::s_Instance = nullptr;

    Engine::Engine(const string& aConfigFileName)
         : mCmpManagerRegistry()
         , mEntityManager()
         , mInputManager()
         , mDisplay()
         , mGameController()
         , mConfig(aConfigFileName)
         , mAudio()
         , mResourceManager()
         , mScript()
         , mStateMachine()
         //, mTimer() = TimeManager::GetInstance();
    {
        ASSERT(s_Instance == nullptr);
        s_Instance = this;

        // Setting memory allocators
        PROFILER_SET_MEMORY_ALLOCATOR(
            [](size_t size) -> void* { return operator new(size); },
            [](void* p) { operator delete(p); },
            []() { /* Do some TLS initialization here if needed */ }
        );

    }

    Engine::~Engine()
    {
        try
        {
            mEntityManager.DeInitialize();
            mGameController.DeInitialize();
            mAudio.DeInitialize();
            mResourceManager.DeInitialize();
            mDisplay.DeInitialize();
            //mTimer = TimeManager::GetInstance();

            for (auto lManager : mManagers)
                lManager->DeInitialize();

            for (auto lContainer : mContainers)
                lContainer->DeInitialize();

            Logger::DeInitialize();
            s_Instance = nullptr;

            PROFILER_SHUTDOWN();
        }
        catch (const std::exception& lException) 
        {
            CRASH("Exception: %s", lException.what());
            Logger::DeInitialize();
        }
    }

    void Engine::Run()
    {
        ContainerFactory().Create(mContainers);
        ManagerFactory().Create(mManagers);
        
        EntityManager().Initialize();
        Display().Initialize(Config().GetResolution().x, Config().GetResolution().y, Config().GetFullScreen(), Config().GetResizable(), Config().GetGameName());
        Audio().Initialize();
        GameController().Initialize();
        PrefabManager().GetPrefabsFromConfig(Config());
        ResourceManager().Initialize(mConfig.GetResourceFiles());
        Script().Initialize(mConfig.GetInitialStateFile());

        //Initalize containers
        for (auto lContainer : mContainers)
            lContainer->Initialize();

        //Initalize managers
        for (auto lManager : mManagers)
            lManager->Initialize();

        //Init the run
        Script().Run();
        Script().RunFunction("INIT");
        while (!GameController().GetIsQuit())
        {
            Update();
        }
    }

    const Framework::BaseManager& Engine::Manager(const string &aName) const
    {
        for (const auto lManager : mManagers)
            if (lManager->GetName() == aName)
                return *lManager;
        CRASH("Not find the '%s' manager", aName.c_str());
        return *mManagers[0];
    }

    Framework::BaseManager& Engine::Manager(const string &aName)
    {
        for (auto lManager : mManagers)
            if (lManager->GetName() == aName)
                return *lManager;
        CRASH("Not find the '%s' manager", aName.c_str());
        return *mManagers[0];
    }

    const Framework::Container& Engine::Container(const string &aName) const
    {
        for (const auto lContainer : mContainers)
            if (lContainer->GetName() == aName)
                return *lContainer;
        CRASH("Cannot find '%s' container", aName.c_str());
        return *mContainers[0];
    }

    Framework::Container& Engine::Container(const string &aName)
    {
        for (auto lContainer : mContainers)
            if (lContainer->GetName() == aName)
                return *lContainer;
        CRASH("Cannot find '%s' container", aName.c_str());
        return *mContainers[0];
    }

    void Engine::BindInterface(LuaState& aLuaState)
    {
        Engine* lInstance = s_Instance;
         LuaBinding(aLuaState).addFunction("GetEngine", [&lInstance]() -> Engine& // The -> specifies the return type. The () are needed for this syntax.
         {
             return *Engine::Instance();
         });

         LuaBinding(aLuaState).beginClass<Engine>("Engine")
             .addFunction("StateMachine",&Engine::StateMachine)
             .addFunction("Display",     &Engine::Display)
         .endClass();
    }

    void Engine::Update()
    {
        PROFILER_FRAME("MainThread")

        mEntityManager.Update();
        mDisplay.Update();
        mGameController.Update();
        mCmpManagerRegistry.Update();
        mStateMachine.Update();
    }

    void Engine::HandleInputEvent(const EventData& aInputEvent)
    {
        const InputEventType lType = static_cast<InputEventType>( aInputEvent.type );
        switch ( lType )
        {
            case InputEventType::KeyboardEvent:
            {
                if( aInputEvent.data.key.keyboardKey == GLFW_KEY_ESCAPE && aInputEvent.data.key.action == GLFW_PRESS )
                {
                    Quit.Emit();
                }
                else if (aInputEvent.data.key.keyboardKey == GLFW_KEY_F11 && aInputEvent.data.key.action == GLFW_PRESS)
                {
                    Engine::Instance()->StateMachine().ExecuteAction("GotoThemeBuilder");
                }
                else
                {
                    mInputManager.HandleInputEvent( aInputEvent );
                }
                break;
            }
            case InputEventType::MouseMotionEvent:
            case InputEventType::MouseButtonEvent:
            case InputEventType::ResizeEvent:
            case InputEventType::FileDropEvent:
            case InputEventType::ScrollEvent:
            case InputEventType::FocusEvent:
            {
                mInputManager.HandleInputEvent(aInputEvent);
                break;
            }
           default:
               WARNING("Unhandled InputEventType: %d", lType)
               break;
        }
    }
}
