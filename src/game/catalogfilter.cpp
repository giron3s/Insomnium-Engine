/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "catalogfilter.h"
#include "graphic/gui/theme.h"
#include "graphic/gui/entypo.h"
#include <numeric>

using namespace Framework;
using namespace Framework::GUI;

namespace Game
{

    //***********************************************************************
    //
    // UIFilterButton class
    //
    //***********************************************************************
    UICatalogFilter::UIFilterButton::UIFilterButton(UICatalogFilter &aHeader, int aIcon)
        : mHeader(&aHeader)
        , mIcon(aIcon)
    {

    }


    glm::ivec2 UICatalogFilter::UIFilterButton::preferredSize(NVGcontext *ctx) const
    {
        float lButtonHeight, lButtonWidth;

        //Size of the icon
        if (nvgIsFontIcon(mIcon))
        {
            float lFontSize = 30;
            nvgFontSize(ctx, lFontSize);
            lButtonHeight = lFontSize * 1.5f;
            nvgFontFace(ctx, "icons");
            nvgFontSize(ctx, lButtonHeight);
            lButtonWidth = nvgTextBounds(ctx, 0, 0, utf8(mIcon).data(), nullptr, nullptr) + mSize.y * 0.15f;
        }
        else
        {
            int w, h;
            float lFontSize = 30;
            nvgFontSize(ctx, lFontSize);
            lButtonHeight = lFontSize * 0.9f;
            nvgImageSize(ctx, mIcon, &w, &h);
            lButtonWidth = w * lButtonHeight / h;
        }
        return glm::ivec2(lButtonWidth, lButtonHeight);
    }

    void UICatalogFilter::UIFilterButton::drawAtPosition(NVGcontext *ctx, const glm::ivec2& position, bool active) 
    {
        float lPosX = static_cast<float>(position.x);
        float lPosY = static_cast<float>(position.y);
        float lWidth = static_cast<float>(mSize.x);
        float lHeight = static_cast<float>(mSize.y);
        UITheme* theme = mHeader->theme();

        nvgSave(ctx);
        nvgIntersectScissor(ctx, lPosX, lPosY, lWidth + 1.0f, lHeight);
        if (!active)
        {
            // Background gradients
            NVGcolor lGradTop = theme->mButtonGradientTopPushed;
            NVGcolor lGradBot = theme->mButtonGradientBotPushed;

            // Draw the background.
            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, lPosX + 1.0f, lPosY + 1.0f, lWidth - 1.0f, lHeight + 1.0f, static_cast<float>(theme->mButtonCornerRadius));
            NVGpaint backgroundColor = nvgLinearGradient(ctx, lPosX, lPosY, lPosX, lPosY + lHeight, lGradTop, lGradBot);
            nvgFillPaint(ctx, backgroundColor);
            nvgFill(ctx);
        }

