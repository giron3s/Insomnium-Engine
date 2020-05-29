/*
    nanogui/popup.h -- Simple popup widget which is attached to another given
    window (can be nested)

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include "graphic/gui/window.h"

namespace Framework
{
    namespace GUI
    {
        /**
         * \class Popup popup.h nanogui/popup.h
         *
         * \brief Popup window for combo boxes, popup buttons, nested dialogs etc.
         *
         * Usually the Popup instance is constructed by another widget (e.g. \ref PopupButton)
         * and does not need to be created by hand.
         */
        class UIPopup : public UIWindow
        {
        public:
            enum Appearance : uint32_t { Left = 1, Right = 2, Down = 4, Up = 8, Menu = 16 };

            /// Construct a new popup
            UIPopup();

            /// Create a new popup parented to a screen (first argument) and a parent window
            UIPopup(UIWindow *parentWindow, UIPopupButton* parentButton);

            /// Free all resources used by the widget and any children
            virtual ~UIPopup();

            /// Return the anchor position in the parent window; the placement of the popup is relative to it
            void setAnchorPos(const glm::ivec2 &anchorPos) { mAnchorPos = anchorPos; }
            /// Set the anchor position in the parent window; the placement of the popup is relative to it
            const glm::ivec2 &anchorPos() const { return mAnchorPos; }

            /// Set the anchor height; this determines the vertical shift relative to the anchor position
            void setAnchorHeight(int anchorHeight) { mAnchorHeight = anchorHeight; }
            /// Return the anchor height; this determines the vertical shift relative to the anchor position
            int anchorHeight() const { return mAnchorHeight; }

            /// Set the side of the parent window at which popup will appear
            void setAppearance(uint32_t appearance) { mAppearance = appearance; }
            /// Return the side of the parent window at which popup will appear
            uint32_t appearance() const { return mAppearance; }

            /// Return the parent window of the popup
            UIWindow *parentWindow() { return mParentWindow; }
            /// Return the parent window of the popup
            const UIWindow *parentWindow() const { return mParentWindow; }

            /// Return the parent PopupButton for this Popup
            UIPopupButton *parentButton() { return mParentButton; }

            /// Invoke the associated layout generator to properly place child widgets, if any
            virtual void performLayout(NVGcontext *ctx) override;

            /// Draw the popup window
            virtual void draw(NVGcontext* ctx) override;

            /// Save the state of the popup into the given \ref Serializer instance
            virtual void Serialize(Json::Value& aSerializer) const override;

            /// Restore the state of the widget from the given \ref Serializer instance
            virtual void Deserialize(const Json::Value& aSerializer) override;

        protected:
            /// Internal helper function to maintain nested window position values
            virtual void refreshRelativePlacement() override;

        protected:
            UIWindow*       mParentWindow;
            UIPopupButton*  mParentButton;
            glm::ivec2      mAnchorPos;
            int             mAnchorHeight;
            uint32_t        mAppearance;
        //public:
        //    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };
    }
}
