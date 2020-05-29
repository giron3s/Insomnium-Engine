/*
    nanogui/stackedwidget.cpp -- Widget used to stack widgets on top
    of each other. Only the active widget is visible.

    The stacked widget was contributed by Stefan Ivanov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"
#include "graphic/gui/stackedwidget.h"

namespace Framework
{
    namespace GUI
    {
        UIStackedWidget::UIStackedWidget(UIWidget *aParent)
            : UIWidget(aParent) 
        {

        }

        void UIStackedWidget::setSelectedIndex(int index)
        {
            assert(index < childCount());
            if (mSelectedIndex >= 0)
                mChildren[mSelectedIndex]->setVisible(false);
            mSelectedIndex = index;
            mChildren[mSelectedIndex]->setVisible(true);
        }

        int UIStackedWidget::selectedIndex() const 
        {
            return mSelectedIndex;
        }

        void UIStackedWidget::performLayout(NVGcontext *ctx) 
        {
            for (auto child : mChildren) {
                child->setPosition(glm::ivec2());
                child->setSize(mSize);
                child->performLayout(ctx);
            }
        }

        glm::ivec2 UIStackedWidget::preferredSize(NVGcontext *ctx) const {
            glm::ivec2 size = glm::ivec2();
            for (auto child : mChildren)
                size = WiseMax(size, child->preferredSize(ctx));
            return size;
        }

        void UIStackedWidget::addChild(int index, UIWidget *widget) {
            if (mSelectedIndex >= 0)
                mChildren[mSelectedIndex]->setVisible(false);
            UIWidget::addChild(index, widget);
            widget->setVisible(true);
            setSelectedIndex(index);
        }
    }
}

