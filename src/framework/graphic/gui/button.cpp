/*
src/button.cpp -- [Normal/Toggle/Radio/Popup] Button widget

NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
The widget drawing code is based on the NanoVG demo application
by Mikko Mononen.

All rights reserved. Use of this source code is governed by a
BSD-style license that can be found in the LICENSE.txt file.
*/
#include "precompiled.h"

#include "engine.h"
#include "engine/fsm/statemachine.h"

#include "json/json.h"
#include "graphic/gui/button.h"
#include "graphic/gui/theme.h"
#include "graphic/gui/buttontype.h"

namespace Framework
{
    namespace GUI
    {

        UIButton::UIButton()
            : UIWidget(nullptr)
            , mCaption("")
            , mCaptionAlignment(Alignment::eCenter)
            , mCaptionMargin(0)
            , mIcon(0)
            , mIconPosition(IconPosition::eLeftCentered)
            , mPushed(false)
            , mFlags(ButtonTypeToInt(ButtonType::eNormalButton))
            , mBackgroundColor(Color(0, 0))
            , mTextColor(Color(0, 0))
            , mBackgroundVisible(true)
        {

        }

        UIButton::UIButton(UIWidget *aParent, const std::string &aCaption, int aIcon)
            : UIWidget(aParent)
            , mCaptionAlignment(Alignment::eCenter)
            , mCaptionMargin(0)
            , mIcon(aIcon)
            , mIconPosition(IconPosition::eLeftCentered)
            , mPushed(false)
            , mFlags(ButtonTypeToInt(ButtonType::eNormalButton))
            , mBackgroundColor(Color(0, 0))
            , mTextColor(Color(0, 0))
            , mBackgroundVisible(true)
        {
            string lManagerName = Utils::Demangling(typeid(LanguageManager).name());
            LanguageManager& lLanguageManager = static_cast<LanguageManager&>(Engine::Instance()->Manager(lManagerName));
            mCaption = lLanguageManager.GetString(aCaption);
        }

        glm::ivec2 UIButton::preferredSize(NVGcontext *ctx) const
        {
            int fontSize = mFontSize == -1 ? mTheme->mButtonFontSize : mFontSize;
            nvgFontSize(ctx, fontSize);
            nvgFontFace(ctx, mTheme->mButtonFont.c_str());
            float tw = nvgTextBounds(ctx, 0, 0, mCaption.c_str(), nullptr, nullptr);
            float iw = 0.0f, ih = fontSize;

            if (mIcon)
            {
                if (nvgIsFontIcon(mIcon))
                {
                    ih *= 1.5f;
                    nvgFontFace(ctx, "icons");
                    nvgFontSize(ctx, ih);
                    iw = nvgTextBounds(ctx, 0, 0, utf8(mIcon).data(), nullptr, nullptr)
                        + mSize.y * 0.15f;
                }
                else
                {
                    int w, h;
                    ih *= 0.9f;
                    nvgImageSize(ctx, mIcon, &w, &h);
                    iw = w * ih / h;
                }
            }
            return glm::ivec2((int)(tw + iw) + 20, fontSize + 10);
        }

