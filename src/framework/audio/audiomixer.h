/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once

#include "core/utils.h"

namespace Framework
{
    class AudioMixer
    {
        NON_COPYABLE_CLASS( AudioMixer );
    public:
        AudioMixer() {}
        ~AudioMixer() {}
        void Initialize();
        void DeInitialize();

        bool IsPlayingMusic() const;
    private:
        
    };
}
