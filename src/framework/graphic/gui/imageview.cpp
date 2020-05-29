/*
    nanogui/imageview.cpp -- Widget used to display images.

    The image view widget was contributed by Stefan Ivanov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"

#include <cmath>
#include "engine.h"
#include "graphic/gui/imageview.h"
#include "graphic/gui/window.h"
#include "graphic/gui/screen.h"
#include "graphic/gui/theme.h"


namespace Framework
{
    namespace GUI
    {
        std::vector<std::string> tokenize(const std::string &string,
            const std::string &delim = "\n",
            bool includeEmpty = false) {
            std::string::size_type lastPos = 0, pos = string.find_first_of(delim, lastPos);
            std::vector<std::string> tokens;

            while (lastPos != std::string::npos) {
                std::string substr = string.substr(lastPos, pos - lastPos);
                if (!substr.empty() || includeEmpty)
                    tokens.push_back(std::move(substr));
                lastPos = pos;
                if (lastPos != std::string::npos) {
                    lastPos += 1;
                    pos = string.find_first_of(delim, lastPos);
                }
            }

            return tokens;
        }


        UIImageView::UIImageView()
            : UIWidget(nullptr)
            , mIcon(0)
            , mScale(1.0f)
            , mOffset(glm::vec2())
            , mFixedScale(false)
            , mFixedOffset(false)
            , mPixelInfoCallback(nullptr)
        {
        }

        UIImageView::~UIImageView()
        {
        }

        void UIImageView::setImage(int aImage)
        {
            UIScreen& lUIScreen = Engine::Instance()->Display();

            //if (mIcon != 0)
            //    nvgDeleteImage(lUIScreen.nvgContext(), mIcon);

            if (aImage < 0)
                WARNING("Can not set the '%d' as image", aImage);

            mIcon = aImage;
        }

        void UIImageView::setImageFromName(std::string aImage)
        {
            int lIcon = Engine::Instance()->ResourceManager().FindImage(aImage);
            if (lIcon < 0)
            {
                WARNING("UIImageView - Cannot set icon from string");
                return;
            }

            setImage(lIcon);
        }

        glm::vec2 UIImageView::imageCoordinateAt(const glm::vec2& position) const {
            auto imagePosition = position - mOffset;
            return imagePosition / mScale;
        }

        glm::vec2 UIImageView::clampedImageCoordinateAt(const glm::vec2& position) const {
            auto imageCoordinate = imageCoordinateAt(position);
            return WiseMin(WiseMax(imageCoordinate, glm::vec2()), imageSizeF());
        }

        glm::vec2 UIImageView::positionForCoordinate(const glm::vec2& imageCoordinate) const {
            return mScale*imageCoordinate + mOffset;
        }

        void UIImageView::setImageCoordinateAt(const glm::vec2& position, const glm::vec2& imageCoordinate) {
            // Calculate where the new offset must be in order to satisfy the image position equation.
            // Round the floating point values to balance out the floating point to integer conversions.
            mOffset = position - (imageCoordinate * mScale);

            // Clamp offset so that the image remains near the screen.
            mOffset = WiseMax(WiseMin(mOffset, sizeF()), -scaledImageSizeF());
        }

        void UIImageView::center()
        {
            mOffset = (sizeF() - scaledImageSizeF());
            mOffset /= 2;
        }

        void UIImageView::fit() {
            // Calculate the appropriate scaling factor.
            mScale = min(sizeF().x / imageSizeF().x, sizeF().y / imageSizeF().y);
            center();
        }

        void UIImageView::setScaleCentered(float scale) {
            auto centerPosition = sizeF();
            centerPosition /= 2;
            auto p = imageCoordinateAt(centerPosition);
            mScale = scale;
            setImageCoordinateAt(centerPosition, p);
        }

        void UIImageView::moveOffset(const glm::vec2& delta) {
            // Apply the delta to the offset.
            mOffset += delta;

            // Prevent the image from going out of bounds.
            auto scaledSize = scaledImageSizeF();
            if (mOffset.x + scaledSize.x < 0)
                mOffset.x = -scaledSize.x;
            if (mOffset.x > sizeF().x)
                mOffset.x = sizeF().x;
            if (mOffset.y + scaledSize.y < 0)
                mOffset.y = -scaledSize.y;
            if (mOffset.y > sizeF().y)
                mOffset.y = sizeF().y;
        }

        void UIImageView::zoom(int amount, const glm::vec2& focusPosition) {
            auto focusedCoordinate = imageCoordinateAt(focusPosition);
            float scaleFactor = std::pow(mZoomSensitivity, amount);
            mScale = std::max(0.01f, scaleFactor * mScale);
            setImageCoordinateAt(focusPosition, focusedCoordinate);
        }

        bool UIImageView::mouseDragEvent(const glm::ivec2& p, const glm::ivec2& rel, int button, int /*modifiers*/)
        {
            if ((button & (1 << GLFW_MOUSE_BUTTON_LEFT)) != 0 && !mFixedOffset) {
                setImageCoordinateAt(glm::vec2(p + rel), imageCoordinateAt(glm::vec2(p)));
                return true;
            }
            return false;
        }

        bool UIImageView::gridVisible() const {
            return (mGridThreshold != -1) && (mScale > mGridThreshold);
        }

        bool UIImageView::pixelInfoVisible() const {
            return mPixelInfoCallback && (mPixelInfoThreshold != -1) && (mScale > mPixelInfoThreshold);
        }

        bool UIImageView::helpersVisible() const {
            return gridVisible() || pixelInfoVisible();
        }

        bool UIImageView::scrollEvent(const glm::ivec2& p, const glm::vec2& rel) {
            if (mFixedScale)
                return false;
            float v = rel.y;
            if (std::abs(v) < 1)
                v = std::copysign(1.f, v);
            zoom(v, glm::vec2(p - position()));
            return true;
        }

        bool UIImageView::keyboardEvent(int key, int /*scancode*/, int action, int modifiers) {
            if (action) {
                switch (key) {
                case GLFW_KEY_LEFT:
                    if (!mFixedOffset) {
                        if (GLFW_MOD_CONTROL & modifiers)
                            moveOffset(glm::vec2(30, 0));
                        else
                            moveOffset(glm::vec2(10, 0));
                        return true;
                    }
                    break;
                case GLFW_KEY_RIGHT:
                    if (!mFixedOffset) {
                        if (GLFW_MOD_CONTROL & modifiers)
                            moveOffset(glm::vec2(-30, 0));
                        else
                            moveOffset(glm::vec2(-10, 0));
                        return true;
                    }
                    break;
                case GLFW_KEY_DOWN:
                    if (!mFixedOffset) {
                        if (GLFW_MOD_CONTROL & modifiers)
                            moveOffset(glm::vec2(0, -30));
                        else
                            moveOffset(glm::vec2(0, -10));
                        return true;
                    }
                    break;
                case GLFW_KEY_UP:
                    if (!mFixedOffset) {
                        if (GLFW_MOD_CONTROL & modifiers)
                            moveOffset(glm::vec2(0, 30));
                        else
                            moveOffset(glm::vec2(0, 10));
                        return true;
                    }
                    break;
                }
            }
            return false;
        }

        bool UIImageView::keyboardCharacterEvent(unsigned int codepoint) {
            switch (codepoint) {
            case '-':
                if (!mFixedScale) {
                    zoom(-1, sizeF() /= 2);
                    return true;
                }
                break;
            case '+':
                if (!mFixedScale) {
                    zoom(1, sizeF() /= 2);
                    return true;
                }
                break;
            case 'c':
                if (!mFixedOffset) {
                    center();
                    return true;
                }
                break;
            case 'f':
                if (!mFixedOffset && !mFixedScale) {
                    fit();
                    return true;
                }
                break;
            case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9':
                if (!mFixedScale) {
                    setScaleCentered(1 << (codepoint - '1'));
                    return true;
                }
                break;
            default:
                return false;
            }
            return false;
        }

        glm::ivec2 UIImageView::preferredSize(NVGcontext* /*ctx*/) const {
            return mImageSize;
        }

        void UIImageView::performLayout(NVGcontext* ctx) 
        {
            UIWidget::performLayout(ctx);
            center();
        }

        void UIImageView::draw(NVGcontext* ctx) 
        {
            
            nvgEndFrame(ctx); // Flush the NanoVG draw stack, not necessary to call nvgBeginFrame afterwards.

            if (mIcon != 0)
                drawIcon(ctx);
            else
                drawImage(ctx);

            UIWidget::draw(ctx);
        }


        void UIImageView::drawIcon(NVGcontext* ctx)
        {
            const UIScreen* lScreen = dynamic_cast<const UIScreen*>(this->window()->parent());
            assert(lScreen);
            glm::vec2 lScreenSize = glm::vec2(lScreen->size());
            glm::vec2 lPositionInScreen = glm::vec2(absolutePosition());

            glEnable(GL_SCISSOR_TEST);
            float r = lScreen->pixelRatio();
            glScissor(lPositionInScreen.x * r, (lScreenSize.y - lPositionInScreen.y - size().y) * r, size().x * r, size().y * r);

            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, mPos.x + 1, mPos.y + 1, mSize.x - 2, mSize.y - 2, 10);
            NVGpaint imgPaint = nvgImagePattern(ctx, mPos.x + 1, mPos.y + 1, mSize.x - 2, mSize.y - 2, 0, mIcon, mEnabled ? 1.0f : 0.5f);
            nvgFillPaint(ctx, imgPaint);
            nvgFill(ctx);

            glDisable(GL_SCISSOR_TEST);

            if (helpersVisible())
                drawHelpers(ctx);
        }

        void UIImageView::drawImage(NVGcontext* ctx)
        {
            // Calculate several variables that need to be send to OpenGL in order for the image to be
            // properly displayed inside the widget.
            const UIScreen* lScreen = dynamic_cast<const UIScreen*>(this->window()->parent());
            assert(lScreen);
            glm::vec2 lScreenSize = glm::vec2(lScreen->size());
            glm::vec2 lScaleFactor(mScale * (imageSizeF().x / lScreenSize.x), mScale * (imageSizeF().y / lScreenSize.y));
            glm::vec2 lPositionInScreen = glm::vec2(absolutePosition());
            glm::vec2 lPositionAfterOffset = lPositionInScreen + mOffset;
            glm::vec2 lImageOffset(lPositionAfterOffset.x / lScreenSize.x, lPositionAfterOffset.y / lScreenSize.y);

            glEnable(GL_SCISSOR_TEST);
            float r = lScreen->pixelRatio();
            glScissor(lPositionInScreen.x * r, (lScreenSize.y - lPositionInScreen.y - size().y) * r, size().x * r, size().y * r);

            Renderer* lRenderer = Engine::Instance()->Display().GetRenderer();
            lRenderer->RenderTexturedQuad(*mImage, lImageOffset, lScaleFactor);

            glDisable(GL_SCISSOR_TEST);
            if (helpersVisible())
                drawHelpers(ctx);
        }

        void UIImageView::drawWidgetBorder(NVGcontext* ctx) const
        {
            nvgBeginPath(ctx);
            nvgStrokeWidth(ctx, 1);
            nvgRoundedRect(ctx, mPos.x + 0.5f, mPos.y + 0.5f, mSize.x - 1,
                mSize.y - 1, 0);
            nvgStrokeColor(ctx, mTheme->mWindowPopup);
            nvgStroke(ctx);

            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, mPos.x + 0.5f, mPos.y + 0.5f, mSize.x - 1,
                mSize.y - 1, mTheme->mButtonCornerRadius);
            nvgStrokeColor(ctx, mTheme->mBorderDark);
            nvgStroke(ctx);
        }

        void UIImageView::drawImageBorder(NVGcontext* ctx) const {
            nvgSave(ctx);
            nvgBeginPath(ctx);
            nvgScissor(ctx, mPos.x, mPos.y, mSize.x, mSize.y);
            nvgStrokeWidth(ctx, 1.0f);
            glm::ivec2 borderPosition = mPos + glm::ivec2(mOffset);
            glm::ivec2 borderSize = glm::ivec2(scaledImageSizeF());
            nvgRect(ctx, borderPosition.x - 0.5f, borderPosition.y - 0.5f,
                borderSize.x + 1, borderSize.y + 1);
            nvgStrokeColor(ctx, Color(1.0f, 1.0f, 1.0f, 1.0f));
            nvgStroke(ctx);
            nvgResetScissor(ctx);
            nvgRestore(ctx);
        }

        void UIImageView::drawHelpers(NVGcontext* ctx) const
        {
            // We need to apply mPos after the transformation to account for the position of the widget
            // relative to the parent.
            glm::vec2 upperLeftCorner = positionForCoordinate(glm::vec2()) + positionF();
            glm::vec2 lowerRightCorner = positionForCoordinate(imageSizeF()) + positionF();
            if (gridVisible())
                drawPixelGrid(ctx, upperLeftCorner, lowerRightCorner, mScale);
            if (pixelInfoVisible())
                drawPixelInfo(ctx, mScale);
        }

        void UIImageView::drawPixelGrid(NVGcontext* ctx, const glm::vec2& upperLeftCorner,
            const glm::vec2& lowerRightCorner, float stride) {
            nvgBeginPath(ctx);

            // Draw the vertical grid lines
            float currentX = upperLeftCorner.x;
            while (currentX <= lowerRightCorner.x) {
                nvgMoveTo(ctx, std::round(currentX), std::round(upperLeftCorner.y));
                nvgLineTo(ctx, std::round(currentX), std::round(lowerRightCorner.y));
                currentX += stride;
            }

            // Draw the horizontal grid lines
            float currentY = upperLeftCorner.y;
            while (currentY <= lowerRightCorner.y) {
                nvgMoveTo(ctx, std::round(upperLeftCorner.x), std::round(currentY));
                nvgLineTo(ctx, std::round(lowerRightCorner.x), std::round(currentY));
                currentY += stride;
            }

            nvgStrokeWidth(ctx, 1.0f);
            nvgStrokeColor(ctx, Color(1.0f, 1.0f, 1.0f, 0.2f));
            nvgStroke(ctx);
        }

        void UIImageView::drawPixelInfo(NVGcontext* ctx, float stride) const {
            // Extract the image coordinates at the two corners of the widget.
            glm::ivec2 topLeft;
            topLeft.x = glm::floor(clampedImageCoordinateAt(glm::vec2()).x);
            topLeft.y = glm::floor(clampedImageCoordinateAt(glm::vec2()).y);

            glm::ivec2 bottomRight;
            bottomRight.x = glm::ceil(clampedImageCoordinateAt(sizeF()).x);
            bottomRight.y = glm::ceil(clampedImageCoordinateAt(sizeF()).y);

            // Extract the positions for where to draw the text.
            glm::vec2 currentCellPosition = (positionF() + positionForCoordinate(glm::vec2(topLeft)));

            float xInitialPosition = currentCellPosition.x;
            int xInitialIndex = topLeft.x;

            // Properly scale the pixel information for the given stride.
            auto fontSize = stride * mFontScaleFactor;
            static constexpr float maxFontSize = 30.0f;
            fontSize = fontSize > maxFontSize ? maxFontSize : fontSize;
            nvgBeginPath(ctx);
            nvgFontSize(ctx, fontSize);
            nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
            nvgFontFace(ctx, mTheme->mLabelFont.c_str());
            while (topLeft.y != bottomRight.y) {
                while (topLeft.x != bottomRight.x) {
                    writePixelInfo(ctx, currentCellPosition, topLeft, stride, fontSize);
                    currentCellPosition.x += stride;
                    ++topLeft.x;
                }
                currentCellPosition.x = xInitialPosition;
                currentCellPosition.y += stride;
                ++topLeft.y;
                topLeft.x = xInitialIndex;
            }
        }

        void UIImageView::writePixelInfo(NVGcontext* ctx, const glm::vec2& cellPosition,
            const glm::ivec2& pixel, float stride, float fontSize) const
        {
            auto pixelData = mPixelInfoCallback(pixel);
            auto pixelDataRows = tokenize(pixelData.first);

            // If no data is provided for this pixel then simply return.
            if (pixelDataRows.empty())
                return;

            nvgFillColor(ctx, pixelData.second);
            float yOffset = (stride - fontSize * pixelDataRows.size()) / 2;
            for (size_t i = 0; i != pixelDataRows.size(); ++i) {
                nvgText(ctx, cellPosition.x + stride / 2, cellPosition.y + yOffset,
                    pixelDataRows[i].data(), nullptr);
                yOffset += fontSize;
            }
        }

        void UIImageView::Serialize(Json::Value& aSerializer) const
        {
            //TODO implement it
            assert(false);
        }

        void UIImageView::Deserialize(const Json::Value& aSerializer)
        {
            UIWidget::Deserialize(aSerializer);
            mFixedScale = aSerializer.get("fixedScale", mFixedScale).asBool();
            mFixedOffset = aSerializer.get("fixedOffset", mFixedOffset).asBool();

            if (!aSerializer.isMember("image") && !aSerializer.isMember("icon"))
                WARNING("ImageView without any image!");

            if (aSerializer.isMember("image"))
            {
                Texture lTexture;
                lTexture.mName = aSerializer["image"].asString();

                if (!lTexture.Load(lTexture.mName))
                {
                    CRASH("Error ocurred during loading a '%s' PNG image!", lTexture.mName.c_str())
                }

                mImageSize = glm::ivec2(lTexture.mWidth, lTexture.mHeight);
                std::shared_ptr<Procedural::TexturedQuad> lTexQuadAsset = std::make_shared<Procedural::TexturedQuad>(2.0f, 2.0f, std::move(lTexture));
                Renderer* lRenderer = Engine::Instance()->Display().GetRenderer();
                if (!lRenderer->PrepareForRendering(*lTexQuadAsset))
                    CRASH("Failed to prepare model %s\n", mImage->GetName().c_str());
                mImage = new Model2D(std::move(lTexQuadAsset));
                fit();
            }

            if (aSerializer.isMember("icon"))
            {
                auto lIcon = aSerializer.get("icon", mIcon);
                if (lIcon.isInt())
                    mIcon = lIcon.asInt();
                else
                    setImageFromName(aSerializer.get("icon", "Undefined").asString());
            }
        }
    }
}
