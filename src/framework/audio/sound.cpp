/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#include "precompiled.h"

//#include "SDL_mixer.h"

#include "sound.h"

namespace Framework
{

    Sound::Sound()
        : m_FileName( "Undefined sound" )
        , m_Chunk( nullptr )
    {
    }

    Sound::Sound( const std::string& soundFileName )
        : m_FileName( "Undefined sound" )
        , m_Chunk( nullptr )
    {
        //Load( soundFileName );
    }

    Sound::~Sound()
    {
        if ( IsLoaded() )
        {
           // Unload();
        }
    }

    void Sound::Load( const std::string& soundFileName )
    {
        //m_Chunk = Mix_LoadWAV( soundFileName.c_str() );
        //if ( m_Chunk == nullptr )
        //{
        //    WARNING( "Cannot load sound file: %s. Error: %s", soundFileName.c_str(), SDL_GetError() );
        //}
    }

    void Sound::Unload()
    {
        //Mix_FreeChunk( m_Chunk );
        //m_Chunk = nullptr;
        //m_FileName = "Undefined sound";
    }

    void Sound::Play( const int loops )
    {
        //int channel = Mix_PlayChannel( FirstUnreservedChannel, m_Chunk, loops );
        //if ( channel == -1 )
        //{
        //    WARNING( "Cannot play sound: %s. Error: %s", m_FileName.c_str(), SDL_GetError() );
        //}
    }

    void Sound::PlayInChannel( const int channel, const int loops )
    {
        //int channelItPlayed = Mix_PlayChannel( channel, m_Chunk, loops );
        //if ( channelItPlayed == -1 )
        //{
        //    WARNING( "Cannot play sound: %s. Error: %s", m_FileName.c_str(), SDL_GetError() );
        //}
    }

}
