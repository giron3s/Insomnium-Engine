/*
    nanogui/tabwidget.cpp -- A wrapper around the widgets TabHeader and StackedWidget
    which hooks the two classes together.

    The tab widget was contributed by Stefan Ivanov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"

#include "engine/ui.h"
#include "json/json.h"
#include "graphic/gui/tabwidget.h"
#include "graphic/gui/tabheader.h"
#include "graphic/gui/stackedwidget.h"
#include "graphic/gui/theme.h"
#include "graphic/gui/window.h"
#include "graphic/gui/screen.h"
#include <algorithm>

namespace Framework
{

    namespace GUI
    {

        UITabWidget::UITabWidget()
            : UIWidget(nullptr)
            , mHeader(new UITabHeader(nullptr)) // create using nullptr, add children below
            , mContent(new UIStackedWidget(nullptr))
        {
            // since TabWidget::addChild is going to throw an exception to prevent
            // mis-use of this class, add the child directly
            UIWidget::addChild(childCount(), mHeader);
            UIWidget::addChild(childCount(), mContent);

            mHeader->setCallback([this](int i)
            {
                mContent->setSelectedIndex(i);
                if (mCallback)
                    mCallback(i);
            });
        }

        UITabWidget::UITabWidget(UIWidget* aParent)
            : UIWidget(aParent)
            , mHeader(new UITabHeader(nullptr)) // create using nullptr, add children below
            , mContent(new UIStackedWidget(nullptr)) 
        {
            // since TabWidget::addChild is going to throw an exception to prevent
            // mis-use of this class, add the child directly
            UIWidget::addChild(childCount(), mHeader);
            UIWidget::addChild(childCount(), mContent);

            mHeader->setCallback([this](int i) 
            {
                mContent->setSelectedIndex(i);
                if (mCallback)
                    mCallback(i);
            });
        }

        void UITabWidget::addChild(int /*index*/, UIWidget * /*widget*/) 
        {
            // there may only be two children: mHeader and mContent, created in the constructor
            throw std::runtime_error(
                "TabWidget: do not add children directly to the TabWidget, create tabs "
                "and add children to the tabs.  See TabWidget class documentation for "
                "example usage."
            );
        }

        void UITabWidget::setActiveTab(int tabIndex)
        {
            mHeader->setActiveTab(tabIndex);
            mContent->setSelectedIndex(tabIndex);
        }

        int UITabWidget::activeTab() const {
            assert(mHeader->activeTab() == mContent->selectedIndex());
            return mContent->selectedIndex();
        }

        int UITabWidget::tabCount() const {
            assert(mContent->childCount() == mHeader->tabCount());
            return mHeader->tabCount();
        }

        UIWidget* UITabWidget::createTab(int index, const string &label)
        {
            UIWidget* lTab(new UIWidget());
            addTab(index, label, lTab);
            return lTab;
        }

        UIWidget* UITabWidget::createTab(const string &label) {
            return createTab(tabCount(), label);
        }

        void UITabWidget::addTab(const string &name, UIWidget* aTab) {
            addTab(tabCount(), name, aTab);
        }

        void UITabWidget::addTab(int index, const string &label, UIWidget* aTab) {
            assert(index <= tabCount());
            // It is important to add the content first since the callback
            // of the header will automatically fire when a new tab is added.
            mContent->addChild(index, aTab);
            mHeader->addTab(index, label);
            assert(mHeader->tabCount() == mContent->childCount());
        }

        int UITabWidget::tabLabelIndex(const string &label) {
            return mHeader->tabIndex(label);
        }

        int UITabWidget::tabIndex(UIWidget* tab) {
            return mContent->childIndex(tab);
        }

        void UITabWidget::ensureTabVisible(int index) {
            if (!mHeader->isTabVisible(index))
                mHeader->ensureTabVisible(index);
        }

        const UIWidget *UITabWidget::tab(const string &tabName) const {
            int index = mHeader->tabIndex(tabName);
            if (index == -1 || index == mContent->childCount())
                return nullptr;
            return mContent->children()[index];
        }

        UIWidget *UITabWidget::tab(const string &tabName) {
            int index = mHeader->tabIndex(tabName);
            if (index == -1 || index == mContent->childCount())
                return nullptr;
            return mContent->children()[index];
        }

        const UIWidget *UITabWidget::tab(int index) const {
            if (index < 0 || index >= mContent->childCount())
                return nullptr;
            return mContent->children()[index];
        }

        UIWidget *UITabWidget::tab(int index) {
            if (index < 0 || index >= mContent->childCount())
                return nullptr;
            return mContent->children()[index];
        }

        bool UITabWidget::removeTab(const string &tabName) {
            int index = mHeader->removeTab(tabName);
            if (index == -1)
                return false;
            mContent->removeChild(index);
            return true;
        }

        void UITabWidget::removeTab(int index) {
            assert(mContent->childCount() < index);
            mHeader->removeTab(index);
            mContent->removeChild(index);
            if (activeTab() == index)
                setActiveTab(index == (index - 1) ? index - 1 : 0);
        }

        const string &UITabWidget::tabLabelAt(int index) const {
            return mHeader->tabLabelAt(index);
        }

        void UITabWidget::performLayout(NVGcontext* ctx) {
            int headerHeight = mHeader->preferredSize(ctx).y;
            int margin = mTheme->mTabInnerMargin;
            mHeader->setPosition({ 0, 0 });
            mHeader->setSize({ mSize.x, headerHeight });
            mHeader->performLayout(ctx);
            mContent->setPosition({ margin, headerHeight + margin });
            mContent->setSize({ mSize.x - 2 * margin, mSize.y - 2 * margin - headerHeight });
            mContent->performLayout(ctx);
        }

        glm::ivec2 UITabWidget::preferredSize(NVGcontext* ctx) const {
            auto contentSize = mContent->preferredSize(ctx);
            auto headerSize = mHeader->preferredSize(ctx);
            int margin = mTheme->mTabInnerMargin;
            auto borderSize = glm::ivec2(2 * margin, 2 * margin);
            glm::ivec2 tabPreferredSize = contentSize + borderSize + glm::ivec2(0, headerSize.y);
            return tabPreferredSize;
        }

        void UITabWidget::draw(NVGcontext* ctx) 
        {
            int tabHeight = mHeader->preferredSize(ctx).y;
            auto activeArea = mHeader->activeButtonArea();


            for (int i = 0; i < 3; ++i) {
                nvgSave(ctx);
                if (i == 0)
                    nvgIntersectScissor(ctx, mPos.x, mPos.y, activeArea.first.x + 1, mSize.y);
                else if (i == 1)
                    nvgIntersectScissor(ctx, mPos.x + activeArea.second.x, mPos.y, mSize.x - activeArea.second.x, mSize.y);
                else
                    nvgIntersectScissor(ctx, mPos.x, mPos.y + tabHeight + 2, mSize.x, mSize.y);

                nvgBeginPath(ctx);
                nvgStrokeWidth(ctx, 1.0f);
                nvgRoundedRect(ctx, mPos.x + 0.5f, mPos.y + tabHeight + 1.5f, mSize.x - 1,
                    mSize.y - tabHeight - 2, mTheme->mButtonCornerRadius);
                nvgStrokeColor(ctx, mTheme->mBorderLight);
                nvgStroke(ctx);

                nvgBeginPath(ctx);
                nvgRoundedRect(ctx, mPos.x + 0.5f, mPos.y + tabHeight + 0.5f, mSize.x - 1,
                    mSize.y - tabHeight - 2, mTheme->mButtonCornerRadius);
                nvgStrokeColor(ctx, mTheme->mBorderDark);
                nvgStroke(ctx);
                nvgRestore(ctx);
            }

            UIWidget::draw(ctx);
        }

        void UITabWidget::Serialize(Json::Value& aSerializer) const
        {

        }

        void UITabWidget::Deserialize(const Json::Value& aSerializer)
        {
            if (aSerializer["tabCaptions"].size() != aSerializer["tabContents"].size())
            {
                CRASH("Nº of the element 'tabCaption' is not the same as nº of the 'tabContents'!  (%d-%d)\n", aSerializer["tabCaptions"].size(), aSerializer["tabContents"].size());
                return;
            }
            if (aSerializer.isMember("activeTab") && (static_cast<uint32_t>(aSerializer["activeTab"].asInt()) > aSerializer.size()))
            {
                CRASH("Value of the activeTab(%d) is bigger than the number of the tabContent(%d)", aSerializer["activeTab"].asInt(), aSerializer["tabContents"].size());
                return;
            }


            UIWidget::Deserialize(aSerializer);

            glm::ivec2 lTabSize;
            if (aSerializer.isMember("tabSize"))
            {
                lTabSize.x = aSerializer["tabSize"][0].asInt();
                lTabSize.y = aSerializer["tabSize"][1].asInt();
            }

            const Json::Value lResTabCaptions = aSerializer["tabCaptions"];
            const Json::Value lResTabContents = aSerializer["tabContents"];
            for (int i = 0, lSize = lResTabCaptions.size(); i < lSize; ++i)
            {
                UIWidget* lTabWidget = createTab(lResTabCaptions[i].asString());
                if(aSerializer.isMember("tabSize"))
                    lTabWidget->setSize(lTabSize);
                mOwner->CreateUIWidgetFromData(lResTabContents[i], lTabWidget);
            }

            setActiveTab(aSerializer.get("activeTab", 0).asInt());
        }
    }
}
