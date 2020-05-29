/*
    src/imagepanel.cpp -- Image panel widget which shows a number of
    square-shaped icons

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"

#include "engine.h"
#include "json/json.h"
#include "graphic/gui/imagepanel.h"
#include "graphic/gui/screen.h"

namespace Framework
{
    namespace GUI
    {
        UIImagePanel::UIImagePanel()
            : UIWidget(nullptr)
            , mThumbSize(64)
            , mSpacing(10)
            , mMargin(10)
            , mMouseIndex(-1) 
        {

        }

        UIImagePanel::UIImagePanel(UIWidget *aParent)
            : UIWidget(aParent)
            , mThumbSize(64)
            , mSpacing(10)
            , mMargin(10)
            , mMouseIndex(-1)
        {

        }

        glm::ivec2 UIImagePanel::gridSize() const
        {
            int nCols = 1 + std::max(0,
                (int)((mSize.x - 2 * mMargin - mThumbSize) /
                (float)(mThumbSize + mSpacing)));
            int nRows = ((int)mImages.size() + nCols - 1) / nCols;
            return glm::ivec2(nCols, nRows);
        }

        int UIImagePanel::indexForPosition(const glm::ivec2 &p) const 
        {
            glm::vec2 pp = (glm::vec2(p - mPos) - glm::vec2(mMargin)) / (float)(mThumbSize + mSpacing);
            float iconRegion = mThumbSize / (float)(mThumbSize + mSpacing);
            bool overImage = pp.x - std::floor(pp.x) < iconRegion && pp.y - std::floor(pp.y) < iconRegion;
            glm::ivec2 gridPos = glm::ivec2(pp), grid = gridSize();
            for (int i = 0; i < gridPos.length(); ++i)
                overImage &= (gridPos[i] >= 0) && (gridPos[i] < grid[i]);
            return overImage ? (gridPos.x + gridPos.y * grid.x) : -1;
        }

        bool UIImagePanel::mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers)
        {
            if ( button == GLFW_MOUSE_BUTTON_LEFT && down )
            {
                mMouseIndex = indexForPosition(p);
            }
            return true;
        }

        glm::ivec2 UIImagePanel::preferredSize(NVGcontext *) const {
            glm::ivec2 grid = gridSize();
            return glm::ivec2(
                grid.x * mThumbSize + (grid.x - 1) * mSpacing + 2 * mMargin,
                grid.y * mThumbSize + (grid.y - 1) * mSpacing + 2 * mMargin
            );
        }

        void UIImagePanel::draw(NVGcontext* ctx) 
        {
            glm::ivec2 grid = gridSize();

            for (size_t i = 0; i<mImages.size(); ++i) {
                glm::ivec2 p = mPos + glm::ivec2(mMargin) +
                    glm::ivec2((int)i % grid.x, (int)i / grid.x) * (mThumbSize + mSpacing);
                int imgw, imgh;

                nvgImageSize(ctx, mImages[i].first, &imgw, &imgh);
                float iw, ih, ix, iy;
                if (imgw < imgh) {
                    iw = mThumbSize;
                    ih = iw * (float)imgh / (float)imgw;
                    ix = 0;
                    iy = -(ih - mThumbSize) * 0.5f;
                }
                else {
                    ih = mThumbSize;
                    iw = ih * (float)imgw / (float)imgh;
                    ix = -(iw - mThumbSize) * 0.5f;
                    iy = 0;
                }

                NVGpaint imgPaint = nvgImagePattern(
                    ctx, p.x + ix, p.y + iy, iw, ih, 0, mImages[i].first,
                    mMouseIndex == (int)i ? 1.0 : 0.7);

                nvgBeginPath(ctx);
                nvgRoundedRect(ctx, p.x, p.y, mThumbSize, mThumbSize, 5);
                nvgFillPaint(ctx, imgPaint);
                nvgFill(ctx);

                NVGpaint shadowPaint =
                    nvgBoxGradient(ctx, p.x - 1, p.y, mThumbSize + 2, mThumbSize + 2, 5, 3,
                        nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
                nvgBeginPath(ctx);
                nvgRect(ctx, p.x - 5, p.y - 5, mThumbSize + 10, mThumbSize + 10);
                nvgRoundedRect(ctx, p.x, p.y, mThumbSize, mThumbSize, 6);
                nvgPathWinding(ctx, NVG_HOLE);
                nvgFillPaint(ctx, shadowPaint);
                nvgFill(ctx);

                nvgBeginPath(ctx);
                nvgRoundedRect(ctx, p.x + 0.5f, p.y + 0.5f, mThumbSize - 1, mThumbSize - 1, 4 - 0.5f);
                nvgStrokeWidth(ctx, 1.0f);
                nvgStrokeColor(ctx, nvgRGBA(255, 255, 255, 80));
                nvgStroke(ctx);
            }
        }

        void UIImagePanel::Serialize(Json::Value& aSerializer) const
        {
        
        }
        
        void UIImagePanel::Deserialize(const Json::Value& aSerializer)
        {
            UIWidget::Deserialize(aSerializer);

            mThumbSize  = aSerializer.get("thumbSize", mThumbSize).asInt();
            mSpacing    = aSerializer.get("spacing", mSpacing).asInt();
            mMargin     = aSerializer.get("margin", mMargin).asInt();
            mMouseIndex = aSerializer.get("mouseIndex", mMouseIndex).asInt();

            //Image Directory
            if( aSerializer.isMember("imageDir") )
            {
                UIScreen& lUIScreen = Engine::Instance()->Display();
                mImages = Utils::LoadImageDirectory(aSerializer["imageDir"].asString(), lUIScreen.nvgContext());
            }

            //Images one by one
            if (aSerializer.isMember("images"))
            {
                UIScreen lUIScreen = Engine::Instance()->Display();

                const Json::Value lResImage = aSerializer["images"];
                for (size_t i = 0, lSize = lResImage.size(); i < lSize; ++i)
                {
                    mImages = Utils::LoadImageDirectory(lResImage[static_cast<int>(i)].asString(), lUIScreen.nvgContext());
                }
            }
        }
    }
}
