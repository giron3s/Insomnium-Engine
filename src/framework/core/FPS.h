/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

namespace Framework
{
    class FPS
    {
    private:
        FPS() {};
        ~FPS() {};

    protected:
        static FPS*   mInstance;

        int           mFPS;
        std::string   mFPSInfo;

        int           mCount;
        unsigned long mStartTime;
        unsigned long mPreviousTicks;

    public:
        static FPS* GetInstance();

        void         Init();
        void         Update();
        void         Destroy();
        int          GetFPS() const { return mFPS; }
        std::string  GetFPSInfo() const { return mFPSInfo; }

    };
};