        bool UIButton::mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers)
        {
            UIWidget::mouseButtonEvent(p, button, down, modifiers);
            /* Temporarily increase the reference count of the button in case the
            button causes the parent window to be destructed */
            ref<UIButton> self = this;

            if (button == GLFW_MOUSE_BUTTON_1 && mEnabled)
            {
                bool lPushedBackup = mPushed;
                if (down)
                {
                    //Normal button
                    if (mFlags & ButtonTypeToInt(ButtonType::eNormalButton)) 
                        mPushed = true;

                    //Toogle button
                    if (mFlags & ButtonTypeToInt(ButtonType::eToggleButton))
                        mPushed = !mPushed;
                    
                    //Radio button
                    if (mFlags & ButtonTypeToInt(ButtonType::eRadioButton)) 
                    {
                        if (mButtonGroup.empty()) 
                        {
                            for (auto widget : parent()->children()) {
                                UIButton *b = dynamic_cast<UIButton *>(widget);
                                if (b != this && b && (b->flags() & ButtonTypeToInt(ButtonType::eRadioButton)) && b->mPushed)
                                {
                                    b->mPushed = false;
                                    if (b->mChangeCallback)
                                        b->mChangeCallback(false);
                                }
                            }
                        }
                        else 
                        {
                            for (auto b : mButtonGroup)
                            {
                                if (b != this && (b->flags() & ButtonTypeToInt(ButtonType::eRadioButton)) && b->mPushed)
                                {
                                    b->mPushed = false;
                                    if (b->mChangeCallback)
                                        b->mChangeCallback(false);
                                }
                            }
                        }
                        mPushed = true;
                    }
                    //Popup button
                    if (mFlags & ButtonTypeToInt(ButtonType::ePopupButton))
                    {
                        for (auto widget : parent()->children())
                        {
                            UIButton *b = dynamic_cast<UIButton *>(widget);
                            if (b != this && b && (b->flags() & ButtonTypeToInt(ButtonType::ePopupButton)) && b->mPushed)
                            {
                                b->mPushed = false;
                                if (b->mChangeCallback)
                                    b->mChangeCallback(false);
                            }
                        }
                        mPushed = true;
                    }
                }
                else if (mPushed)
                {
                    if (contains(p) && mCallback)
                        mCallback();
                    if (mFlags & ButtonTypeToInt(ButtonType::eNormalButton))
                        mPushed = false;
                }

                if (lPushedBackup != mPushed)
                {
                    //Button callback
                    if (mChangeCallback)
                        mChangeCallback(mPushed);

                    //Radio button -> Launch the state machine action at PRESSED
                    if(mFlags & ButtonTypeToInt(ButtonType::eRadioButton) && !mActionCmd.empty())
                        Engine::Instance()->StateMachine().ExecuteAction(mActionCmd);

                    //Popup button -> Launch the state machine action at PRESSED
                    else if (mFlags & ButtonTypeToInt(ButtonType::ePopupButton) && !mActionCmd.empty())
                        Engine::Instance()->StateMachine().ExecuteAction(mActionCmd);

                    //Normal button -> Launch the state machine action at RELEASE
                    else if (mFlags & ButtonTypeToInt(ButtonType::eNormalButton) && lPushedBackup && !mPushed && !mActionCmd.empty())
                        Engine::Instance()->StateMachine().ExecuteAction(mActionCmd);

                    //Toogle button -> Launch the state machine action at PRESSED
                    else if (mFlags & ButtonTypeToInt(ButtonType::eToggleButton) && !mActionCmd.empty())
                        Engine::Instance()->StateMachine().ExecuteAction(mActionCmd);

                }
                return true;
            }
            return false;
        }

        void UIButton::draw(NVGcontext *ctx)
        {
            UIWidget::draw(ctx);

            NVGcolor lGradTop = mTheme->mButtonGradientTopUnfocused;
            NVGcolor lGradBot = mTheme->mButtonGradientBotUnfocused;

            if (mPushed)
            {
                lGradTop = mTheme->mButtonGradientTopPushed;
                lGradBot = mTheme->mButtonGradientBotPushed;
            }
            else if (mMouseFocus && mEnabled)
            {
                lGradTop = mTheme->mButtonGradientTopFocused;
                lGradBot = mTheme->mButtonGradientBotFocused;
            }

            if (mBackgroundVisible)
                drawBackground(ctx, lGradTop, lGradBot);

            int fontSize = mFontSize == -1 ? mTheme->mButtonFontSize : mFontSize;
            nvgFontSize(ctx, fontSize);
            nvgFontFace(ctx, mTheme->mButtonFont.c_str());
            float tw = nvgTextBounds(ctx, 0, 0, mCaption.c_str(), nullptr, nullptr);

            NVGcolor textColor = mTextColor.a() == 0 ? mTheme->mTextColor : mTextColor;
            if (!mEnabled)
                textColor = mTheme->mDisabledTextColor;

            glm::vec2 textPos;
            glm::vec2 center = glm::vec2(mPos.x, mPos.y) + glm::vec2(mSize.x, mSize.y) * 0.5f;

            //Text alignment
            switch (mCaptionAlignment)
            {
                case Alignment::eCenter:
                    textPos = glm::vec2(center.x - tw * 0.5f, center.y - 1);
                    break;

                case Alignment::eLeft:
                    textPos = glm::vec2(mPos.x + mCaptionMargin, center.y - 1);
                    break;

                case Alignment::eRight:
                    textPos = glm::vec2(mPos.x + mSize.x - tw - mCaptionMargin, center.y - 1);
                    break;
                default:
                    WARNING("Invalid caption aligment value[%s]", AlignmentToString(mCaptionAlignment));
                    textPos = glm::vec2(center.x - tw * 0.5f, center.y - 1);
                    break;
            }

            glm::vec2 iconPos = center;
            iconPos.y -= 1.0f;

            if (nvgIsFontIcon(mIcon))
            {
                auto icon = utf8(mIcon);
                float ih = fontSize * 1.5f;
                float iw = 0.f;

                nvgFontSize(ctx, ih);
                nvgFontFace(ctx, "icons");
                nvgFillColor(ctx, textColor);
                iw = nvgTextBounds(ctx, 0, 0, icon.data(), nullptr, nullptr);
                nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);

                if (!mCaption.empty())
                    iw += mSize.y * 0.15f;

                switch (mIconPosition)
                {
                    case IconPosition::eLeftCentered:
                        iconPos.x -= (tw + iw) * 0.5f;
                        textPos.x += iw * 0.5f;
                        break;
                    case IconPosition::eRightCentered:
                        textPos.x -= iw * 0.5f;
                        iconPos.x += tw * 0.5f;
                        break;
                    case IconPosition::eLeft:
                        iconPos.x = mPos.x + 8;
                        break;
                    case IconPosition::eRight:
                        iconPos.x = mPos.x + mSize.x - iw - 8;
                }
                nvgText(ctx, iconPos.x, iconPos.y + 1, icon.data(), nullptr);
            }

