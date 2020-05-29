/*
src/theme.cpp -- Storage class for basic theme-related properties
The text box widget was contributed by Christian Schueller.
NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
The widget drawing code is based on the NanoVG demo application
by Mikko Mononen.
All rights reserved. Use of this source code is governed by a
BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"
#include "graphic/gui/theme.h"
#include "graphic/gui/entypo.h"
#include "graphic/gui/nanogui_resources.h"


namespace Framework
{
    namespace GUI
    {
        Color UITheme::ParseColor(std::string aColorString)
        {
            std::stringstream lColorStream(aColorString);
            std::string lTempString;

            getline(lColorStream, lTempString, ',');
            float r = atof(lTempString.c_str());

            getline(lColorStream, lTempString, ',');
            float g = atof(lTempString.c_str());

            getline(lColorStream, lTempString, ',');
            float b = atof(lTempString.c_str());

            getline(lColorStream, lTempString, ',');
            float a = atof(lTempString.c_str());

            return Color(r, g, b, a);
        }

        std::string UITheme::SerializeColor(Color aColor)
        {
            return to_string(aColor.r()) + "," + to_string(aColor.g()) + "," + to_string(aColor.b()) + "," + to_string(aColor.a());
        }

        UITheme::UITheme(NVGcontext *ctx)
        {
            mStandardFontSize = 16;
            mButtonFontSize = 20;
            mTextBoxFontSize = 20;
            mIconScale = 0.77f;

            mWindowCornerRadius = 2;
            mWindowHeaderHeight = 30;
            mWindowDropShadowSize = 10;
            mButtonCornerRadius = 2;
            mTabBorderWidth = 0.75f;
            mTabInnerMargin = 5;
            mTabMinButtonWidth = 56;
            mTabMaxButtonWidth = 160;
            mTabControlWidth = 20;
            mTabButtonHorizontalPadding = 10;
            mTabButtonVerticalPadding = 2;

            mDropShadow = Color(0, 128);
            mTransparent = Color(0, 0);
            mBorderDark = Color(29, 255);
            mBorderLight = Color(92, 255);
            mBorderMedium = Color(35, 255);
            mTextColor = Color(255, 160);
            mDisabledTextColor = Color(255, 80);
            mTextColorShadow = Color(0, 160);
            mIconColor = mTextColor;

            mButtonGradientTopFocused = Color(64, 255);
            mButtonGradientBotFocused = Color(48, 255);
            mButtonGradientTopUnfocused = Color(74, 255);
            mButtonGradientBotUnfocused = Color(58, 255);
            mButtonGradientTopPushed = Color(41, 255);
            mButtonGradientBotPushed = Color(29, 255);

            /* Window-related */
            mWindowFillUnfocused = Color(43, 230);
            mWindowFillFocused = Color(45, 230);
            mWindowTitleUnfocused = Color(220, 160);
            mWindowTitleFocused = Color(255, 190);

            mWindowHeaderGradientTop = mButtonGradientTopUnfocused;
            mWindowHeaderGradientBot = mButtonGradientBotUnfocused;
            mWindowHeaderSepTop = mBorderLight;
            mWindowHeaderSepBot = mBorderDark;

            mWindowPopup = Color(50, 255);
            mWindowPopupTransparent = Color(50, 0);

            mCheckBoxIcon = ENTYPO_ICON_CHECK;
            mMessageInformationIcon = ENTYPO_ICON_INFO_WITH_CIRCLE;
            mMessageQuestionIcon = ENTYPO_ICON_HELP_WITH_CIRCLE;
            mMessageWarningIcon = ENTYPO_ICON_WARNING;
            mMessageAltButtonIcon = ENTYPO_ICON_CIRCLE_WITH_CROSS;
            mMessagePrimaryButtonIcon = ENTYPO_ICON_CHECK;
            mPopupChevronRightIcon = ENTYPO_ICON_CHEVRON_RIGHT;
            mPopupChevronLeftIcon = ENTYPO_ICON_CHEVRON_LEFT;
            mTabHeaderLeftIcon = ENTYPO_ICON_ARROW_BOLD_LEFT;
            mTabHeaderRightIcon = ENTYPO_ICON_ARROW_BOLD_RIGHT;
            mTextBoxUpIcon = ENTYPO_ICON_CHEVRON_UP;
            mTextBoxDownIcon = ENTYPO_ICON_CHEVRON_DOWN;

            mFontNormal = nvgCreateFontMem(ctx, "sans", roboto_regular_ttf, roboto_regular_ttf_size, 0);
            mFontBold = nvgCreateFontMem(ctx, "sans-bold", roboto_bold_ttf, roboto_bold_ttf_size, 0);
            mFontIcons = nvgCreateFontMem(ctx, "icons", entypo_ttf, entypo_ttf_size, 0);

            if (mFontNormal == -1 || mFontBold == -1 || mFontIcons == -1)
                throw std::runtime_error("Could not load fonts!");

            mLabelFont = "sans";
            mButtonFont = "sans-bold";
            mHeadingFont = "sans-bold";
            mTextboxFont = "sans";
            mTabHeadingFont = "sans-bold";
        }

        UITheme::UITheme(NVGcontext* ctx, std::string aThemeFile) : UITheme(ctx)
        {
            Json::Value lSerializer;
            bool lIsOk = Json::JsonUtils::OpenAndParseJsonFromFile(lSerializer, aThemeFile);
            if (lIsOk) 
            {
                Deserialize(lSerializer);
            }
        }
        
        void UITheme::Serialize(Json::Value& aSerializer)
        {
            aSerializer["StandardFontSize"] = mStandardFontSize;

            aSerializer["ButtonFontSize"] = mButtonFontSize;
            aSerializer["TextBoxFontSize"] = mTextBoxFontSize;
            aSerializer["IconScale"] = mIconScale;

            aSerializer["WindowCornerRadius"] = mWindowCornerRadius;
            aSerializer["WindowHeaderHeight"] = mWindowHeaderHeight;
            aSerializer["WindowDropShadowSize"] = mWindowDropShadowSize;
            aSerializer["ButtonCornerRadius"] = mButtonCornerRadius;

            aSerializer["TabBorderWidth"] = mTabBorderWidth;
            aSerializer["TabInnerMargin"] = mTabInnerMargin;
            aSerializer["TabMinButtonWidth"] = mTabBorderWidth;
            aSerializer["TabMaxButtonWidth"] = mTabMaxButtonWidth;
            aSerializer["TabControlWidth"] = mTabControlWidth;
            aSerializer["TabButtonHorizontalPadding"] = mTabButtonHorizontalPadding;
            aSerializer["TabButtonVerticalPadding"] = mTabButtonVerticalPadding;

            aSerializer["DropShadow"] = SerializeColor(mDropShadow);
            aSerializer["Transparent"] = SerializeColor(mTransparent);
            aSerializer["BorderDark"] = SerializeColor(mBorderDark);
            aSerializer["BorderLight"] = SerializeColor(mBorderLight);
            aSerializer["BorderMedium"] = SerializeColor(mBorderMedium);
            aSerializer["TextColor"] = SerializeColor(mTextColor);
            aSerializer["DisabledTextColor"] = SerializeColor(mDisabledTextColor);
            aSerializer["TextColorShadow"] = SerializeColor(mTextColorShadow);
            aSerializer["IconColor"] = SerializeColor(mIconColor);

            aSerializer["ButtonGradientTopFocused"] = SerializeColor(mButtonGradientTopFocused);
            aSerializer["ButtonGradientBotFocused"] = SerializeColor(mButtonGradientBotFocused);
            aSerializer["ButtonGradientTopUnfocused"] = SerializeColor(mButtonGradientTopUnfocused);
            aSerializer["ButtonGradientBotUnfocused"] = SerializeColor(mButtonGradientBotUnfocused);
            aSerializer["ButtonGradientTopPushed"] = SerializeColor(mButtonGradientTopPushed);
            aSerializer["ButtonGradientBotPushed"] = SerializeColor(mButtonGradientBotPushed);

            /* Window-related */
            aSerializer["WindowFillUnfocused"] = SerializeColor(mWindowFillUnfocused);
            aSerializer["WindowFillFocused"] = SerializeColor(mWindowFillFocused);
            aSerializer["WindowTitleUnfocused"] = SerializeColor(mWindowTitleUnfocused);
            aSerializer["WindowTitleFocused"] = SerializeColor(mWindowTitleFocused);

            aSerializer["WindowHeaderGradientTop"] = SerializeColor(mWindowHeaderGradientTop);
            aSerializer["WindowHeaderGradientBot"] = SerializeColor(mWindowHeaderGradientBot);
            aSerializer["WindowHeaderSepTop"] = SerializeColor(mWindowHeaderSepTop);
            aSerializer["WindowHeaderSepBot"] = SerializeColor(mWindowHeaderSepBot);

            aSerializer["WindowPopup"] = SerializeColor(mWindowPopup);
            aSerializer["WindowPopupTransparent"] = SerializeColor(mWindowPopupTransparent);

            aSerializer["LabelFont"] = mLabelFont;
            aSerializer["ButtonFont"] = mButtonFont;
            aSerializer["HeadingFont"] = mHeadingFont;
            aSerializer["TextboxFont"] = mTextboxFont;
            aSerializer["TabHeadingFont"] = mTabHeadingFont;
        }

        void UITheme::Deserialize(const Json::Value& aSerializer)
        {
            mStandardFontSize = aSerializer.get("StandardFontSize", mStandardFontSize).asInt();
            mButtonFontSize = aSerializer.get("ButtonFontSize", mButtonFontSize).asInt();
            mTextBoxFontSize = aSerializer.get("TextBoxFontSize", mTextBoxFontSize).asInt();
            mIconScale = aSerializer.get("IconScale", mIconScale).asFloat();

            mWindowCornerRadius = aSerializer.get("WindowCornerRadius", mWindowCornerRadius).asInt();
            mWindowHeaderHeight = aSerializer.get("WindowHeaderHeight", mWindowHeaderHeight).asInt();
            mWindowDropShadowSize = aSerializer.get("WindowDropShadowSize", mWindowDropShadowSize).asInt();
            mButtonCornerRadius = aSerializer.get("ButtonCornerRadius", mButtonCornerRadius).asInt();

            mTabBorderWidth = aSerializer.get("TabBorderWidth", mTabBorderWidth).asInt();
            mTabInnerMargin = aSerializer.get("TabInnerMargin", mTabInnerMargin).asInt();
            mTabMinButtonWidth = aSerializer.get("TabMinButtonWidth", mTabMinButtonWidth).asInt();
            mTabMaxButtonWidth = aSerializer.get("TabMaxButtonWidth", mTabMaxButtonWidth).asInt();
            mTabControlWidth = aSerializer.get("TabControlWidth", mTabControlWidth).asInt();
            mTabButtonHorizontalPadding = aSerializer.get("TabButtonHorizontalPadding", mTabButtonHorizontalPadding).asInt();
            mTabButtonVerticalPadding = aSerializer.get("TabButtonVerticalPadding", mTabButtonVerticalPadding).asInt();
            
            mDropShadow = ParseColor(aSerializer.get("DropShadow", SerializeColor(mDropShadow)).asString());
            mTransparent = ParseColor(aSerializer.get("Transparent", SerializeColor(mTransparent)).asString());
            mBorderDark = ParseColor(aSerializer.get("BorderDark", SerializeColor(mBorderDark)).asString());
            mBorderLight = ParseColor(aSerializer.get("BorderLight", SerializeColor(mBorderLight)).asString());
            mBorderMedium = ParseColor(aSerializer.get("BorderMedium", SerializeColor(mBorderMedium)).asString());

            mTextColor = ParseColor(aSerializer.get("TextColor", SerializeColor(mTextColor)).asString());
            mDisabledTextColor = ParseColor(aSerializer.get("DisabledTextColor", SerializeColor(mDisabledTextColor)).asString());
            mTextColorShadow = ParseColor(aSerializer.get("TextColorShadow", SerializeColor(mTextColorShadow)).asString());
            mIconColor = ParseColor(aSerializer.get("IconColor", SerializeColor(mIconColor)).asString());

            mButtonGradientTopFocused = ParseColor(aSerializer.get("ButtonGradientTopFocused", SerializeColor(mButtonGradientTopFocused)).asString());
            mButtonGradientBotFocused = ParseColor(aSerializer.get("ButtonGradientBotFocused", SerializeColor(mButtonGradientBotFocused)).asString());
            mButtonGradientTopUnfocused = ParseColor(aSerializer.get("ButtonGradientTopUnfocused", SerializeColor(mButtonGradientTopUnfocused)).asString());
            mButtonGradientBotUnfocused = ParseColor(aSerializer.get("ButtonGradientBotUnfocused", SerializeColor(mButtonGradientBotUnfocused)).asString());
            mButtonGradientTopPushed = ParseColor(aSerializer.get("ButtonGradientTopPushed", SerializeColor(mButtonGradientTopPushed)).asString());
            mButtonGradientBotPushed = ParseColor(aSerializer.get("ButtonGradientBotPushed", SerializeColor(mButtonGradientBotPushed)).asString());

            // Window-related 
            mWindowFillUnfocused = ParseColor(aSerializer.get("WindowFillUnfocused", SerializeColor(mWindowFillUnfocused)).asString());
            mWindowFillFocused = ParseColor(aSerializer.get("WindowFillFocused", SerializeColor(mWindowFillFocused)).asString());
            mWindowTitleUnfocused = ParseColor(aSerializer.get("WindowTitleUnfocused", SerializeColor(mWindowTitleUnfocused)).asString());
            mWindowTitleFocused = ParseColor(aSerializer.get("WindowTitleFocused", SerializeColor(mWindowTitleFocused)).asString());

            mWindowHeaderGradientTop = ParseColor(aSerializer.get("WindowHeaderGradientTop", SerializeColor(mWindowHeaderGradientTop)).asString());
            mWindowHeaderGradientBot = ParseColor(aSerializer.get("WindowHeaderGradientBot", SerializeColor(mWindowHeaderGradientBot)).asString());
            mWindowHeaderSepTop = ParseColor(aSerializer.get("WindowHeaderSepTop", SerializeColor(mWindowHeaderSepTop)).asString());
            mWindowHeaderSepBot = ParseColor(aSerializer.get("WindowHeaderSepBot", SerializeColor(mWindowHeaderSepBot)).asString());

            mWindowPopup = ParseColor(aSerializer.get("WindowPopup", SerializeColor(mWindowPopup)).asString());
            mWindowPopupTransparent = ParseColor(aSerializer.get("WindowPopupTransparent", SerializeColor(mWindowPopupTransparent)).asString());

            mLabelFont = aSerializer.get("LabelFont", mLabelFont).asString();
            mButtonFont = aSerializer.get("ButtonFont", mButtonFont).asString();
            mHeadingFont = aSerializer.get("HeadingFont", mHeadingFont).asString();
            mTextboxFont = aSerializer.get("TextboxFont", mTextboxFont).asString();
            mTabHeadingFont = aSerializer.get("TabHeadingFont", mTabHeadingFont).asString();
        }

        CustomTheme::CustomTheme(NVGcontext* ctx) : UITheme(ctx)
        {
            // two additional fonts loaded that are not defaults, but can be used
            // independently e.g. new Label(parent, "label", "spirax")
            mSpiraxFont = nvgCreateFont(ctx, "spirax", "data/resources/fonts/spirax.ttf");
            mMembraFont = nvgCreateFont(ctx, "membra", "data/resources/fonts/membra.ttf");

            if (mSpiraxFont == -1 || mMembraFont == -1)
                throw std::runtime_error("Could not load the `spirax` or `membra` fonts!");

            // load the default fonts for this theme
            mSpectralFont = nvgCreateFont(ctx, "spectral", "data/resources/fonts/spectral.ttf");
            mSpectralBoldFont = nvgCreateFont(ctx, "spectral-bold", "data/resources/fonts/spectral-bold.ttf");

            if (mSpectralFont == -1 || mSpectralBoldFont == -1)
                throw std::runtime_error("Could not load the `spectral` fonts!");

            mFontNormal = mSpectralFont;
            mFontBold = mSpectralBoldFont;

            mLabelFont = "spectral";
            mButtonFont = "spectral-bold";
            mHeadingFont = "spectral-bold";
            mTextboxFont = "spectral";
            mTabHeadingFont = "spectral-bold";

            // overriding some default icons as demonstration
            mCheckBoxIcon = ENTYPO_ICON_CROSS;                          // default: ENTYPO_ICON_CHECK
            mPopupChevronRightIcon = ENTYPO_ICON_TRIANGLE_RIGHT;        // default: ENTYPO_ICON_CHEVRON_RIGHT
            mPopupChevronLeftIcon = ENTYPO_ICON_TRIANGLE_LEFT;          // default: ENTYPO_ICON_CHEVRON_LEFT
            mTabHeaderLeftIcon = ENTYPO_ICON_ARROW_WITH_CIRCLE_LEFT;    // default: ENTYPO_ICON_ARROW_BOLD_LEFT
            mTabHeaderRightIcon = ENTYPO_ICON_ARROW_WITH_CIRCLE_RIGHT;  // default: ENTYPO_ICON_ARROW_BOLD_RIGHT
            mTextBoxUpIcon = ENTYPO_ICON_TRIANGLE_UP;                   // default: ENTYPO_ICON_CHEVRON_UP
            mTextBoxDownIcon = ENTYPO_ICON_TRIANGLE_DOWN;               // default: ENTYPO_ICON_CHEVRON_DOWN

            mDropShadow = Color(0.0f, 0.16862745583057404f, 0.21176475286483765f, 1.0f);
            mTransparent = Color(0.0f, 0.0f, 0.0f, 0.0f);
            mBorderDark = Color(0.4627450406551361f, 0.5333333015441895f, 0.5568627119064331f, 0.9019607901573181f);
            mBorderLight = Color(0.6431373953819275f, 0.6901960968971252f, 0.7058823108673096f, 0.9019607901573181f);
            mBorderMedium = Color(0.564706027507782f, 0.6196078658103943f, 0.6392157077789307f, 0.9019607901573181f);
            mTextColor = Color(0.34117648005485535f, 0.42745086550712585f, 0.4509802758693695f, 1.0f);
            mDisabledTextColor = Color(0.6078431010246277f, 0.658823549747467f, 0.6745098233222961f, 0.9019607901573181f);
            mTextColorShadow = Color(0.9333333373069763f, 0.9098039269447327f, 0.8352941870689392f, 0.9019607901573181f);
            mIconColor = Color(0.34117648005485535f, 0.42745086550712585f, 0.4509802758693695f, 1.0f);

            mButtonGradientTopFocused = Color(0.7576307058334351f, 0.7828220129013062f, 0.789692223072052f, 1.0f);
            mButtonGradientBotFocused = Color(0.6495736837387085f, 0.671172022819519f, 0.6770622730255127f, 1.0f);
            mButtonGradientTopUnfocused = Color(0.7975060343742371f, 0.8240231871604919f, 0.8312549591064453f, 1.0f);
            mButtonGradientBotUnfocused = Color(0.683761715888977f, 0.7064968347549438f, 0.7126971483230591f, 1.0f);
            mButtonGradientTopPushed = Color(0.6495736241340637f, 0.671172022819519f, 0.6770622730255127f, 1.0f);
            mButtonGradientBotPushed = Color(0.5569282174110413f, 0.5754461288452148f, 0.5804961919784546f, 1.0f);

            /* Window-related */
            mWindowFillUnfocused = Color(0.8862745761871338f, 0.8627451062202454f, 0.7921568155288696f, 0.9019607901573181f);
            mWindowFillFocused = Color(0.9333333373069763f, 0.9098039269447327f, 0.8352941870689392f, 0.9019607901573181f);
            mWindowTitleUnfocused = Color(0.654053807258606f, 0.6819284558296204f, 0.6852078437805176f, 1.0f);
            mWindowTitleFocused = Color(0.7628562450408936f, 0.7953678369522095f, 0.7991926670074463f, 1.0f);

            mWindowHeaderGradientTop = Color(0.16616077721118927f, 0.3241867423057556f, 0.3645337224006653f, 0.9019607901573181f);
            mWindowHeaderGradientBot = Color(0.027450978755950928f, 0.21176469326019287f, 0.2588234543800354f, 0.9019607901573181f);
            mWindowHeaderSepTop = Color(0.28508710861206055f, 0.4205746054649353f, 0.45516711473464966f, 0.9019607901573181f);
            mWindowHeaderSepBot = Color(0.0f, 0.16862745583057404f, 0.21176475286483765f, 1.0f);

            mWindowPopup = Color(0.9333333373069763f, 0.9098039269447327f, 0.8352941870689392f, 0.9019607901573181f);
            mWindowPopupTransparent = Color(0.19607843458652496f, 0.19607843458652496f, 0.19607843458652496f, 0.0f);
        }
    }
}
