/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#include "precompiled.h"

#include "music.h"
//#include "SDL_mixer.h"

namespace Framework
{

    Music::Music()
        : m_FileName( "Undefined music" )
        , m_MusicInternal( nullptr )
    {
    }

    Music::Music( const std::string& musicFileName )
        : m_FileName( "Undefined music" )
        , m_MusicInternal( nullptr )
    {
        //Load( musicFileName );
    }

    Music::~Music()
    {
        //if ( IsLoaded() )
        //{
        //    Unload();
        //}
    }

    void Music::Load( const std::string& musicFileName )
    {
        //m_MusicInternal = Mix_LoadMUS( musicFileName.c_str() );
        //if ( m_MusicInternal == nullptr )
        //{
        //    WARNING( "Cannot load music: %s. Error: %s", musicFileName.c_str(), SDL_GetError() );
        //}
    }

    void Music::Unload()
    {
        //Mix_FreeMusic( m_MusicInternal );
        //m_MusicInternal = nullptr;
        //m_FileName = "Undefined music";
    }

    void Music::Play( const int loops )
    {
        //ASSERT( IsLoaded() );
        //const int ret = Mix_PlayMusic( m_MusicInternal, loops );
        //if ( ret == -1 )
        //{
        //    WARNING( "Cannot play music: %s. Error: %s", m_FileName.c_str(), SDL_GetError() );
        //}
    }

}
