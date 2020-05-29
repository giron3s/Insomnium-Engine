/*
    nanogui/colorwheel.h -- fancy analog widget to select a color value

    This widget was contributed by Dmitriy Morozov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/**
 * \file nanogui/colorwheel.h
 *
 * \brief Fancy analog widget to select a color value.  This widget was
 *        contributed by Dmitriy Morozov.
 */

#pragma once

#include "graphic/gui/widget.h"

namespace Framework
{
    namespace GUI
    {
        /**
        * \class ColorWheel colorwheel.h nanogui/colorwheel.h
        *
        * \brief Fancy analog widget to select a color value.  This widget was
        *        contributed by Dmitriy Morozov.
        */
        class UIColorWheel : public UIWidget 
        {
        public:

            UIColorWheel();

            /**
              * Adds a ColorWheel to the specified parent.
              *
              * \param parent
              *     The Widget to add this ColorWheel to.
              *
              * \param color
              *     The initial color of the ColorWheel (default: Red).
              */
            UIColorWheel(UIWidget *aParent, const Color& aColor = Color(1.0f, 0.0f, 0.0f, 1.0f));

            /// The callback to execute when a user changes the ColorWheel value.
            std::function<void(const Color &)> callback() const { return mCallback; }

            /// Sets the callback to execute when a user changes the ColorWheel value.
            void setCallback(const std::function<void(const Color &)> &callback) { mCallback = callback; }

            /// The current Color this ColorWheel has selected.
            Color color() const;

            /// Sets the current Color this ColorWheel has selected.
            void setColor(const Color& color);

            /// The preferred size of this ColorWheel.
            virtual glm::ivec2 preferredSize(NVGcontext *ctx) const override;

            /// Draws the ColorWheel.
            virtual void draw(NVGcontext *ctx) override;

            /// Handles mouse button click events for the ColorWheel.
            virtual bool mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers) override;

            /// Handles mouse drag events for the ColorWheel.
            virtual bool mouseDragEvent(const glm::ivec2 &p, const glm::ivec2 &rel, int button, int modifiers) override;

            /// Saves the current state of this ColorWheel to the specified Serializer.
            virtual void Serialize(Json::Value& aSerializer) const override;

            /// Sets the state of this ColorWheel using the specified Serializer.
            virtual void Deserialize(const Json::Value& aSerializer) override;


        private:

            // Used to describe where the mouse is interacting
            enum Region
            {
                None = 0,
                InnerTriangle = 1,
                OuterCircle = 2,
                Both = 3
            };

            // Converts a specified hue (with saturation = value = 1) to RGB space.
            Color hue2rgb(float h) const;

            // Manipulates the positioning of the different regions of the ColorWheel.
            Region adjustPosition(const glm::ivec2 &p, Region consideredRegions = Both);

        protected:
            float                              mHue;        /// The current Hue in the HSV color model.
            float                              mWhite;      /// The implicit Value component of the HSV color model. Valid values are in the range [0, 1].
            float                              mBlack;      /// The implicit Saturation component of the HSV color model. Valid values are in the range [0, 1].
            Region                             mDragRegion; /// The current region the mouse is interacting with.
            std::function<void(const Color &)> mCallback;   /// The current callback to execute when the color value has changed.
        //public:
        //    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };
    }
}
