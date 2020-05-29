/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "engine/script.h"
#include "graphic/gui/window.h"
#include "json/json.h"
#include "core/serialization/jsoncpputils.h"
#include "graphic/gui/colorpicker.h"
#include "graphic/gui/textBox.h"

#include "thememanager.h"

using namespace Framework;
using namespace LuaIntf;
using namespace std::placeholders;

namespace Game
{
    ThemeManager::ThemeManager()
        : BaseManager(Utils::Demangling(typeid(ThemeManager).name()))
    {
        mCustomTheme = nullptr;
        mSelectedElement = "";
    }

    ThemeManager::~ThemeManager()
    {

    }

    void ThemeManager::Initialize()
    {
        mCustomTheme = new CustomTheme(Engine::Instance()->Display().nvgContext());

        LuaContext& lLuaContext = Engine::Instance()->Script().GetLuaContext();
        LuaState lLuaState = lLuaContext.state();

        LuaBinding(lLuaState).addFunction("GetThemeManager", [this]() -> ThemeManager &
        {
            string lManagerName = Utils::Demangling(typeid(ThemeManager).name());
            return static_cast<ThemeManager&>(Engine::Instance()->Manager(lManagerName));
        });

        LuaBinding(lLuaState).beginClass<ThemeManager>("ThemeManager")
            .addFunction("SelectElement", &ThemeManager::SelectElement, LUA_ARGS(std::string))
            .addFunction("SetValue", &ThemeManager::SetValue, LUA_ARGS(std::string))
            .addFunction("ApplyTheme", &ThemeManager::ApplyTheme)
            .addFunction("SaveTheme", &ThemeManager::SaveTheme)
            .addFunction("LoadTheme", &ThemeManager::LoadTheme)
            .endClass();
    }

    void ThemeManager::DeInitialize()
    {
        
    }

    void ThemeManager::SelectElement(string aElement)
    {
        mSelectedElement = aElement;
    }

