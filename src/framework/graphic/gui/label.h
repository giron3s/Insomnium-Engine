/*
    nanogui/label.h -- Text label with an arbitrary font, color, and size

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include "graphic/gui/widget.h"

namespace Framework
{
    namespace GUI
    {
        /**
         * \class Label label.h nanogui/label.h
         *
         * \brief Text label widget.
         *
         * The font and color can be customized. When \ref Widget::setFixedWidth()
         * is used, the text is wrapped when it surpasses the specified width.
         */
        class UILabel : public UIWidget
        {
        public:
            /// Construct a new UILabel
            UILabel();

            /// Construct a new UILabel with the given parent widget
            UILabel(UIWidget *aParent, const std::string &aCaption, const bool aLocalizedCaption = true, const std::string& aFont = "sans", int aFontSize = -1);

            virtual ~UILabel();

            /// Get the label's text caption
            const std::string &caption() const { return mCaption; }

            /// Set the label's text caption
            void setCaption(const std::string &aCaption, const bool aLocalized = true);

            /// Set the currently active font (2 are available by default: 'sans' and 'sans-bold')
            void setFont(const std::string &font) { mFont = font; }
            /// Get the currently active font
            const std::string &font() const { return mFont; }

            /// Get the text color
            Color textColor() const { return mTextColor; }
            /// Set the text color
            void setTextColor(const Color& color) { mTextColor = color; }

            /// Get the background color
            Color backgroundColor() const { return mBackgroundColor; }
            /// Set the background color
            void setBackgroundColor(const Color& color) { mBackgroundColor = color; }

            /// Set the \ref Theme used to draw this widget
            virtual void setTheme(UITheme *theme) override;

            /// Compute the size needed to fully display the label
            virtual glm::ivec2 preferredSize(NVGcontext *ctx) const override;

            /// Draw the label
            virtual void draw(NVGcontext *ctx) override;

            /// Save the state of the widget into the given \ref Serializer instance
            virtual void Serialize(Json::Value& aSerializer) const override;

            /// Restore the state of the widget from the given \ref Serializer instance
            virtual void Deserialize(const Json::Value& aSerializer) override;

        protected:
            std::string mCaption;
            std::string mFont;
            Color       mTextColor, mBackgroundColor;
        //public:
        //    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };
    }
}
