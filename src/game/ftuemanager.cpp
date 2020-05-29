/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "engine/script.h"

#include "graphic/gui/theme.h"
#include "graphic/gui/button.h"
#include "graphic/gui/imageview.h"
#include "graphic/gui/label.h"
#include "graphic/gui/checkbox.h"

#include "userconfig.h"
#include "ftuemanager.h"

using namespace Framework;
using namespace LuaIntf;

namespace Game
{
    FTUEManager::FTUEManager()
        : BaseManager(Utils::Demangling(typeid(FTUEManager).name()))
        , mCurrentPageIndex(0)
        , mDontShowNextTime(false)
    {
        
    }

    FTUEManager::~FTUEManager()
    {

    }

    void FTUEManager::Initialize()
    {
        LuaContext& lLuaContext = Engine::Instance()->Script().GetLuaContext();
        LuaState lLuaState = lLuaContext.state();

        LuaBinding(lLuaState).addFunction("GetFTUEManager", [this]() -> FTUEManager&
        {
            string lManagerName = Utils::Demangling(typeid(FTUEManager).name());
            return static_cast<FTUEManager&>(Engine::Instance()->Manager(lManagerName));
        });

        LuaBinding(lLuaState).beginClass<FTUEManager>("FTUEManager")
            .addFunction("ShouldFTUEShow", &FTUEManager::ShouldFTUEShow)
            .addFunction("SaveFTUEPrefs", &FTUEManager::SaveFTUEPrefs)
            .addFunction("SetTheme", &FTUEManager::SetTheme)
            .addFunction("NextPage", &FTUEManager::NextPage)
            .addFunction("PrevPage", &FTUEManager::PrevPage)
            .addFunction("SetCurrentPage", &FTUEManager::SetCurrentPage)
            .addFunction("GetCurrentPageImage", &FTUEManager::GetCurrentPageImage)
            .addFunction("GetCurrentPageHeading", &FTUEManager::GetCurrentPageHeading)
            .addFunction("GetCurrentPageText", &FTUEManager::GetCurrentPageText)
            .endClass();
  
        Json::Value lSerializer;
        bool lIsOk = Json::JsonUtils::OpenAndParseJsonFromFile(lSerializer, "data/resources/ftue.json");

        if (!lIsOk)
        {
            WARNING("Cannot load ftue info from data/resources/ftue.json");
            return;
        }

        LoadFromJSON(lSerializer);
    }

    void FTUEManager::DeInitialize()
    {
        
    }

    bool FTUEManager::ShouldFTUEShow()
    {
        string lUserConfigName = Utils::Demangling(typeid(UserConfig).name());
        return static_cast<UserConfig&>(Engine::Instance()->Container(lUserConfigName)).GetFTUEEnabled();
    }

    void FTUEManager::SaveFTUEPrefs()
    {
        UICheckBox* lCheckBox = dynamic_cast<UICheckBox*>(Engine::Instance()->Display().findWidget("FTUEDontShowNextTimeBtn"));
        if (lCheckBox)
            mDontShowNextTime = lCheckBox->checked();

        string lUserConfigName = Utils::Demangling(typeid(UserConfig).name());
        static_cast<UserConfig&>(Engine::Instance()->Container(lUserConfigName)).SetFTUEEnabled(!mDontShowNextTime);
    }

    void FTUEManager::LoadFromJSON(const Json::Value& aSerializer)
    {
        const Json::Value& lPages = aSerializer["ftuepages"];
        ASSERT(lPages.isArray());
        for (size_t i = 0, lSize = lPages.size(); i < lSize; ++i)
        {
            const Json::Value lPageRes = lPages[static_cast<int>(i)];
            
            if (!lPageRes.isMember("image"))
                CRASH("FTUE Page has no image");
            string lImage = lPageRes["image"].asString();

            int lImageId = Engine::Instance()->ResourceManager().FindImage(lImage);
            if (lImageId < 0) 
                WARNING("Image %s cannot be loaded for FTUE page", lImage.c_str());

            if (!lPageRes.isMember("heading"))
                CRASH("FTUE Page has no heading");
            string lHeading = lPageRes["heading"].asString();

            if (!lPageRes.isMember("text"))
                CRASH("FTUE Page has no text");
            string lText = lPageRes["text"].asString();

            mPages.push_back(std::make_shared<FTUEPage>(lImageId, lHeading, lText));
        }
    }

    void FTUEManager::SetCurrentPage(int aPageIndex)
    {
        if (aPageIndex < 0 || aPageIndex >= mPages.size())
        {
            WARNING("Incorrect page index passed to SetCurrentPage");
            return;
        }

        mCurrentPageIndex = aPageIndex;

        for (int i = 0; i < mPages.size(); i++)
        {
            UIButton* lButton = dynamic_cast<UIButton*>(Engine::Instance()->Display().findWidget(string("FTUEPageBtn") + std::to_string(i+1)));
            if (lButton) 
                lButton->setTextColor(Color(255, 255, 255, 255));
        }
        
        UIButton* lButton = dynamic_cast<UIButton*>(Engine::Instance()->Display().findWidget(string("FTUEPageBtn") + std::to_string(mCurrentPageIndex+1)));
        if (lButton) 
            lButton->setTextColor(Color(0, 0, 0, 255));

        UIImageView* lImageView = dynamic_cast<UIImageView*>(Engine::Instance()->Display().findWidget("FTUEImage"));
        if (lImageView)
            lImageView->setImage(GetCurrentPageImage());

        UILabel* lHeading = dynamic_cast<UILabel*>(Engine::Instance()->Display().findWidget("FTUEHeading"));
        if (lHeading)
            lHeading->setCaption(GetCurrentPageHeading());

        UILabel* lText = dynamic_cast<UILabel*>(Engine::Instance()->Display().findWidget("FTUEText"));
        if (lText)
            lText->setCaption(GetCurrentPageText());
    }


    void FTUEManager::SetTheme()
    {
        //  We set the window theme only after the FTUE.ui has initialized. Hence we call this function from Lua
        //  after creating the FTUE window.
        UIWidget* lFTUEWindow = Engine::Instance()->Display().findWidget("wndFTUE");
        Framework::GUI::ref<UITheme> lTheme(new UITheme(*lFTUEWindow->theme()));
        lTheme->mWindowFillUnfocused = Color(lTheme->mWindowFillUnfocused.r(), lTheme->mWindowFillUnfocused.g(), lTheme->mWindowFillUnfocused.b(), 1.0f);
        lTheme->mWindowFillFocused = Color(lTheme->mWindowFillFocused.r(), lTheme->mWindowFillFocused.g(), lTheme->mWindowFillFocused.b(), 1.0f);
        lFTUEWindow->setTheme(lTheme);
    }

    void FTUEManager::NextPage()
    {
        if (mCurrentPageIndex < mPages.size() - 1)
            SetCurrentPage(++mCurrentPageIndex);
    }

    void FTUEManager::PrevPage()
    {
        if (mCurrentPageIndex > 0)
            SetCurrentPage(--mCurrentPageIndex);
    }

    int FTUEManager::GetCurrentPageImage()
    {
        ASSERT(mPages.size() > 0)
        return mPages.at(mCurrentPageIndex)->mImage;
    }

    std::string FTUEManager::GetCurrentPageHeading()
    {
        ASSERT(mPages.size() > 0)
        return mPages.at(mCurrentPageIndex)->mHeading;
    }

    std::string FTUEManager::GetCurrentPageText()
    {
        ASSERT(mPages.size() > 0)
        return mPages.at(mCurrentPageIndex)->mText;
    }
}
