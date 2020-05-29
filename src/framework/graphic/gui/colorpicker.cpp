/*
    src/UIColorPicker.cpp -- push button with a popup to tweak a color value

    This widget was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
#include "precompiled.h"

#include "json/json.h"
#include "graphic/gui/colorpicker.h"
#include "graphic/gui/layout.h"
#include "graphic/gui/colorwheel.h"

#include "engine.h"

namespace Framework
{
    namespace GUI
    {

        UIColorPicker::UIColorPicker()
            : UIPopupButton()
            , mColorWheel(nullptr)
            , mPickButton(nullptr)
            , mResetButton(nullptr)
        {
        }

        UIColorPicker::UIColorPicker(UIWidget *parent, const Color& color)
            : UIPopupButton(parent, "")
            , mColorWheel(nullptr)
            , mPickButton(nullptr)
            , mResetButton(nullptr)
        {
            setBackgroundColor(color);
            createColorPicker();
        }

        void UIColorPicker::createColorPicker()
        {
            createPopup();
            UIPopup *popup = this->popup();
            popup->setLayout( new UIGroupLayout() );

            // initialize callback to do nothing; this is for users to hook into
            // receiving a new color value
            mCallback = [](const Color &) {};
            mFinalCallback = [](const Color & c) 
            {
                string lColorString = "" + std::to_string(c.r()) + "," + std::to_string(c.g()) + "," + std::to_string(c.b()) + "," + std::to_string(c.a());
                Engine::Instance()->StateMachine().ExecuteActionWithArgs("OnColorPicked", lColorString); 
            };

            // set the color wheel to the specified color
            mColorWheel = new UIColorWheel(popup, mBackgroundColor);

            // set the pick button to the specified color
            mPickButton = new UIButton(popup, "@PICK");
            mPickButton->setBackgroundColor(mBackgroundColor);
            mPickButton->setTextColor(mBackgroundColor.contrastingColor());
            mPickButton->setFixedSize(glm::ivec2(100, 20));

            // set the reset button to the specified color
            mResetButton = new UIButton(popup, "@RESET");
            mResetButton->setBackgroundColor(mBackgroundColor);
            mResetButton->setTextColor(mBackgroundColor.contrastingColor());
            mResetButton->setFixedSize(glm::ivec2(100, 20));

            UIPopupButton::setChangeCallback([&](bool) {
                if (this->mPickButton->pushed()) {
                    setColor(backgroundColor());
                    mFinalCallback(backgroundColor());
                }
            });

            mColorWheel->setCallback([&](const Color &value) {
                mPickButton->setBackgroundColor(value);
                mPickButton->setTextColor(value.contrastingColor());
                mCallback(value);
            });

            mPickButton->setCallback([this]() {
                if (mPushed) {
                    Color value = mColorWheel->color();
                    setPushed(false);
                    setColor(value);
                    mFinalCallback(value);
                }
            });

            mResetButton->setCallback([this]() {
                Color bg = this->mResetButton->backgroundColor();
                Color fg = this->mResetButton->textColor();

                mColorWheel->setColor(bg);
                mPickButton->setBackgroundColor(bg);
                mPickButton->setTextColor(fg);

                mCallback(bg);
                mFinalCallback(bg);
            });

            setAppearance(UIPopup::Right);
        }

        Color UIColorPicker::color() const
        {
            return backgroundColor();
        }

        void UIColorPicker::setColor(const Color& color)
        {
            /* Ignore setColor() calls when the user is currently editing */
            if (!mPushed) {
                Color fg = color.contrastingColor();
                setBackgroundColor(color);
                setTextColor(fg);
                mColorWheel->setColor(color);

                mPickButton->setBackgroundColor(color);
                mPickButton->setTextColor(fg);

                mResetButton->setBackgroundColor(color);
                mResetButton->setTextColor(fg);
            }
        }

        void UIColorPicker::Serialize(Json::Value& aSerializer) const
        {
        }

        void UIColorPicker::Deserialize(const Json::Value& aSerializer)
        {
            UIPopupButton::Deserialize(aSerializer);

            if (aSerializer.isMember("backgroundColor"))
            {
                Color lColor(aSerializer["backgroundColor"][0].asFloat(),
                             aSerializer["backgroundColor"][1].asFloat(),
                             aSerializer["backgroundColor"][2].asFloat(),
                             aSerializer["backgroundColor"][3].asFloat());
                setBackgroundColor(lColor);
            }

            createColorPicker();
        }
    }
}