        if (active)
        {
            nvgBeginPath(ctx);
            nvgStrokeWidth(ctx, 1.0f);
            nvgRoundedRect(ctx, lPosX + 0.5f, lPosY + 1.5f, lWidth, lHeight + 1.0f, static_cast<float>(theme->mButtonCornerRadius));
            nvgStrokeColor(ctx, theme->mBorderLight);
            nvgStroke(ctx);

            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, lPosX + 0.5f, lPosY + 0.5f, lWidth, lHeight + 1.0f, static_cast<float>(theme->mButtonCornerRadius));
            nvgStrokeColor(ctx, theme->mBorderDark);
            nvgStroke(ctx);
        }
        else
        {
            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, lPosX + 0.5f, lPosY + 1.5f, lWidth, lHeight, static_cast<float>(theme->mButtonCornerRadius));
            nvgStrokeColor(ctx, theme->mBorderDark);
            nvgStroke(ctx);
        }
        nvgResetScissor(ctx);
        nvgRestore(ctx);

        //Draw the icon
        if (mIcon && nvgIsFontIcon(mIcon))
        {
            glm::vec2 lIconPos = glm::vec2(lPosX, lPosY) + glm::vec2(lWidth, lHeight) * 0.5f;
            auto lIcon = utf8(mIcon);
            float ih = 30.f;
            float iw = 0.f;

            nvgFontSize(ctx, ih);
            nvgFontFace(ctx, "icons");
            iw = nvgTextBounds(ctx, 0, 0, lIcon.data(), nullptr, nullptr);
            nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
            nvgText(ctx, lIconPos.x - iw /2.f, lIconPos.y + 1, lIcon.data(), nullptr);
        }
    }

    void UICatalogFilter::UIFilterButton::drawActiveBorderAt(NVGcontext *ctx, const glm::ivec2 &position, float offset, const Color &color) 
    {
        int xPos = position.x;
        int yPos = position.y;
        int width = mSize.x;
        int height = mSize.y;
        nvgBeginPath(ctx);
        nvgLineJoin(ctx, NVG_ROUND);
        nvgMoveTo(ctx, xPos + offset, yPos + height + offset);
        nvgLineTo(ctx, xPos + offset, yPos + offset);
        nvgLineTo(ctx, xPos + width - offset, yPos + offset);
        nvgLineTo(ctx, xPos + width - offset, yPos + height + offset);
        nvgStrokeColor(ctx, color);
        nvgStrokeWidth(ctx, mHeader->theme()->mTabBorderWidth);
        nvgStroke(ctx);
    }

    void UICatalogFilter::UIFilterButton::drawInactiveBorderAt(NVGcontext *aCtx, const glm::ivec2 &aPosition, float aOffset, const Color& aColor) 
    {
        float lPosX = static_cast<float>(aPosition.x);
        float lPosY = static_cast<float>(aPosition.y);
        float lWidth = static_cast<float>(mSize.x);
        float lHeight = static_cast<float>(mSize.y);

        nvgBeginPath(aCtx);
        nvgRoundedRect(aCtx, lPosX + aOffset, lPosY + aOffset, lWidth - aOffset, lHeight - aOffset, static_cast<float>(mHeader->theme()->mButtonCornerRadius));
        nvgStrokeColor(aCtx, aColor);
        nvgStroke(aCtx);
    }


    //***********************************************************************
    //
    // UICatalogFilter class
    //
    //***********************************************************************
    UICatalogFilter::UICatalogFilter(UIWidget* aParent)
        : UIWidget(aParent)
    {

    }

    void UICatalogFilter::setActiveTab(int tabIndex)
    {
        ASSERT(tabIndex < tabCount());
        mActiveTab = tabIndex;

        if (mCallback)
        {
            mCallback(tabIndex);
        }
    }

    int UICatalogFilter::activeTab() const
    {
        return mActiveTab;
    }

    bool UICatalogFilter::isTabVisible(int index) const
    {
        return index >= mVisibleStart && index < mVisibleEnd;
    }

    void UICatalogFilter::addTab(const int& aIcon)
    {
        addTab(tabCount(), aIcon);
    }

    void UICatalogFilter::addTab(int index, const int& aIcon)
    {
        assert(index <= tabCount());
        mTabButtons.insert(next(mTabButtons.begin(), index), UIFilterButton(*this, aIcon));
        setActiveTab(index);
    }

    void UICatalogFilter::removeTab(int index)
    {
        assert(index < tabCount());
        mTabButtons.erase(next(mTabButtons.begin(), index));
        if (index == mActiveTab && index != 0)
            setActiveTab(index - 1);
    }

    void UICatalogFilter::ensureTabVisible(int index)
    {
        auto visibleArea = visibleButtonArea();
        auto visibleWidth = visibleArea.second.x - visibleArea.first.x;
        int allowedVisibleWidth = mSize.x - 2 * theme()->mTabControlWidth;
        assert(allowedVisibleWidth >= visibleWidth);
        assert(index >= 0 && index < (int)mTabButtons.size());

        auto first = visibleBegin();
        auto last = visibleEnd();
        auto goal = tabIterator(index);

        // Reach the goal tab with the visible range.
        if (goal < first)
        {
            do {
                --first;
                visibleWidth += first->size().x;
            } while (goal < first);
            while (allowedVisibleWidth < visibleWidth) {
                --last;
                visibleWidth -= last->size().x;
            }
        }
        else if (goal >= last)
        {
            do
            {
                visibleWidth += last->size().x;
                ++last;
            } while (goal >= last);
            while (allowedVisibleWidth < visibleWidth) {
                visibleWidth -= first->size().x;
                ++first;
            }
        }

        // Check if it is possible to expand the visible range on either side.
        while (first != mTabButtons.begin() && next(first, -1)->size().x < allowedVisibleWidth - visibleWidth)
        {
            --first;
            visibleWidth += first->size().x;
        }
        while (last != mTabButtons.end() && last->size().x < allowedVisibleWidth - visibleWidth) {
            visibleWidth += last->size().x;
            ++last;
        }

        mVisibleStart = (int)distance(mTabButtons.begin(), first);
        mVisibleEnd = (int)distance(mTabButtons.begin(), last);
    }

    pair<glm::ivec2, glm::ivec2> UICatalogFilter::visibleButtonArea() const
    {
        if (mVisibleStart == mVisibleEnd)
            return { glm::ivec2(), glm::ivec2() };
        auto topLeft = mPos + glm::ivec2(theme()->mTabControlWidth, 0);
        auto width = accumulate(visibleBegin(), visibleEnd(), theme()->mTabControlWidth,
            [](int acc, const UIFilterButton& tb) {
            return acc + tb.size().x;
        });
        auto bottomRight = mPos + glm::ivec2(width, mSize.y);
        return { topLeft, bottomRight };
    }

    pair<glm::ivec2, glm::ivec2> UICatalogFilter::activeButtonArea() const
    {
        if (mVisibleStart == mVisibleEnd || mActiveTab < mVisibleStart || mActiveTab >= mVisibleEnd)
            return { glm::ivec2(), glm::ivec2() };
        auto width = accumulate(visibleBegin(), activeIterator(), theme()->mTabControlWidth,
            [](int acc, const UIFilterButton& tb) {
            return acc + tb.size().x;
        });
        auto topLeft = mPos + glm::ivec2(width, 0);
        auto bottomRight = mPos + glm::ivec2(width + activeIterator()->size().x, mSize.y);
        return { topLeft, bottomRight };
    }

    void UICatalogFilter::performLayout(NVGcontext* aCtx)
    {
        UIWidget::performLayout(aCtx);

        glm::ivec2 lCurrentPosition = glm::ivec2();

        // Place the tab buttons relative to the beginning of the tab header.
        for (auto& lTab : mTabButtons)
        {
            auto tabPreferred = lTab.preferredSize(aCtx);
            if (tabPreferred.x < theme()->mTabMinButtonWidth)
                tabPreferred.x = theme()->mTabMinButtonWidth;
            else if (tabPreferred.x > theme()->mTabMaxButtonWidth)
                tabPreferred.x = theme()->mTabMaxButtonWidth;
            lTab.setSize(tabPreferred);

            lCurrentPosition.x += tabPreferred.x;
        }
        calculateVisibleEnd();

        if (mVisibleStart != 0 || mVisibleEnd != tabCount())
        {
            WARNING("Overflowing the filter button's size");
            mOverflowing = true;
        }
    }

    glm::ivec2 UICatalogFilter::preferredSize(NVGcontext* ctx) const
    {
        // Set up the nvg context for measuring the text inside the tab buttons.
        nvgFontSize(ctx, static_cast<float>(fontSize()));
        nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        glm::ivec2 size = glm::ivec2(2 * theme()->mTabControlWidth, 0);
        for (auto& tab : mTabButtons)
        {
            auto tabPreferred = tab.preferredSize(ctx);
            if (tabPreferred.x < theme()->mTabMinButtonWidth)
                tabPreferred.x = theme()->mTabMinButtonWidth;
            else if (tabPreferred.x > theme()->mTabMaxButtonWidth)
                tabPreferred.x = theme()->mTabMaxButtonWidth;
            size.x += tabPreferred.x;
            size.y = max(size.y, tabPreferred.y);
        }
        return size;
    }

    bool UICatalogFilter::mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers)
    {
        if (button == GLFW_MOUSE_BUTTON_1 && down)
        {
            auto first = visibleBegin();
            auto last = visibleEnd();
            int currentPosition = theme()->mTabControlWidth;
            int endPosition = p.x;
            auto firstInvisible = find_if(first, last,
                [&currentPosition, endPosition](const UIFilterButton& tb) {
                currentPosition += tb.size().x;
                return currentPosition > endPosition;
            });

            // Did not click on any of the tab buttons
            if (firstInvisible == last)
                return true;

            // Update the active tab and invoke the callback.
            setActiveTab((int)distance(mTabButtons.begin(), firstInvisible));
            return true;
        }
        return false;
    }

    void UICatalogFilter::draw(NVGcontext* ctx)
    {
        // Draw controls.
        UIWidget::draw(ctx);
        if (mOverflowing)
        {
            WARNING("Overflowing the filter button's size");
        }

        // Set up common text drawing settings.
        nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);

        TabIterator lCurrent = visibleBegin();
        TabIterator lLast = visibleEnd();
        TabIterator lActive = next(mTabButtons.begin(), mActiveTab);
        glm::ivec2 currentPosition = mPos + glm::ivec2(theme()->mTabControlWidth, 0);

        // Flag to draw the active tab last. Looks a little bit better.
        bool drawActive = false;
        glm::ivec2 activePosition = glm::ivec2();

        // Draw inactive visible buttons.
        while (lCurrent != lLast)
        {
            if (lCurrent == lActive)
            {
                drawActive = true;
                activePosition = currentPosition;
            }
            else
            {
                lCurrent->drawAtPosition(ctx, currentPosition, false);
            }
            currentPosition.x += lCurrent->size().x;
            ++lCurrent;
        }

        // Draw active visible button.
        if (drawActive)
            lActive->drawAtPosition(ctx, activePosition, true);
    }

    void UICatalogFilter::calculateVisibleEnd()
    {
        auto first = visibleBegin();
        auto last = mTabButtons.end();
        int currentPosition = theme()->mTabControlWidth;
        int lastPosition = mSize.x - theme()->mTabControlWidth;
        auto firstInvisible = find_if(first, last,
            [&currentPosition, lastPosition](const UIFilterButton& tb) {
            currentPosition += tb.size().x;
            return currentPosition > lastPosition;
        });
        mVisibleEnd = (int)distance(mTabButtons.begin(), firstInvisible);
    }
}
