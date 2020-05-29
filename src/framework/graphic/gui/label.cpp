/*
    src/label.cpp -- Text label with an arbitrary font, color, and size

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"
#include "engine.h"
#include "json/json.h"
#include "graphic/gui/label.h"
#include "graphic/gui/theme.h"


namespace Framework
{
    namespace GUI
    {

        UILabel::UILabel()
            : UIWidget(nullptr),
              mCaption("@UNTITLED"),
              mFont(""),
              mBackgroundColor(1.0f, 0.0f),
              mTextColor(0, 0)
        {

        }

        UILabel::UILabel(UIWidget *aParent,
                         const std::string &aCaption, 
                         const bool aLocalizedCaption/* = true*/, 
                         const std::string& aFont/* = "sans"*/, 
                         int aFontSize/* = -1*/)
            : UIWidget(aParent),
              mFont(aFont),
              mBackgroundColor(1.0f, 0.0f),
              mTextColor(0, 0)
        {
            //Set text of the label
            setCaption(aCaption, aLocalizedCaption);

            //Set the size of the font
            if (aFontSize >= 0) 
                mFontSize = aFontSize;

            //Set the theme
            if (mTheme) 
            {
                mFontSize = mTheme->mStandardFontSize;
                mTextColor = mTheme->mTextColor;
            }
        }

        UILabel::~UILabel()
        {

        }

        void UILabel::setTheme(UITheme *theme)
        {
            UIWidget::setTheme(theme);
            mFontSize = mTheme->mStandardFontSize;
        }

        glm::ivec2 UILabel::preferredSize(NVGcontext *ctx) const
        {
            if (mCaption == "")
                return glm::ivec2(0);

            if (mFont.compare(""))
                nvgFontFace(ctx, mFont.c_str());
            else
                nvgFontFace(ctx, mTheme->mLabelFont.c_str());

            nvgFontSize(ctx, fontSize());
            if (mFixedSize.x > 0) {
                float bounds[4];
                nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
                nvgTextBoxBounds(ctx, mPos.x, mPos.y, mFixedSize.x, mCaption.c_str(), nullptr, bounds);
                return glm::ivec2(mFixedSize.x, bounds[3] - bounds[1]);
            } else {
                nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                return glm::ivec2(
                    nvgTextBounds(ctx, 0, 0, mCaption.c_str(), nullptr, nullptr) + 2,
                    fontSize()
                );
            }
        }

        void UILabel::draw(NVGcontext *ctx) 
        {
            if ( mBackgroundColor.a() )
            { // first, draw own background if present
                nvgBeginPath(ctx);
                nvgRoundedRect(ctx, mPos.x, mPos.y, mSize.x, mSize.y, mTheme->mButtonCornerRadius);
                nvgFillColor(ctx, mBackgroundColor);
                nvgFill(ctx);
            }

            UIWidget::draw(ctx);

            if(mFont.compare(""))
                nvgFontFace(ctx, mFont.c_str());
            else
                nvgFontFace(ctx, mTheme->mLabelFont.c_str());

            nvgFontSize(ctx, fontSize());

            if(mTextColor.a() == 0)
                nvgFillColor(ctx, mTheme->mTextColor);
            else
                nvgFillColor(ctx, mTextColor);
            
            if (mFixedSize.x > 0) {
                nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
                nvgTextBox(ctx, mPos.x, mPos.y, mFixedSize.x, mCaption.c_str(), nullptr);
            } else {
                nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                nvgText(ctx, mPos.x, mPos.y + mSize.y * 0.5f, mCaption.c_str(), nullptr);
            }
        }

        void UILabel::Serialize(Json::Value& aSerializer) const
        {
            //    Widget::save(s);
            //    s.set("caption", mCaption);
            //    s.set("font", mFont);
            //    s.set("color", mColor);
        }

        void UILabel::Deserialize(const Json::Value& aSerializer)
        {
            string lManagerName = Utils::Demangling(typeid(LanguageManager).name());
            LanguageManager& lLanguageManager = static_cast<LanguageManager&>(Engine::Instance()->Manager(lManagerName));

            UIWidget::Deserialize(aSerializer);
            if (aSerializer.isMember("color"))
            {
               float r = aSerializer["color"][0].asFloat();
               float g = aSerializer["color"][1].asFloat();
               float b = aSerializer["color"][2].asFloat();
               float a = aSerializer["color"][3].asFloat();
               mTextColor = Color(r, g, b, a);
            }

            mCaption = lLanguageManager.GetString(aSerializer.get("caption", mCaption).asString());
            mFont    = aSerializer.get("font", mFont).asString();
            mFontSize = aSerializer.get("fontsize", mTheme->mStandardFontSize).asInt();
        }

        void UILabel::setCaption(const std::string &aCaption, const bool aLocalized /*= true*/)
        {
            //Not localized the text
            if (!aLocalized)
                mCaption = aCaption;

            //Localized text
            else
            {
                string lManagerName = Utils::Demangling(typeid(LanguageManager).name());
                LanguageManager& lLanguageManager = static_cast<LanguageManager&>(Engine::Instance()->Manager(lManagerName));
                mCaption = lLanguageManager.GetString(aCaption);
            }
        }
    }
}
