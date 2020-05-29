/*
    nanogui/imagelabel.cpp -- Widget used to display static images.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"

#include "engine.h"
#include "graphic/gui/imagelabel.h"
#include "graphic/gui/window.h"
#include "graphic/gui/screen.h"

namespace Framework
{
    namespace GUI
    {
        UIImageLabel::UIImageLabel()
            : UIWidget(nullptr)
            , mIcon(0)
        {
        }

        UIImageLabel::UIImageLabel(UIWidget* parent, int aIcon)
            : UIWidget(parent)
            , mIcon(aIcon)
        {

        }

        UIImageLabel::~UIImageLabel()
        {
        }

        glm::ivec2 UIImageLabel::preferredSize(NVGcontext* ctx) const
        {
            return mSize;
        }

        void UIImageLabel::draw(NVGcontext* ctx)
        {
            UIWidget::draw(ctx);

            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, mPos.x + 1, mPos.y + 1, mSize.x - 2, mSize.y - 2, 10);
            NVGpaint lImgPaint = nvgImagePattern(ctx, mPos.x + 1, mPos.y + 1, mSize.x - 2, mSize.y - 2, 0, mIcon, 1.0f);
            nvgFillPaint(ctx, lImgPaint);
            nvgFill(ctx);
        }

        void UIImageLabel::Serialize(Json::Value& aSerializer) const
        {
            //TODO IMPLEMENT IT
            ASSERT(false);
        }

        void UIImageLabel::Deserialize(const Json::Value& aSerializer)
        {
            UIWidget::Deserialize(aSerializer);

            auto lIcon = aSerializer.get("icon", mIcon);
            if (lIcon.isInt())
            {
                WARNING("UIImageLabel does not support entypo icons.");
                mIcon = lIcon.asInt();
            }
            else
            {
                string lFileName = lIcon.asString();
                auto lIcon = Engine::Instance()->ResourceManager().FindImage(lFileName);
                if (lIcon < 0)
                    WARNING("UIImageLabel image \"%s\" not found", lFileName.c_str());
                mIcon = lIcon;
            }
        }
    }
}
