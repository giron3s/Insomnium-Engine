/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"

#include <time.h>

#include "core/timer.h"
#include "engine.h"
#include "engine/inputmanager.h"
#include "engine/gamecontroller.h"
#include "engine/components/entity.h"
#include "engine/components/basecmp.h"
#include "core/FPS.h"

namespace Framework
{
    GameController::GameController()
        : mTotalTime(0.0)
        , mDeltaTime(0.0)
        , mThisTime(0.0)
        , mLastTime(0.0)
        , mIsQuit(false)
        , mQuitHandler()
        , mKeyboardEventHandler()
        , mTimers()
        , mFPSCounter(nullptr)
    {
        mTimers.reserve( 4 );
    }

    GameController::~GameController()
    {
        if ( !mTimers.empty() )
        {
            WARNING( "GameController::~GameController - A timer has not been removed from mTimers" );
        }
    }

    bool GameController::Initialize()
    {

        mQuitHandler = Engine::Instance()->Quit.Connect( [this]() { OnQuit(); } );
        mKeyboardEventHandler = Engine::Instance()->Input().KeyboardEvent.Connect([this](const InputEventType aEventType, const KeyboardEventData& aKeyboardEventData) { OnKeyboardEvent(aEventType, aKeyboardEventData); });

        mThisTime = GetTickCount();
        mLastTime = GetTickCount();

        mFPSCounter = FPS::GetInstance();
        mFPSCounter->Init();
        return true;
    }

    void GameController::DeInitialize()
    {
        Engine::Instance()->Input().KeyboardEvent.Disconnect( mKeyboardEventHandler );
        Engine::Instance()->Quit.Disconnect( mQuitHandler );
        mFPSCounter->Destroy();
    }

    void GameController::Update()
    {
        PROFILER_EVENT();

        //Update the delta time
        mThisTime = GetTickCount();
        mDeltaTime = (mThisTime - mLastTime) / 1000.f;
        mLastTime = mThisTime;
        mTotalTime += mDeltaTime;

        //Update the timers
        for (Timer* lTimer : mTimers)
            lTimer->Update(mDeltaTime);

        //Update 
        mFPSCounter->Update();
    }

    void GameController::AddTimer( Timer* timer )
    {
        mTimers.push_back( timer );
    }
        
    void GameController::RemoveTimer( Timer* timer )
    {
        auto it = remove( mTimers.begin(), mTimers.end(), timer );
        if ( it != mTimers.end() )
        {
            mTimers.erase( it );
        }
    }

    void GameController::OnQuit()
    {
        mIsQuit = true;
    }

    void GameController::OnKeyboardEvent(const InputEventType aEventType, const KeyboardEventData& aKeyboardEventData)
    {
       if (aKeyboardEventData.keyboardKey == GLFW_KEY_ESCAPE)
       {
           OnQuit();
       }
    }
    
}
