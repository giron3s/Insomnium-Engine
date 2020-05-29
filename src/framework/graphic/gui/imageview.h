/*
nanogui/imageview.h -- Widget used to display images.

The image view widget was contributed by Stefan Ivanov.

NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
The widget drawing code is based on the NanoVG demo application
by Mikko Mononen.

All rights reserved. Use of this source code is governed by a
BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <graphic/procedural/texturedquad.h>
#include "graphic/gui/widget.h"
#include "graphic/texture.h"
#include "graphic/shader.h"
#include <functional>

namespace Framework
{
    namespace GUI
    {

        /**
        * \class ImageView imageview.h nanogui/imageview.h
        *
        * \brief Widget used to display images.
        */
        class UIImageView : public UIWidget
        {
        public:
            UIImageView();

            ~UIImageView();

            void setImage(int aImage);
            void setImageFromName(std::string aImage);

            glm::vec2 positionF() const { return glm::vec2(mPos.x, mPos.y); }
            glm::vec2 sizeF() const { return glm::vec2(mSize.x, mSize.y); }

            const glm::ivec2& imageSize() const { return mImageSize; }
            glm::ivec2 scaledImageSize() const { return glm::ivec2(static_cast<float>(mImageSize.x * mScale), static_cast<float>(mImageSize.y * mScale)); }
            glm::vec2 imageSizeF() const { return glm::vec2(mImageSize.x, mImageSize.y); }
            glm::vec2 scaledImageSizeF() const { return glm::ivec2(static_cast<float>(mImageSize.x * mScale), static_cast<float>(mImageSize.y * mScale)); }

            const glm::vec2& offset() const { return mOffset; }
            void setOffset(const glm::vec2& offset) { mOffset = offset; }
            float scale() const { return mScale; }
            void setScale(float scale) { mScale = scale > 0.01f ? scale : 0.01f; }

            bool fixedOffset() const { return mFixedOffset; }
            void setFixedOffset(bool fixedOffset) { mFixedOffset = fixedOffset; }
            bool fixedScale() const { return mFixedScale; }
            void setFixedScale(bool fixedScale) { mFixedScale = fixedScale; }

            float zoomSensitivity() const { return mZoomSensitivity; }
            void setZoomSensitivity(float zoomSensitivity) { mZoomSensitivity = zoomSensitivity; }

            float gridThreshold() const { return mGridThreshold; }
            void setGridThreshold(float gridThreshold) { mGridThreshold = gridThreshold; }

            float pixelInfoThreshold() const { return mPixelInfoThreshold; }
            void setPixelInfoThreshold(float pixelInfoThreshold) { mPixelInfoThreshold = pixelInfoThreshold; }

            void setPixelInfoCallback(const std::function<std::pair<std::string, Color>(const glm::ivec2&)>& callback) {
                mPixelInfoCallback = callback;
            }
            const std::function<std::pair<std::string, Color>(const glm::ivec2&)>& pixelInfoCallback() const {
                return mPixelInfoCallback;
            }

            void setFontScaleFactor(float fontScaleFactor) { mFontScaleFactor = fontScaleFactor; }
            float fontScaleFactor() const { return mFontScaleFactor; }

            // Image transformation functions.

            /// Calculates the image coordinates of the given pixel position on the widget.
            glm::vec2 imageCoordinateAt(const glm::vec2& position) const;

            /**
            * Calculates the image coordinates of the given pixel position on the widget.
            * If the position provided corresponds to a coordinate outside the range of
            * the image, the coordinates are clamped to edges of the image.
            */
            glm::vec2 clampedImageCoordinateAt(const glm::vec2& position) const;

            /// Calculates the position inside the widget for the given image coordinate.
            glm::vec2 positionForCoordinate(const glm::vec2& imageCoordinate) const;

            /**
            * Modifies the internal state of the image viewer widget so that the pixel at the provided
            * position on the widget has the specified image coordinate. Also clamps the values of offset
            * to the sides of the widget.
            */
            void setImageCoordinateAt(const glm::vec2& position, const glm::vec2& imageCoordinate);

            /// Centers the image without affecting the scaling factor.
            void center();

            /// Centers and scales the image so that it fits inside the widgets.
            void fit();

            /// Set the scale while keeping the image centered
            void setScaleCentered(float scale);

            /// Moves the offset by the specified amount. Does bound checking.
            void moveOffset(const glm::vec2& delta);

            /**
            * Changes the scale factor by the provided amount modified by the zoom sensitivity member variable.
            * The scaling occurs such that the image coordinate under the focused position remains in
            * the same position before and after the scaling.
            */
            void zoom(int amount, const glm::vec2& focusPosition);

            bool keyboardEvent(int key, int scancode, int action, int modifiers) override;
            bool keyboardCharacterEvent(unsigned int codepoint) override;
            bool mouseDragEvent(const glm::ivec2 &p, const glm::ivec2 &rel, int button, int modifiers) override;
            bool scrollEvent(const glm::ivec2 &p, const glm::vec2 &rel) override;

            /// Function indicating whether the grid is currently visible.
            bool gridVisible() const;

            /// Function indicating whether the pixel information is currently visible.
            bool pixelInfoVisible() const;

            /// Function indicating whether any of the overlays are visible.
            bool helpersVisible() const;

            glm::ivec2 preferredSize(NVGcontext* ctx) const override;
            void performLayout(NVGcontext* ctx) override;
            void draw(NVGcontext* ctx) override;

            virtual void Serialize(Json::Value& aSerializer) const override;
            virtual void Deserialize(const Json::Value& aSerializer) override;

        private:
            // Helper drawing methods.
            void drawIcon(NVGcontext* ctx);
            void drawImage(NVGcontext* ctx);
            void drawWidgetBorder(NVGcontext* ctx) const;
            void drawImageBorder(NVGcontext* ctx) const;
            void drawHelpers(NVGcontext* ctx) const;
            static void drawPixelGrid(NVGcontext* ctx, const glm::vec2& upperLeftCorner, const glm::vec2& lowerRightCorner, float stride);
            void drawPixelInfo(NVGcontext* ctx, float stride) const;
            void writePixelInfo(NVGcontext* ctx, const glm::vec2& cellPosition,
                const glm::ivec2& pixel, float stride, float fontSize) const;

            // Image parameters.
            Model2D*                        mImage;
            int                             mIcon;
            glm::ivec2                      mImageSize;

            // Image display parameters.
            float                           mScale;
            glm::vec2                       mOffset;
            bool                            mFixedScale;
            bool                            mFixedOffset;

            // Fine-tuning parameters.
            float                           mZoomSensitivity = 1.1f;

            // Image info parameters.
            float                           mGridThreshold = -1;
            float                           mPixelInfoThreshold = -1;

            // Image pixel data display members.
            std::function<std::pair<std::string, Color>(const glm::ivec2&)> mPixelInfoCallback;
            float                           mFontScaleFactor = 0.2f;
            //public:
            //    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };
    }
}
