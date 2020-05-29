/*
    src/popup.cpp -- Simple popup widget which is attached to another given
    window (can be nested)

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"

#include "json/json.h"
#include "graphic/gui/popup.h"
#include "graphic/gui/popupbutton.h"
#include "graphic/gui/theme.h"

namespace Framework
{
    namespace GUI
    {
        UIPopup::UIPopup()
            : UIWindow(nullptr),
              mParentWindow(nullptr),
              mParentButton(nullptr),
              mAnchorPos(glm::ivec2(0)),
              mAnchorHeight(30),
              mAppearance(Appearance::Right)
        {
        }

        UIPopup::UIPopup(UIWindow *parentWindow, UIPopupButton* parentButton)
            : UIWindow(parentWindow->parent(), ""),
              mParentWindow(parentWindow),
              mParentButton(parentButton),
              mAnchorPos(glm::ivec2(0)),
              mAnchorHeight(30),
              mAppearance(Appearance::Right)
        {
        }

        UIPopup::~UIPopup()
        {
        }

        void UIPopup::performLayout(NVGcontext *ctx)
        {
            if (mLayout || mChildren.size() != 1) {
                UIWidget::performLayout(ctx);
            } else {
                mChildren[0]->setPosition(glm::ivec2(0));
                mChildren[0]->setSize(mSize);
                mChildren[0]->performLayout(ctx);
            }

            if (mAppearance & Appearance::Left)
                mAnchorPos.x -= mSize.x;
            
            else if (mAppearance & Appearance::Up)
                mAnchorPos.y -= mSize.y;

            if ( !mChildren.empty() &&(mAppearance & Appearance::Menu) )
            {
                mSize.x = 0;
                for(const auto& lItem : mChildren)
                    if(lItem->size().x > mSize.x)
                        mSize.x = lItem->size().x;
                const auto& lLastItem = mChildren[mChildren.size()-1];
                mSize.y = lLastItem->position().y + lLastItem->size().y;
            }
        }

        void UIPopup::refreshRelativePlacement()
        {
            mParentWindow->refreshRelativePlacement();
            mVisible &= mParentWindow->visibleRecursive();
            mPos = mParentWindow->position() + mAnchorPos - glm::ivec2(0, mAnchorHeight);
        }

        void UIPopup::draw(NVGcontext* ctx)
        { 
            refreshRelativePlacement();

            if (!mVisible)
                return;

            int ds = mTheme->mWindowDropShadowSize, cr = mTheme->mWindowCornerRadius;

            nvgSave(ctx);
            nvgResetScissor(ctx);

            /* Draw a drop shadow */
            NVGpaint shadowPaint = nvgBoxGradient(
                ctx, mPos.x, mPos.y, mSize.x, mSize.y, cr*2, ds*2,
                mTheme->mDropShadow, mTheme->mTransparent);

            nvgBeginPath(ctx);
            nvgRect(ctx, mPos.x-ds,mPos.y-ds, mSize.x+2*ds, mSize.y+2*ds);
            nvgRoundedRect(ctx, mPos.x, mPos.y, mSize.x, mSize.y, cr);
            nvgPathWinding(ctx, NVG_HOLE);
            nvgFillPaint(ctx, shadowPaint);
            nvgFill(ctx);

            /* Draw window */
            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, mPos.x, mPos.y, mSize.x, mSize.y, cr);

            if(!(mAppearance & Appearance::Menu))
            {
                glm::ivec2 base = mPos + glm::ivec2(0, mAnchorHeight);
                int sign = -1;
                if (mAppearance & Appearance::Left) {
                    base.x -= mSize.x;
                    sign = 1;
                }
                else if (mAppearance & Appearance::Up) {
                    base.y -= mSize.y;
                    sign = 1;
                }

                nvgMoveTo(ctx, base.x + 15*sign, base.y);
                nvgLineTo(ctx, base.x - 1*sign, base.y - 15);
                nvgLineTo(ctx, base.x - 1*sign, base.y + 15);

                nvgFillColor(ctx, mTheme->mWindowPopup);
                nvgFill(ctx);
            }

            nvgRestore(ctx);

            UIWidget::draw(ctx);
        }

        void UIPopup::Serialize(Json::Value& aSerializer) const
        {
            //Window::save(s);
            //s.set("anchorPos", mAnchorPos);
            //s.set("anchorHeight", mAnchorHeight);
            //s.set("side", mSide);
        }

        void UIPopup::Deserialize(const Json::Value& aSerializer)
        {
            UIWindow::Deserialize(aSerializer);

            if (aSerializer.isMember("appearance"))
            {
                mAppearance = 0;
                auto lAppearanceString = aSerializer.get("appearance", "").asString();
                auto i = lAppearanceString.find("Menu");
                if(i != string::npos)
                    lAppearanceString.erase(i, 4);
                if(lAppearanceString.compare("Left") == 0)
                    mAppearance = Appearance::Left;
                else if(lAppearanceString.compare("Right") == 0)
                    mAppearance = Appearance::Right;
                else if(lAppearanceString.compare("Down") == 0)
                    mAppearance = Appearance::Down;
                else if(lAppearanceString.compare("Up") == 0)
                    mAppearance = Appearance::Up;
                if(!mAppearance)
                    CRASH("Popup appearance '%s' is invalid", lAppearanceString.c_str());
                if(i != string::npos)
                {
                    mAppearance |= Appearance::Menu;
                    mAnchorHeight = 0;
                }
            }

            mAnchorHeight =  aSerializer.get("anchorHeight", mAnchorHeight).asInt();

            if (aSerializer.isMember("anchorPos"))
            {
                mAnchorPos.x = aSerializer["anchorPos"][0].asInt();
                mAnchorPos.y = aSerializer["anchorPos"][1].asInt();
            }
        }
    }
}
