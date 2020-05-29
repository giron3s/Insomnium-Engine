/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once

struct _Mix_Music;
typedef struct _Mix_Music Mix_Music;

namespace Framework
{
    class Music
    {
    public:
        static const int InfiniteLoop = -1;
        static const int NoLoop = 0;

        Music();
        Music( const std::string& musicFileName );
        ~Music();

        void Load( const std::string& musicFileName );
        void Unload();
        bool IsLoaded() const { return m_MusicInternal != nullptr; }

        void Play( const int loops = InfiniteLoop );
    private:
        std::string m_FileName;
        Mix_Music* m_MusicInternal;
    };
}
