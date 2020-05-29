/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once

struct Mix_Chunk;

namespace Framework
{
    class Sound 
    {
    public:
        static const int InfiniteLoop = -1;
        static const int NoLoop = 0;

        Sound();
        Sound( const std::string& soundFileName );
        ~Sound();

        void Load( const std::string& soundFileName );
        void Unload();
        void Play( const int loops  = NoLoop );
        void PlayInChannel( const int channel, const int loops = NoLoop );

        bool IsLoaded() const { return m_Chunk != nullptr; }
    private:
        static const int FirstUnreservedChannel = -1;

        std::string m_FileName;
        Mix_Chunk* m_Chunk;        
    };
}
