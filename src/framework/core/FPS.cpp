/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/



#include "precompiled.h"
#include "FPS.h"

using namespace Framework;

// Initialize the instance
FPS* FPS::mInstance = 0;

// Return with the instance of the class
FPS* FPS::GetInstance()
{
    if (mInstance == 0)
        mInstance = new FPS();
    return mInstance;
}

 // The Initialize function sets all the counters to zero and starts the timer.
void FPS::Init()
{
    mFPS = 0;
    mCount = 0;
    mStartTime = GetTickCount();
    mPreviousTicks = mStartTime;
}

// The Frame function must be called each frame so that it can increment the frame count by 1. If it finds that one second has elapsed then it will store the frame count in the m_fps variable. It then resets the count and starts the timer again.
void FPS::Update() 
{
    //  Option 1 - Counts total application update cycles per second
    mCount++;
    if (GetTickCount() >= (mStartTime + 1000)) 
    {
        mFPS = mCount;
        mCount = 0;
        mStartTime = GetTickCount();

        static const string sText = "FPS: ";
        mFPSInfo = sText + to_string(mFPS);
    }

    //  Option 2 - Counts frames rendered per second (GPU refresh rate dependant)
    /*
    int iCurrentTicks = GetTickCount();
    int lTicksThisFrame = iCurrentTicks - mPreviousTicks;
    mPreviousTicks = iCurrentTicks;

    if (lTicksThisFrame > 0 && (iCurrentTicks - mStartTime) > 200)
    {
        mStartTime = iCurrentTicks;
        mFPS = (mFPS + (1000.0f / lTicksThisFrame)) * 0.5f;
    }
    */
}

void FPS::Destroy() 
{
    delete mInstance;
    mInstance = NULL;
}
