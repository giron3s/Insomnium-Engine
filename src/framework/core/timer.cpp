/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "timer.h"
#include "engine.h"

namespace Framework
{
    Timer::Timer() :
          mMaxIterations( 0 )
        , mCurrentIteration( 0 )
        , mTimeDelta( 0.0f )
        , mTimeToComplete( 1.0f )
        , mIsPlaying( false )
    {
        Engine::Instance()->GameController().AddTimer( this );
    }

    Timer::~Timer()
    {
        Engine::Instance()->GameController().RemoveTimer( this );
    }

    void Timer::Start(float aTimeToComplete /*In seconds*/, unsigned int aMaxIterations)
    {
        mTimeToComplete = aTimeToComplete;
        mMaxIterations = aMaxIterations;
        mTimeDelta = 0.0f;
        mCurrentIteration = 0;
        mIsPlaying = true;
    }

    void Timer::Stop()
    {
        mTimeDelta = mTimeToComplete;
        mIsPlaying = false;
    }

    void Timer::Update( const double aDeltaTime )
    {
        if (!mIsPlaying)
            return;

        //Update the delta time
        if (mTimeDelta < mTimeToComplete)
        {
            mTimeDelta += aDeltaTime;
        }
        //Timer fired
        else
        {
            ++mCurrentIteration;
            if ( mMaxIterations && mCurrentIteration == mMaxIterations )
            {
                mIsPlaying = false;
                mTimeDelta = mTimeToComplete;

                OnIteration.Emit();
                OnComplete.Emit();
            }
            else
            {
                mTimeDelta = mTimeDelta - mTimeToComplete;
                OnIteration.Emit();
            }
        }
    }
}

