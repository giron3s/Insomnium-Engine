/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#include "precompiled.h"

//#include "SDL_mixer.h"

#include "audiomixer.h"

namespace Framework
{
    void AudioMixer::Initialize()
    {
        /*const int ret = Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
        if ( ret == -1 )
        {
            WARNING( "Error calling Mix_OpenAudio" );
        }*/
    }

    void AudioMixer::DeInitialize()
    {
        //Mix_CloseAudio();
    }

    bool AudioMixer::IsPlayingMusic() const
    {
        //return Mix_PlayingMusic() != 0;
        return false;
    } 
}