            else if (nvgIsImageIcon(mIcon))
            {
                int w, h;
                nvgImageSize(ctx, mIcon, &w, &h);
                float ih = fontSize * 0.9f,
                    iw = w * ih / h;
                switch (mIconPosition)
                {
                case IconPosition::eLeftCentered:
                    iconPos.x -= (tw + iw) * 0.5f;
                    break;
                case IconPosition::eRightCentered:
                    iconPos.x += tw * 0.5f;
                    break;
                case IconPosition::eLeft:
                    iconPos.x = mPos.x + 8;
                    break;
                case IconPosition::eRight:
                    iconPos.x = mPos.x + mSize.x - iw - 8;
                }
                nvgBeginPath(ctx);
                nvgRoundedRect(ctx, mPos.x + 1, mPos.y + 1, mSize.x - 2, mSize.y - 2, mTheme->mButtonCornerRadius - 1);
                NVGpaint imgPaint = nvgImagePattern(ctx,
                    mPos.x + 1, mPos.y + 1, mSize.x - 2, mSize.y - 2,
                    //iconPos.x, iconPos.y - ih / 2, iw, ih,
                    0, mIcon, mEnabled ? 1.0f : 0.5f);
                nvgFillPaint(ctx, imgPaint);
                nvgFill(ctx);
            }

