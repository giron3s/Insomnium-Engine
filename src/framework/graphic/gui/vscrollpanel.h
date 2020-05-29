/*
    nanogui/vscrollpanel.h -- Adds a vertical scrollbar around a widget
    that is too big to fit into a certain area

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
        * \class VScrollPanel vscrollpanel.h nanogui/vscrollpanel.h
        *
        * \brief Adds a vertical scrollbar around a widget that is too big to fit into
        *        a certain area.
        */
        class UIVScrollPanel : public UIWidget
        {
        public:
            UIVScrollPanel();

            UIVScrollPanel(UIWidget *aParent);

            /// Return the current scroll amount as a value between 0 and 1. 0 means scrolled to the top and 1 to the bottom.
            float scroll() const { return mScroll; }
            /// Set the scroll amount to a value between 0 and 1. 0 means scrolled to the top and 1 to the bottom.
            void setScroll(float aScroll) { mScroll = aScroll; }

            virtual void performLayout(NVGcontext *ctx) override;
            virtual glm::ivec2 preferredSize(NVGcontext *ctx) const override;
            virtual bool mouseDragEvent(const glm::ivec2 &p, const glm::ivec2 &rel, int button, int modifiers) override;
            virtual bool scrollEvent(const glm::ivec2 &p, const glm::vec2 &rel) override;
            virtual void draw(NVGcontext *ctx) override;

            virtual void Serialize(Json::Value& aSerializer) const override;
            virtual void Deserialize(const Json::Value& aSerializer) override;

        protected:
            int     mChildPreferredHeight;
            float   mScroll;
            bool    mUpdateLayout;
       // public:
       //     EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };
    }
}
