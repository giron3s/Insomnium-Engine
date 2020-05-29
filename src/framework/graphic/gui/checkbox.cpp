/*
    src/checkbox.cpp -- Two-state check box widget

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"
#include "engine.h"

#include "json/json.h"
#include "graphic/gui/checkbox.h"
#include "graphic/gui/theme.h"
#include "graphic/gui/entypo.h"

namespace Framework
{
    namespace GUI
    {
        UICheckBox::UICheckBox()
            : UIWidget(nullptr)
            , mCaption("")
            , mPushed(false)
            , mChecked(false)
            , mCallback(std::function<void(bool)>())
        {

        }

        UICheckBox::UICheckBox(UIWidget *aParent, const std::string &aCaption, const std::function<void(bool) > &aCallback)
            : UIWidget(aParent)
            , mPushed(false)
            , mChecked(false)
            , mCallback(aCallback) 
        {
            string lManagerName = Utils::Demangling(typeid(LanguageManager).name());
            LanguageManager& lLanguageManager = static_cast<LanguageManager&>(Engine::Instance()->Manager(lManagerName));
            mCaption = lLanguageManager.GetString(aCaption);
        }

        bool UICheckBox::mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers) 
        {
            UIWidget::mouseButtonEvent(p, button, down, modifiers);
            if (!mEnabled)
                return false;

            if (button == GLFW_MOUSE_BUTTON_1) {
                if (down) {
                    mPushed = true;
                }
                else if (mPushed) {
                    if (contains(p)) {
                        mChecked = !mChecked;
                        if (mCallback)
                            mCallback(mChecked);
                    }
                    mPushed = false;
                }
                return true;
            }
            return false;
        }

        glm::ivec2 UICheckBox::preferredSize(NVGcontext *ctx) const
        {
            if (mFixedSize != glm::ivec2())
                return mFixedSize;
            nvgFontSize(ctx, fontSize());
            nvgFontFace(ctx, mTheme->mLabelFont.c_str());
            return glm::ivec2(
                nvgTextBounds(ctx, 0, 0, mCaption.c_str(), nullptr, nullptr) +
                1.8f * fontSize(),
                fontSize() * 1.3f);
        }

        void UICheckBox::draw(NVGcontext *ctx) 
        {
            UIWidget::draw(ctx);

            nvgFontSize(ctx, fontSize());
            nvgFontFace(ctx, mTheme->mLabelFont.c_str());
            nvgFillColor(ctx,
                mEnabled ? mTheme->mTextColor : mTheme->mDisabledTextColor);
            nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
            nvgText(ctx, mPos.x + 1.6f * fontSize(), mPos.y + mSize.y * 0.5f,
                mCaption.c_str(), nullptr);

            NVGpaint bg = nvgBoxGradient(ctx, mPos.x + 1.5f, mPos.y + 1.5f,
                mSize.y - 2.0f, mSize.y - 2.0f, 3, 3,
                mPushed ? Color(0, 100) : Color(0, 32),
                Color(0, 0, 0, 180));

            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, mPos.x + 1.0f, mPos.y + 1.0f, mSize.y - 2.0f,
                mSize.y - 2.0f, 3);
            nvgFillPaint(ctx, bg);
            nvgFill(ctx);

            if (mChecked) {
                nvgFontSize(ctx, 1.8 * mSize.y);
                nvgFontFace(ctx, "icons");
                nvgFillColor(ctx, mEnabled ? mTheme->mIconColor
                    : mTheme->mDisabledTextColor);
                nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
                nvgText(ctx, mPos.x + mSize.y * 0.5f + 1,
                    mPos.y + mSize.y * 0.5f, utf8(ENTYPO_ICON_CHECK).data(),
                    nullptr);
            }
        }

        void UICheckBox::Serialize(Json::Value& aSerializer) const
        {
            //Widget::save(s);
            //s.set("caption", mCaption);
            //s.set("pushed", mPushed);
            //s.set("checked", mChecked);
        }

        void UICheckBox::Deserialize(const Json::Value& aSerializer) 
        {
            string lManagerName = Utils::Demangling(typeid(LanguageManager).name());
            LanguageManager& lLanguageManager = static_cast<LanguageManager&>(Engine::Instance()->Manager(lManagerName));

            UIWidget::Deserialize(aSerializer);
            mCaption = lLanguageManager.GetString(aSerializer.get("caption", mCaption).asString());
            mPushed  = aSerializer.get("pushed", mPushed).asBool();
            mChecked = aSerializer.get("checked", mChecked).asBool();
        }

        void UICheckBox::setCaption(const std::string &caption)
        {
            string lManagerName = Utils::Demangling(typeid(LanguageManager).name());
            LanguageManager& lLanguageManager = static_cast<LanguageManager&>(Engine::Instance()->Manager(lManagerName));
            mCaption = lLanguageManager.GetString(caption);
        }
    }
}
