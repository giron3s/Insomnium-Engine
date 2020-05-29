/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

#include "graphic/gui/widget.h"
#include <vector>
#include <string>
#include <functional>
#include <utility>
#include <iterator>

namespace Game
{
    class UICatalogFilter : public Framework::GUI::UIWidget
    {
    public:
        UICatalogFilter(UIWidget *aParent);
    
        bool overflowing() const { return mOverflowing; }
    
        /**
        * Sets the callable objects which is invoked when a tab button is pressed.
        * The argument provided to the callback is the index of the tab.
        */
        void setCallback(const std::function<void(int)>& callback) { mCallback = callback; };
        const std::function<void(int)>& callback() const { return mCallback; }
    
        void setActiveTab(int tabIndex);
        int activeTab() const;
        bool isTabVisible(int index) const;
        int tabCount() const { return (int)mTabButtons.size(); }
    
        /// Inserts a tab at the end of the tabs collection.
        void addTab(const int& aIcon);

        /// Inserts a tab into the tabs collection at the specified index.
        void addTab(int index, const int& aIcon);

        /// Removes the tab with the specified index.
        void removeTab(int index);
    
        /**
        * Recalculate the visible range of tabs so that the tab with the specified
        * index is visible. The tab with the specified index will either be the
        * first or last visible one depending on the position relative to the
        * old visible range.
        */
        void ensureTabVisible(int index);
    
        /**
        * Returns a pair of Vectors describing the top left (pair.first) and the
        * bottom right (pair.second) positions of the rectangle containing the visible tab buttons.
        */
        std::pair<glm::ivec2, glm::ivec2> visibleButtonArea() const;
    
        /**
        * Returns a pair of Vectors describing the top left (pair.first) and the
        * bottom right (pair.second) positions of the rectangle containing the active tab button.
        * Returns two zero vectors if the active button is not visible.
        */
        std::pair<glm::ivec2, glm::ivec2> activeButtonArea() const;
    
        virtual void performLayout(NVGcontext* ctx) override;
        virtual glm::ivec2 preferredSize(NVGcontext* ctx) const override;
        virtual bool mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers) override;
    
        virtual void draw(NVGcontext* ctx) override;
    
    private:
        /**
        * \class UIFilterButton catalogfilter.h
        *
        * \brief Implementation class of the filter tab buttons.
        */
        class UIFilterButton
        {
        public:
            UIFilterButton(UICatalogFilter& aHeader, const int aIcon = 0);

            int icon() const { return mIcon; }
            void setIcon(int icon) { mIcon = icon; }

            void setSize(const glm::ivec2& size) { mSize = size; }
            const glm::ivec2& size() const { return mSize; }

            glm::ivec2 preferredSize(NVGcontext* ctx) const;
            void drawAtPosition(NVGcontext* ctx, const glm::ivec2& position, bool active);
            void drawActiveBorderAt(NVGcontext * ctx, const glm::ivec2& position, float offset, const Framework::GUI::Color& color);
            void drawInactiveBorderAt(NVGcontext * ctx, const glm::ivec2& position, float offset, const Framework::GUI::Color& color);

        private:
            UICatalogFilter* mHeader;
            int              mIcon;
            glm::ivec2       mSize;
        };

        using TabIterator = std::vector<UIFilterButton>::iterator;
        using ConstTabIterator = std::vector<UIFilterButton>::const_iterator;
    
        TabIterator visibleBegin() { return std::next(mTabButtons.begin(), mVisibleStart); }
        TabIterator visibleEnd() { return std::next(mTabButtons.begin(), mVisibleEnd); }
        TabIterator activeIterator() { return std::next(mTabButtons.begin(), mActiveTab); }
        TabIterator tabIterator(int index) { return std::next(mTabButtons.begin(), index); }
    
        ConstTabIterator visibleBegin() const { return std::next(mTabButtons.begin(), mVisibleStart); }
        ConstTabIterator visibleEnd() const { return std::next(mTabButtons.begin(), mVisibleEnd); }
        ConstTabIterator activeIterator() const { return std::next(mTabButtons.begin(), mActiveTab); }
        ConstTabIterator tabIterator(int index) const { return std::next(mTabButtons.begin(), index); }
    
        /// Given the beginning of the visible tabs, calculate the end.
        void calculateVisibleEnd();
    
        std::function<void(int)>    mCallback;
        std::vector<UIFilterButton> mTabButtons;
        int                         mVisibleStart = 0;
        int                         mVisibleEnd = 0;
        int                         mActiveTab = 0;
        bool                        mOverflowing = false;
    };
}