            nvgFontSize(ctx, fontSize);
            nvgFontFace(ctx, mTheme->mButtonFont.c_str());
            nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
            nvgFillColor(ctx, mTheme->mTextColorShadow);
            nvgText(ctx, textPos.x, textPos.y, mCaption.c_str(), nullptr);
            nvgFillColor(ctx, textColor);
            nvgText(ctx, textPos.x, textPos.y + 1, mCaption.c_str(), nullptr);
        }

        void UIButton::drawBackground(NVGcontext* ctx, NVGcolor lGradTop, NVGcolor lGradBot)
        {
            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, mPos.x + 1, mPos.y + 1.0f, mSize.x - 2, mSize.y - 2, mTheme->mButtonCornerRadius - 1);

            if (mBackgroundColor.a() != 0)
            {
                nvgFillColor(ctx, Color(mBackgroundColor.r(), mBackgroundColor.g(), mBackgroundColor.b(), 1.f));
                nvgFill(ctx);
                lGradTop.a = lGradBot.a = 0.0f;
            }

            NVGpaint bg = nvgLinearGradient(ctx, mPos.x, mPos.y, mPos.x, mPos.y + mSize.y, lGradTop, lGradBot);
            nvgFillPaint(ctx, bg);
            nvgFill(ctx);

            nvgBeginPath(ctx);
            nvgStrokeWidth(ctx, 1.0f);
            nvgRoundedRect(ctx, mPos.x + 0.5f, mPos.y + (mPushed ? 0.5f : 1.5f), mSize.x - 1, mSize.y - 1 - (mPushed ? 0.0f : 1.0f), mTheme->mButtonCornerRadius);
            nvgStrokeColor(ctx, mTheme->mBorderLight);
            nvgStroke(ctx);

            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, mPos.x + 0.5f, mPos.y + 0.5f, mSize.x - 1, mSize.y - 2, mTheme->mButtonCornerRadius);
            nvgStrokeColor(ctx, mTheme->mBorderDark);
            nvgStroke(ctx);
        }

        void UIButton::Serialize(Json::Value& aSerializer) const
        {
            //Widget::save(s);
            //s.set("caption", mCaption);
            //s.set("icon", mIcon);
            //s.set("iconPosition", (int)mIconPosition);
            //s.set("pushed", mPushed);
            //s.set("flags", mFlags);
            //s.set("backgroundColor", mBackgroundColor);
            //s.set("textColor", mTextColor);
        }

        void UIButton::Deserialize(const Json::Value& aSerializer)
        {
            string lManagerName = Utils::Demangling(typeid(LanguageManager).name());
            LanguageManager& lLanguageManager = static_cast<LanguageManager&>(Engine::Instance()->Manager(lManagerName));

            UIWidget::Deserialize(aSerializer);
            mCaption = lLanguageManager.GetString(aSerializer.get("caption", mCaption).asString());
            mFontSize = aSerializer.get("fontSize", mFontSize).asInt();
            mActionCmd = aSerializer.get("action", mActionCmd).asString();
            mPushed = aSerializer.get("pushed", mPushed).asBool();

            //Caption params
            mCaptionMargin = aSerializer.get("captionMargin", mCaptionMargin).asInt();
            string lAlignmentStr = aSerializer.get("captionAlignment", AlignmentToString(mCaptionAlignment)).asString();
            mCaptionAlignment = StringToAlignment(lAlignmentStr.c_str());
            if (mCaptionAlignment == Alignment::eINVALID)
                WARNING("Invalid caption aligment value[%s]", lAlignmentStr.c_str());

            //Icon params
            auto lIcon = aSerializer.get("icon", mIcon);
            if ( lIcon.isInt() )
                mIcon = lIcon.asInt();
            else
            {
                string lFileName = lIcon.asString();
                auto lIcon = Engine::Instance()->ResourceManager().FindImage(lFileName);

                //Invalid icon
                if (lIcon < 0)
                    WARNING("Button image \"%s\" not found", lFileName.c_str());
                mIcon = lIcon;
            }
            string lIconPosStr = aSerializer.get("iconPosition", IconPositionToString(mIconPosition)).asString();
            mIconPosition = StringToIconPosition(lIconPosStr.c_str());

            //Type flags
            if (aSerializer.isMember("flag"))
            {
                ButtonType lType = StringToButtonType(aSerializer["flag"].asString().c_str());
                mFlags = ButtonTypeToInt(lType);
            }

            if (aSerializer.isMember("backgroundColor"))
            {
                int r = aSerializer["backgroundColor"][0].asInt();
                int g = aSerializer["backgroundColor"][1].asInt();
                int b = aSerializer["backgroundColor"][2].asInt();
                int a = aSerializer["backgroundColor"][3].asInt();
                mBackgroundColor = Color(r, g, b, a);
            }

            if (aSerializer.isMember("textColor"))
            {
                int r = aSerializer["textColor"][0].asInt();
                int g = aSerializer["textColor"][1].asInt();
                int b = aSerializer["textColor"][2].asInt();
                int a = aSerializer["textColor"][3].asInt();
                mTextColor = Color(r, g, b, a);
            }

            mBackgroundVisible = aSerializer.get("mBackgroundVisible", true).asBool();
        }
    }

    void UIButton::setCaptionAlignment(Alignment aAlignment, int aMargin)
    {
        mCaptionAlignment = aAlignment;
        mCaptionMargin = aMargin;
    }

    void UIButton::setCaption(const std::string &caption)
    {
        string lManagerName = Utils::Demangling(typeid(LanguageManager).name());
        LanguageManager& lLanguageManager = static_cast<LanguageManager&>(Engine::Instance()->Manager(lManagerName));
        mCaption = lLanguageManager.GetString(caption);
    }

    void UIButton::setCaptionUnlocalized(const std::string &caption)
    {
        mCaption = caption;
    }
}
