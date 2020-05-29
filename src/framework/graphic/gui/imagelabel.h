/*
nanogui/imagelabel.h -- Widget used to display static images.

NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
The widget drawing code is based on the NanoVG demo application
by Mikko Mononen.

All rights reserved. Use of this source code is governed by a
BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once

#include "graphic/gui/widget.h"
#include "graphic/texture.h"

namespace Framework
{
    namespace GUI
    {
        class UIImageLabel : public UIWidget
        {
        public:
            UIImageLabel();
            UIImageLabel(UIWidget* parent, int aIcon);  /// Construct a new ImageLabel with the given parent widget

            ~UIImageLabel();

            /// Returns the icon of this Label.  
            int icon() const { return mIcon; }

            /// Sets the icon of this Label.  See \ref nanogui::Button::mIcon.
            void setIcon(int icon) { mIcon = icon; }

            glm::ivec2 preferredSize(NVGcontext* ctx) const;

            /// Draw the label
            virtual void draw(NVGcontext* ctx) override;

            /// Save the state of the widget into the given \ref Serializer instance
            virtual void Serialize(Json::Value& aSerializer) const override;

            /// Restore the state of the widget from the given \ref Serializer instance
            virtual void Deserialize(const Json::Value& aSerializer) override;

        protected:
            int  mIcon;
        };
    }
}
