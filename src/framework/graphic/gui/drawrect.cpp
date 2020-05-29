/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/

#include "precompiled.h"

#include "json/json.h"
#include "graphic/gui/drawrect.h"
#include "graphic/gui/theme.h"


namespace Framework
{
    namespace GUI
    {

        UIDrawRect::UIDrawRect() : UIWidget(nullptr)
        {

        }

        UIDrawRect::UIDrawRect(UIWidget *parent)
            : UIWidget(parent)
        {
            if (mTheme) {
                mColor = mTheme->mTextColor;
            }
        }


        UIDrawRect::~UIDrawRect()
        {

        }

        void UIDrawRect::setTheme(UITheme *theme)
        {
            UIWidget::setTheme(theme);
            if (mTheme) {
                mFontSize = mTheme->mStandardFontSize;
                mColor = mTheme->mTextColor;
            }
        }

        void UIDrawRect::draw(NVGcontext *ctx)
        {
            UIWidget::draw(ctx);
            nvgBeginPath(ctx);
            nvgScissor(ctx, mPos.x, mPos.y, mSize.x, mSize.y);

            nvgRect(ctx, mPos.x, mPos.y, mSize.x, mSize.y);
            nvgFillColor(ctx, mColor);
            nvgFill(ctx);

            nvgStroke(ctx);
        }

        void UIDrawRect::Serialize(Json::Value& aSerializer) const
        {
            //    Widget::save(s);
            //    s.set("caption", mCaption);
            //    s.set("font", mFont);
            //    s.set("color", mColor);
        }

        void UIDrawRect::Deserialize(const Json::Value& aSerializer)
        {
            UIWidget::Deserialize(aSerializer);
            if (aSerializer.isMember("color"))
            {
                float r = aSerializer["color"][0].asFloat();
                float g = aSerializer["color"][1].asFloat();
                float b = aSerializer["color"][2].asFloat();
                float a = aSerializer["color"][3].asFloat();
                mColor = Color(r, g, b, a);
            }
        }
    }
}