/*
    nanogui/toolbutton.h -- Simple radio+toggle button with an icon

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once
#include "json/json.h"
#include "graphic/gui/button.h"
#include "graphic/gui/buttontype.h"

namespace Framework
{
    namespace GUI
    {
        /**
        * \class ToolButton toolbutton.h nanogui/toolbutton.h
        *
        * \brief Simple radio+toggle button with an icon.
        */
        class UIToolButton : public UIButton 
        {
        public:

            UIToolButton() : UIButton()
            {
                setFlags(ButtonTypeToInt(ButtonType::eRadioButton) | ButtonTypeToInt(ButtonType::eToggleButton));
                setFixedSize(glm::ivec2(25, 25));
            }

            UIToolButton(UIWidget *aParent, int aIcon, const std::string &aCaption = "")
                : UIButton(aParent, aCaption, aIcon)
            {
                setFlags(ButtonTypeToInt( ButtonType::eRadioButton) | ButtonTypeToInt(ButtonType::eToggleButton) );
                setFixedSize(glm::ivec2(25, 25));
            }

            /// Save the state of the tabwidget into the given \ref Serializer instance
            void Serialize(Json::Value& aSerializer) const override
            {
                UIButton::Serialize(aSerializer);
            }

            /// Restore the state of the tabwidget from the given \ref Serializer instance
            void Deserialize(const Json::Value& aSerializer) override
            {
                UIButton::Deserialize(aSerializer);
            }

        //public:
        //    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };
    }
}
