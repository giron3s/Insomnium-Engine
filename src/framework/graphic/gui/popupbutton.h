/*
    nanogui/popupbutton.h -- Button which launches a popup widget

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include "graphic/gui/button.h"
#include "graphic/gui/popup.h"

namespace Framework
{
    namespace GUI
    {
        /**
        * \class PopupButton popupbutton.h nanogui/popupbutton.h
        *
        * \brief Button which launches a popup widget.
        *
        * \remark
        *     This class overrides \ref nanogui::Widget::mIconExtraScale to be ``0.8f``,
        *     which affects all subclasses of this Widget.  Subclasses must explicitly
        *     set a different value if needed (e.g., in their constructor).
        */
        class UIPopupButton : public UIButton
        {
        public:
            UIPopupButton();
            UIPopupButton(UIWidget *aParent, const std::string &aCaption = "@UNTITLED", int aButtonIcon = 0);
            virtual ~UIPopupButton();

            void createPopup();

            static void CollapseChildPopups(UIWidget* aItem);

            void setChevronIcon(int icon) { mChevronIcon = icon; }
            int chevronIcon() const { return mChevronIcon; }

            void setAppearance(uint32_t appearance);

            UIPopup* popup() { return mPopup; }
            const UIPopup* popup() const { return mPopup; }

            virtual void draw(NVGcontext* ctx) override;
            virtual glm::ivec2 preferredSize(NVGcontext *ctx) const override;
            virtual void performLayout(NVGcontext *ctx) override;

            /// The callback that is called when any type of mouse button event is issued to this Button.
            bool mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers) override;

            virtual void Serialize(Json::Value& aSerializer) const override;
            virtual void Deserialize(const Json::Value& aSerializer) override;

        protected:
            UIPopup*    mPopup;
            glm::ivec2  mPopupSize;
            int         mChevronIcon;
        //public:
        //    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };
    }
}
