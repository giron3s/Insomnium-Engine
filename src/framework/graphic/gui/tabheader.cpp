/*
    nanogui/tabheader.cpp -- Widget used to control tabs.

    The tab header widget was contributed by Stefan Ivanov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"
#include "engine.h"
#include "graphic/gui/tabheader.h"
#include "graphic/gui/theme.h"
#include "graphic/gui/entypo.h"
#include <numeric>

namespace Framework
{
    namespace GUI
    {
        UITabHeader::UITabButton::UITabButton(UITabHeader &header, const std::string &label)
            : mHeader(&header)
        {
            string lManagerName = Utils::Demangling(typeid(LanguageManager).name());
            LanguageManager& lLanguageManager = static_cast<LanguageManager&>(Engine::Instance()->Manager(lManagerName));
            mLabel = lLanguageManager.GetString(label);
        }

        void UITabHeader::UITabButton::setLabel(const std::string &aLabel)
        {
            string lManagerName = Utils::Demangling(typeid(LanguageManager).name());
            LanguageManager& lLanguageManager = static_cast<LanguageManager&>(Engine::Instance()->Manager(lManagerName));
            mLabel = lLanguageManager.GetString(aLabel);
        }

        glm::ivec2 UITabHeader::UITabButton::preferredSize(NVGcontext *ctx) const 
        {
            // No need to call nvg font related functions since this is done by the tab header implementation
            float bounds[4];
            nvgFontSize(ctx, mHeader->theme()->mButtonFontSize);
            int labelWidth = nvgTextBounds(ctx, 0, 0, mLabel.c_str(), nullptr, bounds);
            int buttonWidth = labelWidth + 2 * mHeader->theme()->mTabButtonHorizontalPadding;
            int buttonHeight = bounds[3] - bounds[1] + 2 * mHeader->theme()->mTabButtonVerticalPadding;
            return glm::ivec2(buttonWidth, buttonHeight);
        }

        void UITabHeader::UITabButton::calculateVisibleString(NVGcontext *ctx) 
        {
            // The size must have been set in by the enclosing tab header.
            NVGtextRow displayedText;
            nvgTextBreakLines(ctx, mLabel.c_str(), nullptr, mSize.x, &displayedText, 1);

            // Check to see if the text need to be truncated.
            if (displayedText.next[0]) {
                auto truncatedWidth = nvgTextBounds(ctx, 0.0f, 0.0f,
                    displayedText.start, displayedText.end, nullptr);
                auto dotsWidth = nvgTextBounds(ctx, 0.0f, 0.0f, dots, nullptr, nullptr);
                while ((truncatedWidth + dotsWidth + mHeader->theme()->mTabButtonHorizontalPadding) > mSize.x
                    && displayedText.end != displayedText.start) {
                    --displayedText.end;
                    truncatedWidth = nvgTextBounds(ctx, 0.0f, 0.0f,
                        displayedText.start, displayedText.end, nullptr);
                }

                // Remember the truncated width to know where to display the dots.
                mVisibleWidth = truncatedWidth;
                mVisibleText.last = displayedText.end;
            }
            else {
                mVisibleText.last = nullptr;
                mVisibleWidth = 0;
            }
            mVisibleText.first = displayedText.start;
        }

        void UITabHeader::UITabButton::drawAtPosition(NVGcontext *ctx, const glm::ivec2& position, bool active) {
            int xPos = position.x;
            int yPos = position.y;
            int width = mSize.x;
            int height = mSize.y;
            auto theme = mHeader->theme();

            nvgSave(ctx);
            nvgIntersectScissor(ctx, xPos, yPos, width + 1, height);
            if (!active) {
                // Background gradients
                NVGcolor gradTop = theme->mButtonGradientTopPushed;
                NVGcolor gradBot = theme->mButtonGradientBotPushed;

                // Draw the background.
                nvgBeginPath(ctx);
                nvgRoundedRect(ctx, xPos + 1, yPos + 1, width - 1, height + 1,
                    theme->mButtonCornerRadius);
                NVGpaint backgroundColor = nvgLinearGradient(ctx, xPos, yPos, xPos, yPos + height,
                    gradTop, gradBot);
                nvgFillPaint(ctx, backgroundColor);
                nvgFill(ctx);
            }

            if (active) {
                nvgBeginPath(ctx);
                nvgStrokeWidth(ctx, 1.0f);
                nvgRoundedRect(ctx, xPos + 0.5f, yPos + 1.5f, width,
                    height + 1, theme->mButtonCornerRadius);
                nvgStrokeColor(ctx, theme->mBorderLight);
                nvgStroke(ctx);

                nvgBeginPath(ctx);
                nvgRoundedRect(ctx, xPos + 0.5f, yPos + 0.5f, width,
                    height + 1, theme->mButtonCornerRadius);
                nvgStrokeColor(ctx, theme->mBorderDark);
                nvgStroke(ctx);
            }
            else {
                nvgBeginPath(ctx);
                nvgRoundedRect(ctx, xPos + 0.5f, yPos + 1.5f, width,
                    height, theme->mButtonCornerRadius);
                nvgStrokeColor(ctx, theme->mBorderDark);
                nvgStroke(ctx);
            }
            nvgResetScissor(ctx);
            nvgRestore(ctx);

            // Draw the text with some padding
            int textX = xPos + mHeader->theme()->mTabButtonHorizontalPadding;
            int textY = yPos + mHeader->theme()->mTabButtonVerticalPadding;
            NVGcolor textColor = mHeader->theme()->mTextColor;
            nvgBeginPath(ctx);
            nvgFillColor(ctx, textColor);
            nvgFontSize(ctx, mHeader->theme()->mButtonFontSize);
            nvgText(ctx, textX, textY, mVisibleText.first, mVisibleText.last);
            if (mVisibleText.last != nullptr)
                nvgText(ctx, textX + mVisibleWidth, textY, dots, nullptr);
        }

        void UITabHeader::UITabButton::drawActiveBorderAt(NVGcontext *ctx, const glm::ivec2 &position,
            float offset, const Color &color) {
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

        void UITabHeader::UITabButton::drawInactiveBorderAt(NVGcontext *ctx, const glm::ivec2 &position,
            float offset, const Color& color) {
            int xPos = position.x;
            int yPos = position.y;
            int width = mSize.x;
            int height = mSize.y;
            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, xPos + offset, yPos + offset, width - offset, height - offset,
                mHeader->theme()->mButtonCornerRadius);
            nvgStrokeColor(ctx, color);
            nvgStroke(ctx);
        }


        UITabHeader::UITabHeader(UIWidget* aParent, const std::string& aFont)
            : UIWidget(aParent)
            , mFont(aFont) 
        {

        }

        void UITabHeader::setActiveTab(int tabIndex) 
        {
            assert(tabIndex < tabCount());
            mActiveTab = tabIndex;
            if (mCallback)
                mCallback(tabIndex);
        }

        int UITabHeader::activeTab() const 
        {
            return mActiveTab;
        }

        bool UITabHeader::isTabVisible(int index) const 
        {
            return index >= mVisibleStart && index < mVisibleEnd;
        }

        void UITabHeader::addTab(const std::string & label) 
        {
            addTab(tabCount(), label);
        }

        void UITabHeader::addTab(int index, const std::string &label) 
        {
            assert(index <= tabCount());
            mTabButtons.insert(std::next(mTabButtons.begin(), index), UITabButton(*this, label));
            setActiveTab(index);
        }

        int UITabHeader::removeTab(const std::string &label)
        {
            auto element = std::find_if(mTabButtons.begin(), mTabButtons.end(),
                [&](const UITabButton& tb) { return label == tb.label(); });
            int index = (int)std::distance(mTabButtons.begin(), element);
            if (element == mTabButtons.end())
                return -1;
            mTabButtons.erase(element);
            if (index == mActiveTab && index != 0)
                setActiveTab(index - 1);
            return index;
        }

        void UITabHeader::removeTab(int index)
        {
            assert(index < tabCount());
            mTabButtons.erase(std::next(mTabButtons.begin(), index));
            if (index == mActiveTab && index != 0)
                setActiveTab(index - 1);
        }

        const std::string& UITabHeader::tabLabelAt(int index) const 
        {
            assert(index < tabCount());
            return mTabButtons[index].label();
        }

        int UITabHeader::tabIndex(const std::string &label) {
            auto it = std::find_if(mTabButtons.begin(), mTabButtons.end(),
                [&](const UITabButton& tb) { return label == tb.label(); });
            if (it == mTabButtons.end())
                return -1;
            return (int)(it - mTabButtons.begin());
        }

        void UITabHeader::ensureTabVisible(int index) 
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
            if (goal < first) {
                do {
                    --first;
                    visibleWidth += first->size().x;
                } while (goal < first);
                while (allowedVisibleWidth < visibleWidth) {
                    --last;
                    visibleWidth -= last->size().x;
                }
            }
            else if (goal >= last) {
                do {
                    visibleWidth += last->size().x;
                    ++last;
                } while (goal >= last);
                while (allowedVisibleWidth < visibleWidth) {
                    visibleWidth -= first->size().x;
                    ++first;
                }
            }

            // Check if it is possible to expand the visible range on either side.
            while (first != mTabButtons.begin()
                && std::next(first, -1)->size().x < allowedVisibleWidth - visibleWidth) {
                --first;
                visibleWidth += first->size().x;
            }
            while (last != mTabButtons.end()
                && last->size().x < allowedVisibleWidth - visibleWidth) {
                visibleWidth += last->size().x;
                ++last;
            }

            mVisibleStart = (int)std::distance(mTabButtons.begin(), first);
            mVisibleEnd = (int)std::distance(mTabButtons.begin(), last);
        }

        std::pair<glm::ivec2, glm::ivec2> UITabHeader::visibleButtonArea() const {
            if (mVisibleStart == mVisibleEnd)
                return { glm::ivec2(), glm::ivec2() };
            auto topLeft = mPos + glm::ivec2(theme()->mTabControlWidth, 0);
            auto width = std::accumulate(visibleBegin(), visibleEnd(), theme()->mTabControlWidth,
                [](int acc, const UITabButton& tb) {
                return acc + tb.size().x;
            });
            auto bottomRight = mPos + glm::ivec2(width, mSize.y);
            return { topLeft, bottomRight };
        }

        std::pair<glm::ivec2, glm::ivec2> UITabHeader::activeButtonArea() const {
            if (mVisibleStart == mVisibleEnd || mActiveTab < mVisibleStart || mActiveTab >= mVisibleEnd)
                return { glm::ivec2(), glm::ivec2() };
            auto width = std::accumulate(visibleBegin(), activeIterator(), theme()->mTabControlWidth,
                [](int acc, const UITabButton& tb) {
                return acc + tb.size().x;
            });
            auto topLeft = mPos + glm::ivec2(width, 0);
            auto bottomRight = mPos + glm::ivec2(width + activeIterator()->size().x, mSize.y);
            return { topLeft, bottomRight };
        }

        void UITabHeader::performLayout(NVGcontext* ctx) {
            UIWidget::performLayout(ctx);

            glm::ivec2 currentPosition = glm::ivec2();
            // Place the tab buttons relative to the beginning of the tab header.
            for (auto& tab : mTabButtons) {
                auto tabPreferred = tab.preferredSize(ctx);
                if (tabPreferred.x < theme()->mTabMinButtonWidth)
                    tabPreferred.x = theme()->mTabMinButtonWidth;
                else if (tabPreferred.x > theme()->mTabMaxButtonWidth)
                    tabPreferred.x = theme()->mTabMaxButtonWidth;
                tab.setSize(tabPreferred);
                tab.calculateVisibleString(ctx);
                currentPosition.x += tabPreferred.x;
            }
            calculateVisibleEnd();
            if (mVisibleStart != 0 || mVisibleEnd != tabCount())
                mOverflowing = true;
        }

        glm::ivec2 UITabHeader::preferredSize(NVGcontext* ctx) const {
            // Set up the nvg context for measuring the text inside the tab buttons.
            nvgFontFace(ctx, mTheme->mTabHeadingFont.c_str());
            nvgFontSize(ctx, fontSize());
            nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
            glm::ivec2 size = glm::ivec2(2 * theme()->mTabControlWidth, 0);
            for (auto& tab : mTabButtons) {
                auto tabPreferred = tab.preferredSize(ctx);
                if (tabPreferred.x < theme()->mTabMinButtonWidth)
                    tabPreferred.x = theme()->mTabMinButtonWidth;
                else if (tabPreferred.x > theme()->mTabMaxButtonWidth)
                    tabPreferred.x = theme()->mTabMaxButtonWidth;
                size.x += tabPreferred.x;
                size.y = std::max(size.y, tabPreferred.y);
            }
            return size;
        }

        bool UITabHeader::mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers)
        {
            UIWidget::mouseButtonEvent(p, button, down, modifiers);
            if (button == GLFW_MOUSE_BUTTON_1 && down) {
                switch (locateClick(p)) {
                case ClickLocation::LeftControls:
                    onArrowLeft();
                    return true;
                case ClickLocation::RightControls:
                    onArrowRight();
                    return true;
                case ClickLocation::TabButtons:
                    auto first = visibleBegin();
                    auto last = visibleEnd();
                    int currentPosition = theme()->mTabControlWidth;
                    int endPosition = p.x;
                    auto firstInvisible = std::find_if(first, last,
                        [&currentPosition, endPosition](const UITabButton& tb) {
                        currentPosition += tb.size().x;
                        return currentPosition > endPosition;
                    });

                    // Did not click on any of the tab buttons
                    if (firstInvisible == last)
                        return true;

                    // Update the active tab and invoke the callback.
                    setActiveTab((int)std::distance(mTabButtons.begin(), firstInvisible));
                    return true;
                }
            }
            return false;
        }

        void UITabHeader::draw(NVGcontext* ctx) {
            // Draw controls.
            UIWidget::draw(ctx);
            if (mOverflowing)
                drawControls(ctx);

            // Set up common text drawing settings.
            nvgFontFace(ctx, mTheme->mTabHeadingFont.c_str());
            nvgFontSize(ctx, fontSize());
            nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);

            auto current = visibleBegin();
            auto last = visibleEnd();
            auto active = std::next(mTabButtons.begin(), mActiveTab);
            glm::ivec2 currentPosition = mPos + glm::ivec2(theme()->mTabControlWidth, 0);

            // Flag to draw the active tab last. Looks a little bit better.
            bool drawActive = false;
            glm::ivec2 activePosition = glm::ivec2();

            // Draw inactive visible buttons.
            while (current != last) {
                if (current == active) {
                    drawActive = true;
                    activePosition = currentPosition;
                }
                else {
                    current->drawAtPosition(ctx, currentPosition, false);
                }
                currentPosition.x += current->size().x;
                ++current;
            }

            // Draw active visible button.
            if (drawActive)
                active->drawAtPosition(ctx, activePosition, true);
        }

        void UITabHeader::calculateVisibleEnd() {
            auto first = visibleBegin();
            auto last = mTabButtons.end();
            int currentPosition = theme()->mTabControlWidth;
            int lastPosition = mSize.x - theme()->mTabControlWidth;
            auto firstInvisible = std::find_if(first, last,
                [&currentPosition, lastPosition](const UITabButton& tb) {
                currentPosition += tb.size().x;
                return currentPosition > lastPosition;
            });
            mVisibleEnd = (int)std::distance(mTabButtons.begin(), firstInvisible);
        }

        void UITabHeader::drawControls(NVGcontext* ctx) 
        {
            // Left button.
            bool active = mVisibleStart != 0;

            // Draw the arrow.
            nvgBeginPath(ctx);
            auto iconLeft = utf8(mTheme->mTabHeaderLeftIcon);
            int fontSize = mFontSize == -1 ? mTheme->mButtonFontSize : mFontSize;
            float ih = fontSize;
            ih *= 1.5f;
            nvgFontSize(ctx, ih);
            nvgFontFace(ctx, "icons");
            NVGcolor arrowColor;
            if (active)
                arrowColor = mTheme->mTextColor;
            else
                arrowColor = mTheme->mButtonGradientBotPushed;
            nvgFillColor(ctx, arrowColor);
            nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
            float yScaleLeft = 0.5f;
            float xScaleLeft = 0.2f;
            glm::vec2 leftIconPos = glm::vec2(mPos) + glm::vec2(xScaleLeft*theme()->mTabControlWidth, yScaleLeft * static_cast<float>(mSize.y));
            nvgText(ctx, leftIconPos.x, leftIconPos.y + 1, iconLeft.data(), nullptr);

            // Right button.
            active = mVisibleEnd != tabCount();
            // Draw the arrow.
            nvgBeginPath(ctx);
            auto iconRight = utf8(mTheme->mTabHeaderRightIcon);
            fontSize = mFontSize == -1 ? mTheme->mButtonFontSize : mFontSize;
            ih = fontSize;
            ih *= 1.5f;
            nvgFontSize(ctx, ih);
            nvgFontFace(ctx, "icons");
            float rightWidth = nvgTextBounds(ctx, 0, 0, iconRight.data(), nullptr, nullptr);
            if (active)
                arrowColor = mTheme->mTextColor;
            else
                arrowColor = mTheme->mButtonGradientBotPushed;
            nvgFillColor(ctx, arrowColor);
            nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
            float yScaleRight = 0.5f;
            float xScaleRight = 1.0f - xScaleLeft - rightWidth / theme()->mTabControlWidth;
            glm::vec2 rightIconPos = glm::vec2(mPos) + glm::vec2(static_cast<float>(mSize.x), yScaleRight * static_cast<float>(mSize.y)) -
                                     glm::vec2(xScaleRight*theme()->mTabControlWidth + rightWidth, 0);

            nvgText(ctx, rightIconPos.x, rightIconPos.y + 1, iconRight.data(), nullptr);
        }

        UITabHeader::ClickLocation UITabHeader::locateClick(const glm::ivec2& aPos)
        {
            //Left control
            glm::ivec2 lLeftDistance = aPos - mPos;
            bool lHitLeft = true;
            for (glm::length_t i = 0; i < lLeftDistance.length(); ++i)
                lHitLeft &= (lLeftDistance[i] >= 0) && (lLeftDistance[i] < glm::ivec2(theme()->mTabControlWidth, mSize.y)[i]);

            if (lHitLeft)
                return ClickLocation::LeftControls;

            //Right control
            glm::ivec2 lRightDistance = (aPos - (mPos + glm::ivec2(mSize.x - theme()->mTabControlWidth, 0)));
            bool lHitRight = true;
            for (glm::length_t i = 0; i < lRightDistance.length(); ++i)
                lHitRight &= (lRightDistance[i] >= 0) && (lRightDistance[i] < glm::ivec2(theme()->mTabControlWidth, mSize.y)[i]);

            if (lHitRight)
                return ClickLocation::RightControls;

            //Tab button
            return ClickLocation::TabButtons;
        }

        void UITabHeader::onArrowLeft() {
            if (mVisibleStart == 0)
                return;
            --mVisibleStart;
            calculateVisibleEnd();
        }

        void UITabHeader::onArrowRight() {
            if (mVisibleEnd == tabCount())
                return;
            ++mVisibleStart;
            calculateVisibleEnd();
        }
    }
}
