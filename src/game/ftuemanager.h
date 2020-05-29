/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once

#include "core/utils.h"
#include "engine/basemanager.h"

namespace Game
{
    class FTUEPage
    {
    public:
        int mImage;
        std::string mHeading;
        std::string mText;

        FTUEPage(int aImage, std::string aHeading, std::string aText) :
              mImage(aImage)
            , mHeading(aHeading)
            , mText(aText)
        {
            
        }

        ~FTUEPage()
        {

        }
    };

    class FTUEManager : public Framework::BaseManager
    {
    public:

        FTUEManager();
        ~FTUEManager() override;

        void            Initialize() override;
        void            DeInitialize() override;

        void            LoadFromJSON(const Json::Value& aSerializer);
        bool            ShouldFTUEShow();
        void            SaveFTUEPrefs();
        void            SetTheme();
        void            NextPage();
        void            PrevPage();
        void            SetCurrentPage(int aPageIndex);

        int             GetCurrentPageImage();
        std::string     GetCurrentPageHeading();
        std::string     GetCurrentPageText();
        void            ToggleDontShowNextTime();

    protected:

        bool            mDontShowNextTime;
        int             mCurrentPageIndex;
        std::vector<std::shared_ptr<FTUEPage>> mPages;
    };
}
