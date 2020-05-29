/*
    nanogui/checkbox.h -- Two-state check box widget

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/**
 * \file nanogui/checkbox.h
 *
 * \brief Two-state check box Widget.
 */

#pragma once

#include "graphic/gui/widget.h"

namespace Framework
{
    namespace GUI
    {
        /**
        * \class CheckBox checkbox.h nanogui/checkbox.h
        *
        * \brief Two-state check box widget.
        * *
        * \remark
        *     This class overrides \ref nanogui::Widget::mIconExtraScale to be ``1.2f``,
        *     which affects all subclasses of this Widget.  Subclasses must explicitly
        *     set a different value if needed (e.g., in their constructor).
        */
        class UICheckBox : public UIWidget
        {
        public:

            UICheckBox();

            /**
              * Adds a CheckBox to the specified ``parent``.
              *
              * \param parent
              *     The Widget to add this CheckBox to.
              *
              * \param caption
              *     The caption text of the CheckBox (default ``"Untitled"``).
              *
              * \param callback
              *     If provided, the callback to execute when the CheckBox is checked or
              *     unchecked.  Default parameter function does nothing.  See
              *     \ref nanogui::CheckBox::mPushed for the difference between "pushed"
              *     and "checked".
              */
            UICheckBox(UIWidget *aParent, const std::string &aCaption = "@UNTITLED",
                const std::function<void(bool)> &aCallback = std::function<void(bool)>());

            /// The caption of this CheckBox.
            const std::string &caption() const { return mCaption; }

            /// Sets the caption of this CheckBox.
            void setCaption(const std::string &caption);

            /// Whether or not this CheckBox is currently checked.
            const bool &checked() const { return mChecked; }

            /// Sets whether or not this CheckBox is currently checked.
            void setChecked(const bool &checked) { mChecked = checked; }

            /// Whether or not this CheckBox is currently pushed.  See \ref nanogui::CheckBox::mPushed.
            const bool &pushed() const { return mPushed; }

            /// Sets whether or not this CheckBox is currently pushed.  See \ref nanogui::CheckBox::mPushed.
            void setPushed(const bool &pushed) { mPushed = pushed; }

            /// Returns the current callback of this CheckBox.
            std::function<void(bool)> callback() const { return mCallback; }

            /// Sets the callback to be executed when this CheckBox is checked / unchecked.
            void setCallback(const std::function<void(bool)> &callback) { mCallback = callback; }

            /**
             * The mouse button callback will return ``true`` when all three conditions are met:
             *
             * 1. This CheckBox is "enabled" (see \ref nanogui::Widget::mEnabled).
             * 2. ``p`` is inside this CheckBox.
             * 3. ``button`` is ``GLFW_MOUSE_BUTTON_1`` (left mouse click).
             *
             * Since a mouse button event is issued for both when the mouse is pressed, as well
             * as released, this function sets \ref nanogui::CheckBox::mPushed to ``true`` when
             * parameter ``down == true``.  When the second event (``down == false``) is fired,
             * \ref nanogui::CheckBox::mChecked is inverted and \ref nanogui::CheckBox::mCallback
             * is called.
             *
             * That is, the callback provided is only called when the mouse button is released,
             * **and** the click location remains within the CheckBox boundaries.  If the user
             * clicks on the CheckBox and releases away from the bounds of the CheckBox,
             * \ref nanogui::CheckBox::mPushed is simply set back to ``false``.
             */
            virtual bool mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers) override;

            /// The preferred size of this CheckBox.
            virtual glm::ivec2 preferredSize(NVGcontext *ctx) const override;

            /// Draws this CheckBox.
            virtual void draw(NVGcontext *ctx) override;

            /// Saves this CheckBox to the specified Serializer.
            virtual void Serialize(Json::Value& aSerializer) const override;

            /// Loads the state of the specified Serializer to this CheckBox.
            virtual void Deserialize(const Json::Value& aSerializer) override;
        protected:
            std::string               mCaption;  /// The caption text of this CheckBox.
            bool                      mPushed;   /// Internal tracking variable to distinguish between mouse click and release.
            bool                      mChecked;  /// Whether or not this CheckBox is currently checked or unchecked.
            std::function<void(bool)> mCallback; /// The function to execute when \ref nanogui::CheckBox::mChecked is changed.
        //public:
        //    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };
    }
}
