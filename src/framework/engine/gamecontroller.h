/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once
#include <vector>
#include "core/utils.h"

namespace Framework
{
    class Entity;    
    class Timer;
    class FPS;

    class GameController
    {
        NON_COPYABLE_CLASS( GameController );
    public:
        GameController();
        virtual ~GameController();

        bool   Initialize();
        void   DeInitialize();
        void   Update();
        void   AddTimer( Timer* aTimer );
        void   RemoveTimer( Timer* aTimer );

        double GetTotalTime() const { return mTotalTime; }
        double GetDeltaTime() const { return mDeltaTime; }
        double GetThisTime() const { return mThisTime; }
        double GetLastTime() const { return mLastTime; }

        bool   GetIsQuit() const { return mIsQuit; }
        void   OnQuit();

    protected:        
        void OnKeyboardEvent(const InputEventType aEventType, const KeyboardEventData& aKeyboardEventData);
        
        double                  mTotalTime;
        double                  mDeltaTime;
        double                  mThisTime;
        double                  mLastTime;
        bool                    mIsQuit;
        EventHandler            mQuitHandler;
        EventHandler            mKeyboardEventHandler;
        std::vector<Timer*>     mTimers;
        FPS*                    mFPSCounter;
    };

};