    void ThemeManager::SetValue(string aValue)
    {
        if (!mSelectedElement.compare("DropShadow")) mCustomTheme->mDropShadow = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("Transparent")) mCustomTheme->mTransparent = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("BorderDark")) mCustomTheme->mBorderDark = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("BorderLight")) mCustomTheme->mBorderLight = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("BorderMedium")) mCustomTheme->mBorderMedium = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("Text")) mCustomTheme->mTextColor = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("TextDisabled")) mCustomTheme->mDisabledTextColor = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("TextShadow")) mCustomTheme->mTextColorShadow = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("Icon")) mCustomTheme->mIconColor = UITheme::ParseColor(aValue);

        if (!mSelectedElement.compare("ButtonGradientTopFocused")) mCustomTheme->mButtonGradientTopFocused = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("ButtonGradientBottomFocused")) mCustomTheme->mButtonGradientBotFocused = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("ButtonGradientTopUnfocused")) mCustomTheme->mButtonGradientTopUnfocused = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("ButtonGradientBottomUnfocused")) mCustomTheme->mButtonGradientBotUnfocused = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("ButtonGradientTopPushed")) mCustomTheme->mButtonGradientTopPushed = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("ButtonGradientBottomPushed")) mCustomTheme->mButtonGradientBotPushed = UITheme::ParseColor(aValue);
        
        if (!mSelectedElement.compare("WindowFillFocused")) mCustomTheme->mWindowFillFocused = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("WindowFillUnfocused")) mCustomTheme->mWindowFillUnfocused = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("WindowTitleFocused")) mCustomTheme->mWindowTitleFocused = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("WindowTitleUnfocused")) mCustomTheme->mWindowTitleUnfocused = UITheme::ParseColor(aValue);

        if (!mSelectedElement.compare("WindowHeaderGradientTop")) mCustomTheme->mWindowHeaderGradientTop = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("WindowHeaderGradientBottom")) mCustomTheme->mWindowHeaderGradientBot = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("WindowHeaderSepTop")) mCustomTheme->mWindowHeaderSepTop = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("WindowHeaderSepBottom")) mCustomTheme->mWindowHeaderSepBot = UITheme::ParseColor(aValue);

        if (!mSelectedElement.compare("WindowPopup")) mCustomTheme->mWindowPopup = UITheme::ParseColor(aValue);
        if (!mSelectedElement.compare("WindowPopupTransparent")) mCustomTheme->mWindowPopupTransparent = UITheme::ParseColor(aValue);

        if (mSelectedElement.compare("StandardFontSize") == 0)   mCustomTheme->mStandardFontSize = atoi(aValue.c_str());
        if (mSelectedElement.compare("ButtonFontSize") == 0)   mCustomTheme->mButtonFontSize = atoi(aValue.c_str());
        if (mSelectedElement.compare("TextBoxFontSize") == 0)   mCustomTheme->mTextBoxFontSize = atoi(aValue.c_str());
        if (mSelectedElement.compare("IconScale") == 0)   mCustomTheme->mIconScale = (float)atof(aValue.c_str());
        
        if (mSelectedElement.compare("ButtonCornerRadius") == 0)   mCustomTheme->mButtonCornerRadius = atoi(aValue.c_str());
        if (mSelectedElement.compare("WindowCornerRadius") == 0)   mCustomTheme->mWindowCornerRadius = atoi(aValue.c_str());
        if (mSelectedElement.compare("WindowHeaderHeight") == 0)   mCustomTheme->mWindowHeaderHeight = atoi(aValue.c_str());
        if (mSelectedElement.compare("WindowDropShadowSize") == 0)   mCustomTheme->mWindowDropShadowSize = atoi(aValue.c_str());

        if (mSelectedElement.compare("LabelFont") == 0)   mCustomTheme->mLabelFont = aValue;
        if (mSelectedElement.compare("ButtonFont") == 0)   mCustomTheme->mButtonFont = aValue;
        if (mSelectedElement.compare("HeadingFont") == 0)   mCustomTheme->mHeadingFont = aValue;
        if (mSelectedElement.compare("TextboxFont") == 0)   mCustomTheme->mTextboxFont = aValue;
        if (mSelectedElement.compare("TabHeadingFont") == 0)   mCustomTheme->mTabHeadingFont = aValue;

        int lSetFont = nvgFindFont(Engine::Instance()->Display().nvgContext(), "aValue");
        if (lSetFont == -1)
        {
            //  The font current being set is not yet loaded into NanoGUI so we need to load the font now
            std::string lFontPath = "data/resources/fonts/" + aValue + ".ttf";
            int lNewFont = nvgCreateFont(Engine::Instance()->Display().nvgContext(), aValue.c_str(), lFontPath.c_str());
            if (lNewFont == -1)
            {
                WARNING("ThemeManager cannot load font %s", aValue.c_str());
            }
        }
    }

    void ThemeManager::ApplyTheme()
    {
        UIWindow* lThemeDisplayWindow = (UIWindow*)(Engine::Instance()->Display().findWidget("ThemeDisplayWindow"));
        if (lThemeDisplayWindow == nullptr){
            WARNING("Cannot get Widget ThemeDisplayWindow");
            return;
        }

        lThemeDisplayWindow->setTheme(mCustomTheme);

        ((UIColorPicker*)Engine::Instance()->Display().findWidget("DropShadowColorBtn"))->setColor(mCustomTheme->mDropShadow);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("TransparentColorBtn"))->setColor(mCustomTheme->mTransparent);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("BorderDarkColorBtn"))->setColor(mCustomTheme->mBorderDark);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("BorderLightColorBtn"))->setColor(mCustomTheme->mBorderLight);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("BorderMediumColorBtn"))->setColor(mCustomTheme->mBorderMedium);
        
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("TextColorBtn"))->setColor(mCustomTheme->mTextColor);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("TextDisabledColorBtn"))->setColor(mCustomTheme->mDisabledTextColor);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("TextShadowColorBtn"))->setColor(mCustomTheme->mTextColorShadow);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("IconColorBtn"))->setColor(mCustomTheme->mIconColor);
        
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("ButtonGradientTopFocusedColorBtn"))->setColor(mCustomTheme->mButtonGradientTopFocused);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("ButtonGradientBottomFocusedColorBtn"))->setColor(mCustomTheme->mButtonGradientBotFocused);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("ButtonGradientTopUnfocusedColorBtn"))->setColor(mCustomTheme->mButtonGradientTopUnfocused);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("ButtonGradientBottomUnfocusedColorBtn"))->setColor(mCustomTheme->mButtonGradientBotUnfocused);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("ButtonGradientTopPushedColorBtn"))->setColor(mCustomTheme->mButtonGradientTopPushed);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("ButtonGradientBottomPushedColorBtn"))->setColor(mCustomTheme->mButtonGradientBotPushed);

        ((UIColorPicker*)Engine::Instance()->Display().findWidget("WindowFillFocusedColorBtn"))->setColor(mCustomTheme->mWindowFillFocused);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("WindowFillUnfocusedColorBtn"))->setColor(mCustomTheme->mWindowFillUnfocused);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("WindowTitleFocusedColorBtn"))->setColor(mCustomTheme->mWindowTitleFocused);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("WindowTitleUnfocusedColorBtn"))->setColor(mCustomTheme->mWindowTitleUnfocused);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("WindowHeaderGradientTopColorBtn"))->setColor(mCustomTheme->mWindowHeaderGradientTop);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("WindowHeaderGradientBottomColorBtn"))->setColor(mCustomTheme->mWindowHeaderGradientBot);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("WindowHeaderSepTopColorBtn"))->setColor(mCustomTheme->mWindowHeaderSepTop);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("WindowHeaderSepBottomColorBtn"))->setColor(mCustomTheme->mWindowHeaderSepBot);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("WindowPopupColorBtn"))->setColor(mCustomTheme->mWindowPopup);
        ((UIColorPicker*)Engine::Instance()->Display().findWidget("WindowPopupTransparentColorBtn"))->setColor(mCustomTheme->mWindowPopupTransparent);

        ((UITextBox*)Engine::Instance()->Display().findWidget("StandardFontSizeValue"))->setValue(to_string(mCustomTheme->mStandardFontSize));
        ((UITextBox*)Engine::Instance()->Display().findWidget("ButtonFontSizeValue"))->setValue(to_string(mCustomTheme->mButtonFontSize));
        ((UITextBox*)Engine::Instance()->Display().findWidget("TextBoxFontSizeValue"))->setValue(to_string(mCustomTheme->mTextBoxFontSize));
        ((UITextBox*)Engine::Instance()->Display().findWidget("IconScaleValue"))->setValue(to_string(mCustomTheme->mIconScale));
        ((UITextBox*)Engine::Instance()->Display().findWidget("ButtonCornerRadiusValue"))->setValue(to_string(mCustomTheme->mButtonCornerRadius));
        ((UITextBox*)Engine::Instance()->Display().findWidget("WindowCornerRadiusValue"))->setValue(to_string(mCustomTheme->mWindowCornerRadius));
        ((UITextBox*)Engine::Instance()->Display().findWidget("WindowHeaderHeightValue"))->setValue(to_string(mCustomTheme->mWindowHeaderHeight));
        ((UITextBox*)Engine::Instance()->Display().findWidget("WindowDropShadowSizeValue"))->setValue(to_string(mCustomTheme->mWindowDropShadowSize));
        
        ((UITextBox*)Engine::Instance()->Display().findWidget("LabelFontValue"))->setValue(mCustomTheme->mLabelFont);
        ((UITextBox*)Engine::Instance()->Display().findWidget("ButtonFontValue"))->setValue(mCustomTheme->mButtonFont);
        ((UITextBox*)Engine::Instance()->Display().findWidget("HeadingFontValue"))->setValue(mCustomTheme->mHeadingFont);
        ((UITextBox*)Engine::Instance()->Display().findWidget("TextboxFontValue"))->setValue(mCustomTheme->mTextboxFont);
        ((UITextBox*)Engine::Instance()->Display().findWidget("TabHeadingFontValue"))->setValue(mCustomTheme->mTabHeadingFont);
    }

    void ThemeManager::SaveTheme()
    {
        char buf[1024];
        GetCurrentDirectoryA(256, buf);
        std::string lFileName = file_dialog({ {"theme", "Theme files"}, {"*", "All files"} }, true);
        SetCurrentDirectoryA(buf);

        if (lFileName.empty())
            return;

        if (lFileName.find(".theme") == std::string::npos)
            lFileName = lFileName + ".theme";
         
        Json::Value lSerializer;
  
        mCustomTheme->Serialize(lSerializer);
 
        Json::StyledWriter writer;
        string lOutput = writer.write(lSerializer);
        fstream lFileOut(lFileName, ios_base::out);
        lFileOut << lOutput;
    }

    void ThemeManager::LoadTheme()
    {
        char buf[1024];
        GetCurrentDirectoryA(256, buf);
        std::string lFileName = file_dialog({ {"theme", "Theme files"} }, false);
        SetCurrentDirectoryA(buf);

        if (!lFileName.empty())
        {
            mCustomTheme = new UITheme(Engine::Instance()->Display().nvgContext(), lFileName);
            ApplyTheme();
        }
    }
}
