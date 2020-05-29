/*
    src/window.cpp -- Top-level window widget

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"
#include "engine.h"
#include "json/json.h"
#include "graphic/gui/window.h"
#include "graphic/gui/theme.h"
#include "graphic/gui/screen.h"
#include "graphic/gui/layout.h"

namespace Framework
{
    namespace GUI
    {
        UIWindow::UIWindow()
            : UIWidget(nullptr)
            , mTitle("")
            , mButtonPanel(nullptr)
            , mModal(false)
            , mDrag(false)
            , mCentered(false)
            , mFillParent(false)
        {

        }


        UIWindow::UIWindow(UIWidget *aParent, const std::string &aTitle)
            : UIWidget(aParent)
            , mButtonPanel(nullptr)
            , mModal(false)
            , mDrag(false)
            , mCentered(false)
            , mFillParent(false)
        {
            string lManagerName = Utils::Demangling(typeid(LanguageManager).name());
            LanguageManager& lLanguageManager = static_cast<LanguageManager&>(Engine::Instance()->Manager(lManagerName));
            mTitle = lLanguageManager.GetString(aTitle);
        }

        UIWindow::~UIWindow()
        {

        }

        glm::ivec2 UIWindow::preferredSize(NVGcontext *ctx) const
        {
            glm::ivec2 lPreferredSize;
            if (mFillParent)
                lPreferredSize = mParent->size();
            else
                lPreferredSize = UIWidget::preferredSize(ctx);


            if (mButtonPanel)
                mButtonPanel->setVisible(true);

            //Add the title bounds to the prefered size
            nvgFontSize(ctx, 18.0f);
            nvgFontFace(ctx, mTheme->mHeadingFont.c_str());
            float bounds[4];
            nvgTextBounds(ctx, 0, 0, mTitle.c_str(), nullptr, bounds);

            // Get the max area
            return WiseMax(lPreferredSize, glm::ivec2(bounds[2]-bounds[0] + 20, bounds[3]-bounds[1]));
        }

        UIWidget *UIWindow::buttonPanel()
        {
            if (!mButtonPanel)
            {
                mButtonPanel = new UIWidget(this);
                mButtonPanel->setLayout(new UIBoxLayout(Orientation::eHorizontal, Alignment::eMiddle, 0, 4));
            }
            return mButtonPanel;
        }

        void UIWindow::performLayout(NVGcontext *ctx) {
            if (!mButtonPanel) {
                UIWidget::performLayout(ctx);
            } else {
                mButtonPanel->setVisible(false);
                UIWidget::performLayout(ctx);
                for (auto w : mButtonPanel->children()) {
                    w->setFixedSize(glm::ivec2(22, 22));
                    w->setFontSize(15);
                }
                mButtonPanel->setVisible(true);
                mButtonPanel->setSize(glm::ivec2(width(), 22));
                mButtonPanel->setPosition(glm::ivec2(width() - (mButtonPanel->preferredSize(ctx).x + 5), 3));
                mButtonPanel->performLayout(ctx);
            }

            //Centered the window
            if (mCentered)
                center();
        }

        void UIWindow::draw(NVGcontext *ctx) {
            int ds = mTheme->mWindowDropShadowSize, cr = mTheme->mWindowCornerRadius;
            int hh = mTheme->mWindowHeaderHeight;

            /* Draw window */
            nvgSave(ctx);
            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, mPos.x, mPos.y, mSize.x, mSize.y, cr);

            nvgFillColor(ctx, mMouseFocus ? mTheme->mWindowFillFocused
                                          : mTheme->mWindowFillUnfocused);
            nvgFill(ctx);


            /* Draw a drop shadow */
            NVGpaint shadowPaint = nvgBoxGradient(
                ctx, mPos.x, mPos.y, mSize.x, mSize.y, cr*2, ds*2,
                mTheme->mDropShadow, mTheme->mTransparent);

            nvgSave(ctx);
            nvgResetScissor(ctx);
            nvgBeginPath(ctx);
            nvgRect(ctx, mPos.x-ds,mPos.y-ds, mSize.x+2*ds, mSize.y+2*ds);
            nvgRoundedRect(ctx, mPos.x, mPos.y, mSize.x, mSize.y, cr);
            nvgPathWinding(ctx, NVG_HOLE);
            nvgFillPaint(ctx, shadowPaint);
            nvgFill(ctx);
            nvgRestore(ctx);

            if (!mTitle.empty()) {
                /* Draw header */
                NVGpaint headerPaint = nvgLinearGradient(
                    ctx, mPos.x, mPos.y, mPos.x,
                    mPos.y + hh,
                    mTheme->mWindowHeaderGradientTop,
                    mTheme->mWindowHeaderGradientBot);

                nvgBeginPath(ctx);
                nvgRoundedRect(ctx, mPos.x, mPos.y, mSize.x, hh, cr);

                nvgFillPaint(ctx, headerPaint);
                nvgFill(ctx);

                nvgBeginPath(ctx);
                nvgRoundedRect(ctx, mPos.x, mPos.y, mSize.x, hh, cr);
                nvgStrokeColor(ctx, mTheme->mWindowHeaderSepTop);

                nvgSave(ctx);
                nvgIntersectScissor(ctx, mPos.x, mPos.y, mSize.x, 0.5f);
                nvgStroke(ctx);
                nvgRestore(ctx);

                nvgBeginPath(ctx);
                nvgMoveTo(ctx, mPos.x + 0.5f, mPos.y + hh - 1.5f);
                nvgLineTo(ctx, mPos.x + mSize.x - 0.5f, mPos.y + hh - 1.5);
                nvgStrokeColor(ctx, mTheme->mWindowHeaderSepBot);
                nvgStroke(ctx);

                nvgFontSize(ctx, 18.0f);
                nvgFontFace(ctx, mTheme->mHeadingFont.c_str());
                nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

                nvgFontBlur(ctx, 2);
                nvgFillColor(ctx, mTheme->mDropShadow);
                nvgText(ctx, mPos.x + mSize.x / 2,
                        mPos.y + hh / 2, mTitle.c_str(), nullptr);

                nvgFontBlur(ctx, 0);
                nvgFillColor(ctx, mFocused ? mTheme->mWindowTitleFocused
                                           : mTheme->mWindowTitleUnfocused);
                nvgText(ctx, mPos.x + mSize.x / 2, mPos.y + hh / 2 - 1,
                        mTitle.c_str(), nullptr);
            }

            nvgRestore(ctx);
            UIWidget::draw(ctx);
        }

        void UIWindow::dispose() {
            UIWidget *widget = this;
            while (widget->parent())
                widget = widget->parent();
            ((UIScreen *) widget)->disposeWindow(this);
        }

        void UIWindow::center() {
            UIWidget *widget = this;
            while (widget->parent())
                widget = widget->parent();
            ((UIScreen *) widget)->centerWindow(this);
        }

        bool UIWindow::mouseDragEvent(const glm::ivec2 &, const glm::ivec2 &rel, int button, int /* modifiers */)
        {
            if (mDrag && (button & (1 << GLFW_MOUSE_BUTTON_1)) != 0)
            {
                mPos += rel;

                mPos = WiseMax(mPos, glm::ivec2(0));
                mPos = WiseMin(mPos, parent()->size() - mSize);
                return true;
            }
            return false;
        }

        bool UIWindow::mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers) {
            if (UIWidget::mouseButtonEvent(p, button, down, modifiers))
                return true;
            if (button == GLFW_MOUSE_BUTTON_1) {
                mDrag = down && (p.y - mPos.y) < mTheme->mWindowHeaderHeight;
                return true;
            }
            return false;
        }

        bool UIWindow::scrollEvent(const glm::ivec2 &p, const glm::vec2 &rel) {
            UIWidget::scrollEvent(p, rel);
            return true;
        }

        void UIWindow::refreshRelativePlacement() {
            /* Overridden in \ref Popup */
        }

        void UIWindow::Serialize(Json::Value& aSerializer) const
        {
            //TODO implement it
            assert(false);
        }

        void UIWindow::Deserialize(const Json::Value& aSerializer)
        {
            string lManagerName = Utils::Demangling(typeid(LanguageManager).name());
            LanguageManager& lLanguageManager = static_cast<LanguageManager&>(Engine::Instance()->Manager(lManagerName));

            UIWidget::Deserialize(aSerializer);
            mTitle = lLanguageManager.GetString(aSerializer.get("title", mTitle).asString());
            mModal = aSerializer.get("modal", mModal).asBool();
            mCentered = aSerializer.get("centered", mCentered).asBool();
            mFillParent = aSerializer.get("fillParent", mFillParent).asBool();

            //setTheme(new CustomTheme(Engine::Instance()->Display().nvgContext()));
        }

        void UIWindow::setTitle(const std::string &title)
        {
            string lManagerName = Utils::Demangling(typeid(LanguageManager).name());
            LanguageManager& lLanguageManager = static_cast<LanguageManager&>(Engine::Instance()->Manager(lManagerName));
            mTitle = lLanguageManager.GetString(title);
        }
    }
}

