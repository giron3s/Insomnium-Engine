/*
    src/progressbar.cpp -- Standard widget for visualizing progress

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"

#include "json/json.h"
#include "graphic/gui/progressbar.h"

namespace Framework
{
    namespace GUI
    {
        UIProgressBar::UIProgressBar()
            : UIWidget(nullptr)
            , mValue(0.0f) 
        {
        
        }

        UIProgressBar::UIProgressBar(UIWidget *aParent)
            : UIWidget(aParent)
            , mValue(0.0f)
        {
        
        }

        glm::ivec2 UIProgressBar::preferredSize(NVGcontext *) const 
        {
            return glm::ivec2(70, 12);
        }

        void UIProgressBar::draw(NVGcontext* ctx) 
        {
            UIWidget::draw(ctx);

            NVGpaint paint = nvgBoxGradient(
                ctx, mPos.x + 1, mPos.y + 1,
                mSize.x - 2, mSize.y, 3, 4, Color(0, 32), Color(0, 92));
            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, mPos.x, mPos.y, mSize.x, mSize.y, 3);
            nvgFillPaint(ctx, paint);
            nvgFill(ctx);

            float value = std::min(std::max(0.0f, mValue), 1.0f);
            int barPos = (int)std::round((mSize.x - 2) * value);

            paint = nvgBoxGradient(
                ctx, mPos.x, mPos.y,
                barPos + 1.5f, mSize.y - 1, 3, 4,
                Color(220, 100), Color(128, 100));

            nvgBeginPath(ctx);
            nvgRoundedRect(
                ctx, mPos.x + 1, mPos.y + 1,
                barPos, mSize.y - 2, 3);
            nvgFillPaint(ctx, paint);
            nvgFill(ctx);
        }

        void UIProgressBar::Serialize(Json::Value& aSerializer) const
        {
        //    Widget::save(s);
        //    s.set("value", mValue);
        }

        void UIProgressBar::Deserialize(const Json::Value& aSerializer)
        {
            UIWidget::Deserialize(aSerializer);
            mValue = aSerializer.get("value", mValue).asFloat();
        }
    }
}

