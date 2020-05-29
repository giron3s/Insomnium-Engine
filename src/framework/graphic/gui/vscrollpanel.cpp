/*
    src/vscrollpanel.cpp -- Adds a vertical scrollbar around a widget
    that is too big to fit into a certain area

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"

#include "json/json.h"
#include "graphic/gui/vscrollpanel.h"
#include "graphic/gui/theme.h"

namespace Framework
{
    namespace GUI
    {

        UIVScrollPanel::UIVScrollPanel()
            : UIWidget(nullptr)
            , mChildPreferredHeight(0)
            , mScroll(0.0f)
            , mUpdateLayout(false)
        {

        }

        UIVScrollPanel::UIVScrollPanel(UIWidget *aParent)
            : UIWidget(aParent)
            , mChildPreferredHeight(0)
            , mScroll(0.0f)
            , mUpdateLayout(false)
        {

        }

        void UIVScrollPanel::performLayout(NVGcontext *ctx) {
            UIWidget::performLayout(ctx);

            if (mChildren.empty())
                return;
            if (mChildren.size() > 1)
                throw std::runtime_error("VScrollPanel should have one child.");

            UIWidget *child = mChildren[0];
            mChildPreferredHeight = child->preferredSize(ctx).y;

            if (mChildPreferredHeight > mSize.y) {
                child->setPosition(glm::ivec2(0, -mScroll*(mChildPreferredHeight - mSize.y)));
                child->setSize(glm::ivec2(mSize.x - 12, mChildPreferredHeight));
            }
            else {
                child->setPosition(glm::ivec2());
                child->setSize(mSize);
                mScroll = 0;
            }
            child->performLayout(ctx);
        }

        glm::ivec2 UIVScrollPanel::preferredSize(NVGcontext *ctx) const
        {
            if (mChildren.empty())
                return glm::ivec2();
            return mChildren[0]->preferredSize(ctx) + glm::ivec2(12, 0);
        }

        bool UIVScrollPanel::mouseDragEvent(const glm::ivec2 &p, const glm::ivec2 &rel, int button, int modifiers)
        {
            if (!mChildren.empty() && mChildPreferredHeight > mSize.y) {
                float scrollh = height() *
                    std::min(1.0f, height() / (float)mChildPreferredHeight);

                mScroll = std::max((float) 0.0f, std::min((float) 1.0f,
                    mScroll + rel.y / (float)(mSize.y - 8 - scrollh)));
                mUpdateLayout = true;
                return true;
            }
            else {
                return UIWidget::mouseDragEvent(p, rel, button, modifiers);
            }
        }

        bool UIVScrollPanel::scrollEvent(const glm::ivec2 &p, const glm::vec2 &rel)
        {
            if (!mChildren.empty() && mChildPreferredHeight > mSize.y) {
                float scrollAmount = rel.y * (mSize.y / 20.0f);
                float scrollh = height() *
                    std::min(1.0f, height() / (float)mChildPreferredHeight);

                mScroll = std::max((float) 0.0f, std::min((float) 1.0f,
                    mScroll - scrollAmount / (float)(mSize.y - 8 - scrollh)));
                mUpdateLayout = true;
                return true;
            }
            else {
                return UIWidget::scrollEvent(p, rel);
            }
        }

        void UIVScrollPanel::draw(NVGcontext *ctx)
        {
            if (mChildren.empty())
                return;
            UIWidget *child = mChildren[0];
            child->setPosition(glm::ivec2(0, -mScroll*(mChildPreferredHeight - mSize.y)));
            mChildPreferredHeight = child->preferredSize(ctx).y;
            float scrollh = height() *
                std::min(1.0f, height() / (float)mChildPreferredHeight);

            if (mUpdateLayout)
                child->performLayout(ctx);

            nvgSave(ctx);
            nvgTranslate(ctx, mPos.x, mPos.y);
            nvgIntersectScissor(ctx, 0, 0, mSize.x, mSize.y);
            if (child->visible())
                child->draw(ctx);
            nvgRestore(ctx);

            if (mChildPreferredHeight <= mSize.y)
                return;

            NVGpaint paint = nvgBoxGradient(
                ctx, mPos.x + mSize.x - 12 + 1, mPos.y + 4 + 1, 8,
                mSize.y - 8, 3, 4, Color(0, 32), Color(0, 92));
            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, mPos.x + mSize.x - 12, mPos.y + 4, 8,
                mSize.y - 8, 3);
            nvgFillPaint(ctx, paint);
            nvgFill(ctx);

            paint = nvgBoxGradient(
                ctx, mPos.x + mSize.x - 12 - 1,
                mPos.y + 4 + (mSize.y - 8 - scrollh) * mScroll - 1, 8, scrollh,
                3, 4, Color(220, 100), Color(128, 100));

            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, mPos.x + mSize.x - 12 + 1,
                mPos.y + 4 + 1 + (mSize.y - 8 - scrollh) * mScroll, 8 - 2,
                scrollh - 2, 2);
            nvgFillPaint(ctx, paint);
            nvgFill(ctx);
        }

        void UIVScrollPanel::Serialize(Json::Value& aSerializer) const
        {
            //Widget::save(s);
            //s.set("childPreferredHeight", mChildPreferredHeight);
            //s.set("scroll", mScroll);
        }

        void UIVScrollPanel::Deserialize(const Json::Value& aSerializer)
        {
            UIWidget::Deserialize(aSerializer);
            mChildPreferredHeight = aSerializer.get("childPreferredHeight", mChildPreferredHeight).asInt();
            mScroll = aSerializer.get("scroll", mScroll).asFloat();
        }
    }
}