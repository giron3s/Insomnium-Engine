/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once


#include "engine/container.h"

namespace Game
{
    class UserConfig : public Framework::Container
    {
    public:
                UserConfig();
                ~UserConfig() override;
                                  
        void    Initialize() override;
        void    DeInitialize() override;

        bool        GetFTUEEnabled()        const   { return mFTUEEnabled;      }
        bool        GetAutosaveEnabled()    const   { return mAutosaveEnabled;  }
        int         GetAutosaveTime()       const   { return mAutosaveTime;     }
        int         GetLogLevel()           const   { return mLogLevel;         }
        std::string GetLanguage()           const   { return mLanguage;         }
        bool        GetLoggedIn()           const   { return mLoggedIn;     }

        void SetFTUEEnabled(bool aEnabled)          { mFTUEEnabled = aEnabled;      }
        void SetAutosaveEnabled(const bool aEnabled){ mAutosaveEnabled = aEnabled;  }
        void SetAutosaveTime(int aTime)             { mAutosaveTime = aTime;        }
        void SetLogLevel(int aLevel)                {  mLogLevel = aLevel;          }
        void SetLanguage(std::string aLanguage)     { mLanguage = aLanguage;        }
        void SetLoggedIn(bool aLoggedIn)            { mLoggedIn = aLoggedIn;        }

    private:

        bool        mFTUEEnabled;
        bool        mAutosaveEnabled;
        int         mAutosaveTime;
        int         mLogLevel;
        std::string mLanguage;
        bool        mLoggedIn;
    };
}
