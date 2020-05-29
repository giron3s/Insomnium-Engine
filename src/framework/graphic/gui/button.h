/*
    nanogui/button.h -- [Normal/Toggle/Radio/Popup] Button widget

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include "graphic/gui/widget.h"
#include "graphic/gui/iconposition.h"
#include "graphic/gui/alignment.h"

namespace Framework
{
    namespace GUI
    {
        /**
        * \class Button button.h nanogui/button.h
        *
        * \brief [Normal/Toggle/Radio/Popup] Button widget.
        */
        class UIButton : public UIWidget
        {
        public:

            UIButton();

            /**
             * \brief Creates a button attached to the specified parent.
             *
             * \param parent
             *     The \ref nanogui::Widget this Button will be attached to.
             *
             * \param caption
             *     The name of the button (default ``"Untitled"``).
             *
             * \param icon
             *     The icon to display with this Button.  See \ref nanogui::Button::mIcon.
             */
            UIButton(UIWidget *aParent, const std::string &aCaption = "@UNTITLED", int aIcon = 0);

            /// Returns the caption of this Button.
            const std::string &caption() const { return mCaption; }

            /// Sets the caption of this Button.
            void setCaption(const std::string &caption);
            void setCaptionUnlocalized(const std::string &caption);

            /// Returns the background color of this Button.
            const Color &backgroundColor() const { return mBackgroundColor; }

            /// Sets the background color of this Button.
            void setBackgroundColor(const Color &backgroundColor) { mBackgroundColor = backgroundColor; }

            /// Returns the text color of the caption of this Button.
            const Color &textColor() const { return mTextColor; }

            /// Sets the text color of the caption of this Button.
            void setTextColor(const Color &textColor) { mTextColor = textColor; }

            /// Returns the icon of this Button.  See \ref nanogui::Button::mIcon.
            int icon() const { return mIcon; }

            /// Sets the icon of this Button.  See \ref nanogui::Button::mIcon.
            void setIcon(int icon) { mIcon = icon; }

            /// The current flags of this Button (see \ref nanogui::Button::Flags for options).
            int flags() const { return mFlags; }

            /// Sets the flags of this Button (see \ref nanogui::Button::Flags for options).
            void setFlags(int buttonFlags) { mFlags = buttonFlags; }

            /// Sets LUA action command
            void setLuaAction(const std::string& aActionCmd) { mActionCmd = aActionCmd; }

            /// The position of the icon for this Button.
            IconPosition iconPosition() const { return mIconPosition; }

            /// Sets the position of the icon for this Button.
            void setIconPosition(IconPosition iconPosition) { mIconPosition = iconPosition; }

            /// Whether or not this Button is currently pushed.
            bool pushed() const { return mPushed; }

            /// Sets whether or not this Button is currently pushed.
            void setPushed(bool pushed) { mPushed = pushed; }

            /// The current callback to execute (for any type of button).
            std::function<void()> callback() const { return mCallback; }

            /// Set the push callback (for any type of button).
            void setCallback(const std::function<void()> &callback) { mCallback = callback; }

            /// The current callback to execute (for toggle buttons).
            std::function<void(bool)> changeCallback() const { return mChangeCallback; }

            /// Set the change callback (for toggle buttons).
            void setChangeCallback(const std::function<void(bool)> &callback) { mChangeCallback = callback; }

            /// Set the button group (for radio buttons).
            void setButtonGroup(const std::vector<UIButton *> &buttonGroup) { mButtonGroup = buttonGroup; }

            /// The current button group (for radio buttons).
            const std::vector<UIButton *> &buttonGroup() const { return mButtonGroup; }

            /// The preferred size of this Button.
            virtual glm::ivec2 preferredSize(NVGcontext *ctx) const override;

            /// The callback that is called when any type of mouse button event is issued to this Button.
            virtual bool mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers) override;

            /// Responsible for drawing the Button.
            virtual void draw(NVGcontext *ctx) override;
            void drawBackground(NVGcontext *ctx, NVGcolor lGradTop, NVGcolor lGradBot);

            /// Saves the state of this Button provided the given Serializer.
            virtual void Serialize(Json::Value& aSerializer) const override;

            /// Sets the state of this Button provided the given Serializer.
            virtual void Deserialize(const Json::Value& aSerializer) override;

            void setCaptionAlignment(Alignment aAlignment, int aMargin = 0);

        protected:
            std::string               mActionCmd;         /// The lua action command

            std::string               mCaption;           /// The caption of this Button.
            Alignment                 mCaptionAlignment;  
            int                       mCaptionMargin;     /// Margin of the text

            int                       mIcon;
            IconPosition              mIconPosition;      /// The position to draw the icon at.
                                                          
            bool                      mPushed;            /// Whether or not this Button is currently pushed.
            int                       mFlags;             /// The current flags of this button (see \ref nanogui::Button::Flags for options).
            Color                     mBackgroundColor;   /// The background color of this Button.
            Color                     mTextColor;         /// The color of the caption text of this Button.
            std::function<void()>     mCallback;          /// The callback issued for all types of buttons.
            std::function<void(bool)> mChangeCallback;    /// The callback issued for toggle buttons.
            std::vector<UIButton *>   mButtonGroup;       /// The button group for radio buttons.

            bool                      mBackgroundVisible;

//        public:
//            EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };
    }
}
