/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/

#pragma once

#include "graphic/gui/widget.h"

namespace Framework
{
    namespace GUI
    {
        class UIDrawRect : public UIWidget
        {
        public:

            UIDrawRect();

            UIDrawRect(UIWidget *aParent);

            virtual ~UIDrawRect();

            /// Get the label color
            Color color() const { return mColor; }
            /// Set the label color
            void setColor(const Color& color) { mColor = color; }

            /// Set the \ref Theme used to draw this widget
            virtual void setTheme(UITheme *theme) override;

            /// Draw the label
            virtual void draw(NVGcontext *ctx) override;

            /// Save the state of the widget into the given \ref Serializer instance
            virtual void Serialize(Json::Value& aSerializer) const override;

            /// Restore the state of the widget from the given \ref Serializer instance
            virtual void Deserialize(const Json::Value& aSerializer) override;

        protected:
            Color       mColor;
        };
    }
}