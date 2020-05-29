/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

namespace Framework
{
    class Timer final
    {
    public:

        Timer();
        virtual ~Timer();

        void Update( const double aDeltaTime );
        void Start( const float aTimeToComplete = 1.0f /*In seconds*/ , const unsigned int aMaxIterations = 0 );
        void Stop();

        bool IsPlaying() const { return mIsPlaying; }

        Signal<> OnIteration;
        Signal<> OnComplete;

    private:
        unsigned int mMaxIterations;
        unsigned int mCurrentIteration;
        double       mTimeDelta;       /*In seconds*/
        double       mTimeToComplete;  /*In seconds*/
        bool         mIsPlaying;
    };
};
