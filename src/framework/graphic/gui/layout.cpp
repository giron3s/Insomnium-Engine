/*
    src/layout.cpp -- A collection of useful layout managers

    The grid layout was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"

#include "json/json.h"
#include "graphic/gui/layout.h"
#include "graphic/gui/widget.h"
#include "graphic/gui/window.h"
#include "graphic/gui/theme.h"
#include "graphic/gui/label.h"
#include <numeric>

namespace Framework
{
    namespace GUI
    {

        //**********************************************************************************************
        //   BoxLayout
        //**********************************************************************************************
        UIBoxLayout::UIBoxLayout()
            : mOrientation(Orientation::eHorizontal)
            , mAlignment(Alignment::eMiddle)
            , mMargin(0)
            , mSpacing(0)
        {

        }

        UIBoxLayout::UIBoxLayout(Orientation orientation, Alignment alignment,
            int margin, int spacing)
            : mOrientation(orientation), mAlignment(alignment), mMargin(margin),
            mSpacing(spacing) {
        }

        glm::ivec2 UIBoxLayout::preferredSize(NVGcontext *ctx, const UIWidget *widget) const {
            glm::ivec2 size = glm::ivec2(2 * mMargin);

            int yOffset = 0;
            const UIWindow *window = dynamic_cast<const UIWindow *>(widget);
            if (window && !window->title().empty()) {
                if (mOrientation == Orientation::eVertical)
                    size[1] += widget->theme()->mWindowHeaderHeight - mMargin / 2;
                else
                    yOffset = widget->theme()->mWindowHeaderHeight;
            }

            bool first = true;
            int axis1 = (int)mOrientation, axis2 = ((int)mOrientation + 1) % 2;
            for (auto w : widget->children()) {
                if (!w->visible())
                    continue;
                if (first)
                    first = false;
                else
                    size[axis1] += mSpacing;

                glm::ivec2 ps = w->preferredSize(ctx), fs = w->fixedSize();
                glm::ivec2 targetSize(
                    fs[0] ? fs[0] : ps[0],
                    fs[1] ? fs[1] : ps[1]
                );

                size[axis1] += targetSize[axis1];
                size[axis2] = std::max(size[axis2], targetSize[axis2] + 2 * mMargin);
                first = false;
            }
            return size + glm::ivec2(0, yOffset);
        }

        void UIBoxLayout::performLayout(NVGcontext *ctx, UIWidget *widget) const
        {
            glm::ivec2 fs_w = widget->fixedSize();
            glm::ivec2 containerSize(
                fs_w[0] ? fs_w[0] : widget->width(),
                fs_w[1] ? fs_w[1] : widget->height()
            );

            int axis1 = (int)mOrientation, axis2 = ((int)mOrientation + 1) % 2;
            int position = mMargin;
            int yOffset = 0;

            const UIWindow *window = dynamic_cast<const UIWindow *>(widget);
            if (window && !window->title().empty()) {
                if (mOrientation == Orientation::eVertical) {
                    position += widget->theme()->mWindowHeaderHeight - mMargin / 2;
                }
                else {
                    yOffset = widget->theme()->mWindowHeaderHeight;
                    containerSize[1] -= yOffset;
                }
            }

            bool first = true;
            for (auto w : widget->children())
            {
                if (!w->visible())
                    continue;
                if (first)
                    first = false;
                else
                    position += mSpacing;

                glm::ivec2 ps = w->preferredSize(ctx), fs = w->fixedSize();
                glm::ivec2 targetSize(
                    fs[0] ? fs[0] : ps[0],
                    fs[1] ? fs[1] : ps[1]
                );
                glm::ivec2 pos(0, yOffset);

                pos[axis1] = position;

                //Apply the aligment
                switch (mAlignment)
                {
                    case Alignment::eMinimum:
                        pos[axis2] += mMargin;
                        break;
                    case Alignment::eMiddle:
                        pos[axis2] += (containerSize[axis2] - targetSize[axis2]) / 2;
                        break;
                    case Alignment::eMaximum:
                        pos[axis2] += containerSize[axis2] - targetSize[axis2] - mMargin * 2;
                        break;
                    case Alignment::eFill:
                        pos[axis2] += mMargin;
                        targetSize[axis2] = fs[axis2] ? fs[axis2] : (containerSize[axis2] - mMargin * 2);
                        break;
                    default:
                        WARNING("The UIBoxLayout not handle the '%s' alignment type!", AlignmentToString(mAlignment));
                        break;
                }

                w->setPosition(pos);
                w->setSize(targetSize);
                w->performLayout(ctx);
                position += targetSize[axis1];
            }
        }

        void UIBoxLayout::Serialize(Json::Value& aSerializer) const
        {
            //TODO implement it!!
            assert(false);
        }

        void UIBoxLayout::Deserialize(const Json::Value& aSerializer)
        {
            mMargin = aSerializer.get("margin", mMargin).asInt();
            mSpacing = aSerializer.get("spacing", mSpacing).asInt();

            string lOrientationStr = aSerializer.get("orientation", OrientationToString(mOrientation)).asString();
            mOrientation = StringToOrientation(lOrientationStr.c_str());

            string lAlignmentStr = aSerializer.get("alignment", AlignmentToString(mAlignment)).asString();
            mAlignment = StringToAlignment(lAlignmentStr.c_str());
        }


        //**********************************************************************************************
        //   Grouplayout
        //**********************************************************************************************
        UIGroupLayout::UIGroupLayout(int aMargin /*= 15*/,
            int aSpacing /*= 6*/,
            int aGroupSpacing/*= 14*/,
            int aGroupIndent/*= 20*/)

            : mMargin(aMargin)
            , mSpacing(aSpacing)
            , mGroupSpacing(aGroupSpacing)
            , mGroupIndent(aGroupIndent)
        {

        }

        glm::ivec2 UIGroupLayout::preferredSize(NVGcontext *ctx, const UIWidget *widget) const {
            int height = mMargin, width = 2 * mMargin;

            const UIWindow *window = dynamic_cast<const UIWindow *>(widget);
            if (window && !window->title().empty())
                height += widget->theme()->mWindowHeaderHeight - mMargin / 2;

            bool first = true, indent = false;
            for (auto c : widget->children()) {
                if (!c->visible())
                    continue;
                const UILabel *label = dynamic_cast<const UILabel *>(c);
                if (!first)
                    height += (label == nullptr) ? mSpacing : mGroupSpacing;
                first = false;

                glm::ivec2 ps = c->preferredSize(ctx), fs = c->fixedSize();
                glm::ivec2 targetSize(
                    fs[0] ? fs[0] : ps[0],
                    fs[1] ? fs[1] : ps[1]
                );

                bool indentCur = indent && label == nullptr;
                height += targetSize.y;
                width = std::max(width, targetSize.x + 2 * mMargin + (indentCur ? mGroupIndent : 0));

                if (label)
                    indent = !label->caption().empty();
            }
            height += mMargin;
            return glm::ivec2(width, height);
        }

        void UIGroupLayout::performLayout(NVGcontext *ctx, UIWidget *widget) const {
            int height = mMargin, availableWidth =
                (widget->fixedWidth() ? widget->fixedWidth() : widget->width()) - 2 * mMargin;

            const UIWindow *window = dynamic_cast<const UIWindow *>(widget);
            if (window && !window->title().empty())
                height += widget->theme()->mWindowHeaderHeight - mMargin / 2;

            bool first = true, indent = false;
            for (auto c : widget->children()) {
                if (!c->visible())
                    continue;
                const UILabel *label = dynamic_cast<const UILabel *>(c);
                if (!first)
                    height += (label == nullptr) ? mSpacing : mGroupSpacing;
                first = false;

                bool indentCur = indent && label == nullptr;
                glm::ivec2 ps = glm::ivec2(availableWidth - (indentCur ? mGroupIndent : 0), c->preferredSize(ctx).y);
                glm::ivec2 fs = c->fixedSize();

                glm::ivec2 targetSize(
                    fs[0] ? fs[0] : ps[0],
                    fs[1] ? fs[1] : ps[1]
                );

                c->setPosition(glm::ivec2(mMargin + (indentCur ? mGroupIndent : 0), height));
                c->setSize(targetSize);
                c->performLayout(ctx);

                height += targetSize.y;

                if (label)
                    indent = !label->caption().empty();
            }
        }


        void UIGroupLayout::Serialize(Json::Value& aSerializer) const
        {
            //TODO implement it!!
            assert(false);
        }

        void UIGroupLayout::Deserialize(const Json::Value& aSerializer)
        {
            mMargin = aSerializer.get("margin", mMargin).asInt();
            mSpacing = aSerializer.get("spacing", mSpacing).asInt();
            mGroupSpacing = aSerializer.get("groupSpacing", mGroupSpacing).asInt();
            mGroupIndent = aSerializer.get("groupIndent", mGroupIndent).asInt();
        }

        //**********************************************************************************************
        //   Gridlayout
        //**********************************************************************************************
        /// Create a 2-column grid layout by default
        UIGridLayout::UIGridLayout(Orientation aOrientation /*= Orientation::eHorizontal*/,
            Alignment aAlignment/* = Alignment::eMiddle*/,
            int aResolution/* = 2*/,
            int aMargin/* = 0*/,
            int aSpacing/* = 0*/)
            : mOrientation(aOrientation)
            , mResolution(aResolution)
            , mMargin(aMargin)
        {
            mDefaultAlignment[0] = mDefaultAlignment[1] = aAlignment;
            mSpacing = glm::ivec2(aSpacing);
        }

        glm::ivec2 UIGridLayout::preferredSize(NVGcontext *ctx, const UIWidget *widget) const {
            /* Compute minimum row / column sizes */
            std::vector<int> grid[2];
            computeLayout(ctx, widget, grid);

            glm::ivec2 size(
                2 * mMargin + std::accumulate(grid[0].begin(), grid[0].end(), 0)
                + std::max((int)grid[0].size() - 1, 0) * mSpacing[0],
                2 * mMargin + std::accumulate(grid[1].begin(), grid[1].end(), 0)
                + std::max((int)grid[1].size() - 1, 0) * mSpacing[1]
            );

            const UIWindow *window = dynamic_cast<const UIWindow *>(widget);
            if (window && !window->title().empty())
                size[1] += widget->theme()->mWindowHeaderHeight - mMargin / 2;

            return size;
        }

        void UIGridLayout::computeLayout(NVGcontext *ctx, const UIWidget *widget, std::vector<int> grid[2]) const {
            int axis1 = (int)mOrientation, axis2 = (axis1 + 1) % 2;
            size_t numChildren = widget->children().size(), visibleChildren = 0;
            for (auto w : widget->children())
                visibleChildren += w->visible() ? 1 : 0;

            glm::ivec2 dim;
            dim[axis1] = mResolution;
            dim[axis2] = (int)((visibleChildren + mResolution - 1) / mResolution);

            grid[axis1].clear(); grid[axis1].resize(dim[axis1], 0);
            grid[axis2].clear(); grid[axis2].resize(dim[axis2], 0);

            size_t child = 0;
            for (int i2 = 0; i2 < dim[axis2]; i2++) {
                for (int i1 = 0; i1 < dim[axis1]; i1++) {
                    UIWidget *w = nullptr;
                    do {
                        if (child >= numChildren)
                            return;
                        w = widget->children()[child++];
                    } while (!w->visible());

                    glm::ivec2 ps = w->preferredSize(ctx);
                    glm::ivec2 fs = w->fixedSize();
                    glm::ivec2 targetSize(
                        fs[0] ? fs[0] : ps[0],
                        fs[1] ? fs[1] : ps[1]
                    );

                    grid[axis1][i1] = std::max(grid[axis1][i1], targetSize[axis1]);
                    grid[axis2][i2] = std::max(grid[axis2][i2], targetSize[axis2]);
                }
            }
        }

        void UIGridLayout::performLayout(NVGcontext *ctx, UIWidget *widget) const {
            glm::ivec2 fs_w = widget->fixedSize();
            glm::ivec2 containerSize(
                fs_w[0] ? fs_w[0] : widget->width(),
                fs_w[1] ? fs_w[1] : widget->height()
            );

            /* Compute minimum row / column sizes */
            std::vector<int> grid[2];
            computeLayout(ctx, widget, grid);
            int dim[2] = { (int)grid[0].size(), (int)grid[1].size() };

            glm::ivec2 extra = glm::ivec2(0);
            const UIWindow *window = dynamic_cast<const UIWindow *>(widget);
            if (window && !window->title().empty())
                extra[1] += widget->theme()->mWindowHeaderHeight - mMargin / 2;

            ///* Strech to size provided by widget */
            for (int i = 0; i < 2; i++)
            {
                int gridSize = 2 * mMargin + extra[i];
                for (int s : grid[i]) {
                    gridSize += s;
                    if (i + 1 < dim[i])
                        gridSize += mSpacing[i];
                }
            
                /// If the gap < 0 -> Reduce the last grid size
                /// If the gap > 0 -> Increase the last grid size
                int gap = containerSize[i] - gridSize;
                grid[i][dim[i] -1] += gap;
            }

            int axis1 = (int)mOrientation, axis2 = (axis1 + 1) % 2;
            glm::ivec2 start = glm::ivec2(mMargin) + extra;

            size_t numChildren = widget->children().size();
            size_t child = 0;

            glm::ivec2 pos = start;
            for (int i2 = 0; i2 < dim[axis2]; i2++) {
                pos[axis1] = start[axis1];
                for (int i1 = 0; i1 < dim[axis1]; i1++) {
                    UIWidget *w = nullptr;
                    do {
                        if (child >= numChildren)
                            return;
                        w = widget->children()[child++];
                    } while (!w->visible());

                    glm::ivec2 ps = w->preferredSize(ctx);
                    glm::ivec2 fs = w->fixedSize();
                    glm::ivec2 targetSize(
                        fs[0] ? fs[0] : ps[0],
                        fs[1] ? fs[1] : ps[1]
                    );

                    glm::ivec2 itemPos(pos);
                    for (int j = 0; j < 2; j++) 
                    {
                        int axis = (axis1 + j) % 2;
                        int item = j == 0 ? i1 : i2;
                        Alignment align = alignment(axis, item);

                        switch (align)
                        {
                            case Alignment::eMinimum:
                                break;
                            case Alignment::eMiddle:
                                itemPos[axis] += (grid[axis][item] - targetSize[axis]) / 2;
                                break;
                            case Alignment::eMaximum:
                                itemPos[axis] += grid[axis][item] - targetSize[axis];
                                break;
                            case Alignment::eFill:
                                targetSize[axis] = fs[axis] ? fs[axis] : grid[axis][item];
                                break;
                            default:
                                WARNING("The UIGridLayout not handle the '%s' alignment type!", AlignmentToString(align));
                                break;
                        }
                    }
                    w->setPosition(itemPos);
                    w->setSize(targetSize);
                    w->performLayout(ctx);
                    pos[axis1] += grid[axis1][i1] + mSpacing[axis1];
                }
                pos[axis2] += grid[axis2][i2] + mSpacing[axis2];
            }
        }

        void UIGridLayout::Serialize(Json::Value& aSerializer) const
        {
            //TODO implement it!!
            assert(false);
        }

        void UIGridLayout::Deserialize(const Json::Value& aSerializer)
        {
            if (!aSerializer.isMember("columns"))
            {
                CRASH("Not defined the 'columns' attribute in: \n %s", aSerializer.toStyledString().c_str());
                return;
            }
            
            //Margin and spacing 
            mMargin = aSerializer.get("margin", mMargin).asInt();
            if (aSerializer.isMember("spacing"))
            {
                mSpacing.x = aSerializer["spacing"][0].asInt();
                mSpacing.y = aSerializer["spacing"][1].asInt();
            }

            //Orientation
            string lOrientationStr = aSerializer.get("orientation", OrientationToString(mOrientation)).asString();
            mOrientation = StringToOrientation(lOrientationStr.c_str());

            //Resolution
            int lNumRows = aSerializer.get("rows", 1).asInt();
            int lNumColumns = aSerializer.get("columns", 1).asInt();
            mResolution = mOrientation == Orientation::eHorizontal ? lNumColumns : lNumRows;

            //Horizonal alignment
            if (aSerializer.isMember("hAlignment") && aSerializer["hAlignment"].size() == 1)
            {
                string lAlignmentStr = aSerializer["hAlignment"][0].asString();
                setColAlignment(StringToAlignment(lAlignmentStr.c_str()));
            }
            //Multiple horizonal alignment
            if (aSerializer.isMember("hAlignment") && aSerializer["hAlignment"].size() > 1)
            {
                std::vector<Alignment> lColumnAlignments;
                const Json::Value lResAligments = aSerializer["hAlignment"];
                for (size_t i = 0, lSize = lResAligments.size(); i < lSize; ++i)
                {
                    string lAlignmentStr = lResAligments[static_cast<int>(i)].asString();
                    lColumnAlignments.push_back(StringToAlignment(lAlignmentStr.c_str()));
                }
                setColAlignment(lColumnAlignments);
            }

            // Vertical Alignment
            if (aSerializer.isMember("vAlignment") && aSerializer["vAlignment"].size() == 1)
            {
                string lAlignmentStr = aSerializer["vAlignment"][0].asString();
                setRowAlignment(StringToAlignment(lAlignmentStr.c_str()));
            }

            //Multiple vertical alignment
            if (aSerializer.isMember("vAlignment") && aSerializer["vAlignment"].size() > 1)
            {
                std::vector<Alignment> lRowAlignments;
                const Json::Value lResAligments = aSerializer["vAlignment"];
                for (size_t i = 0, lSize = lResAligments.size(); i < lSize; ++i)
                {
                    string lAlignmentStr = lResAligments[static_cast<int>(i)].asString();
                    lRowAlignments.push_back(StringToAlignment(lAlignmentStr.c_str()));
                }
                setRowAlignment(lRowAlignments);
            }
        }

        //**********************************************************************************************
        //   Advanced Grid Layout
        //**********************************************************************************************
        UIAdvancedGridLayout::UIAdvancedGridLayout(const std::vector<int> &cols, const std::vector<int> &rows, int margin)
            : mCols(cols), mRows(rows), mMargin(margin) {
            mColStretch.resize(mCols.size(), 0);
            mRowStretch.resize(mRows.size(), 0);
        }

        glm::ivec2 UIAdvancedGridLayout::preferredSize(NVGcontext *ctx, const UIWidget *widget) const {
            /* Compute minimum row / column sizes */
            std::vector<int> grid[2];
            computeLayout(ctx, widget, grid);

            glm::ivec2 size(
                std::accumulate(grid[0].begin(), grid[0].end(), 0),
                std::accumulate(grid[1].begin(), grid[1].end(), 0));

            glm::ivec2 extra = glm::ivec2(2 * mMargin);
            const UIWindow *window = dynamic_cast<const UIWindow *>(widget);
            if (window && !window->title().empty())
                extra[1] += widget->theme()->mWindowHeaderHeight - mMargin / 2;

            return size + extra;
        }

        void UIAdvancedGridLayout::performLayout(NVGcontext *ctx, UIWidget *widget) const {
            std::vector<int> grid[2];
            computeLayout(ctx, widget, grid);

            grid[0].insert(grid[0].begin(), mMargin);
            const UIWindow *window = dynamic_cast<const UIWindow *>(widget);
            if (window && !window->title().empty())
                grid[1].insert(grid[1].begin(), widget->theme()->mWindowHeaderHeight + mMargin / 2);
            else
                grid[1].insert(grid[1].begin(), mMargin);

            for (int axis = 0; axis < 2; ++axis) {
                for (size_t i = 1; i < grid[axis].size(); ++i)
                    grid[axis][i] += grid[axis][i - 1];

                for (UIWidget *w : widget->children()) {
                    if (!w->visible())
                        continue;
                    Anchor anchor = this->anchor(w);

                    int itemPos = grid[axis][anchor.pos[axis]];
                    int cellSize = grid[axis][anchor.pos[axis] + anchor.size[axis]] - itemPos;
                    int ps = w->preferredSize(ctx)[axis], fs = w->fixedSize()[axis];
                    int targetSize = fs ? fs : ps;

                    switch (anchor.align[axis]) 
                    {
                        case Alignment::eMinimum:
                            break;
                        case Alignment::eMiddle:
                            itemPos += (cellSize - targetSize) / 2;
                            break;
                        case Alignment::eMaximum:
                            itemPos += cellSize - targetSize;
                            break;
                        case Alignment::eFill:
                            targetSize = fs ? fs : cellSize;
                            break;
                        default:
                            WARNING("The UIAdvancedGridLayout not handle the '%d' alignment type!", anchor.align[axis]);
                            break;
                    }

                    glm::ivec2 pos = w->position(), size = w->size();
                    pos[axis] = itemPos;
                    size[axis] = targetSize;
                    w->setPosition(pos);
                    w->setSize(size);
                    w->performLayout(ctx);
                }
            }
        }

        void UIAdvancedGridLayout::computeLayout(NVGcontext *ctx, const UIWidget *widget,
            std::vector<int> *_grid) const {
            glm::ivec2 fs_w = widget->fixedSize();
            glm::ivec2 containerSize(
                fs_w[0] ? fs_w[0] : widget->width(),
                fs_w[1] ? fs_w[1] : widget->height()
            );

            glm::ivec2 extra = glm::ivec2(2 * mMargin);
            const UIWindow *window = dynamic_cast<const UIWindow *>(widget);
            if (window && !window->title().empty())
                extra[1] += widget->theme()->mWindowHeaderHeight - mMargin / 2;

            containerSize -= extra;

            for (int axis = 0; axis < 2; ++axis) {
                std::vector<int> &grid = _grid[axis];
                const std::vector<int> &sizes = axis == 0 ? mCols : mRows;
                const std::vector<float> &stretch = axis == 0 ? mColStretch : mRowStretch;
                grid = sizes;

                for (int phase = 0; phase < 2; ++phase) {
                    for (auto pair : mAnchor) {
                        const UIWidget *w = pair.first;
                        if (!w->visible())
                            continue;
                        const Anchor &anchor = pair.second;
                        if ((anchor.size[axis] == 1) != (phase == 0))
                            continue;
                        int ps = w->preferredSize(ctx)[axis], fs = w->fixedSize()[axis];
                        int targetSize = fs ? fs : ps;

                        if (anchor.pos[axis] + anchor.size[axis] > (int)grid.size())
                            throw std::runtime_error(
                                "Advanced grid layout: widget is out of bounds: " +
                                (std::string) anchor);

                        int currentSize = 0;
                        float totalStretch = 0;
                        for (int i = anchor.pos[axis];
                            i < anchor.pos[axis] + anchor.size[axis]; ++i) {
                            if (sizes[i] == 0 && anchor.size[axis] == 1)
                                grid[i] = std::max(grid[i], targetSize);
                            currentSize += grid[i];
                            totalStretch += stretch[i];
                        }
                        if (targetSize <= currentSize)
                            continue;
                        if (totalStretch == 0)
                            throw std::runtime_error(
                                "Advanced grid layout: no space to place widget: " +
                                (std::string) anchor);
                        float amt = (targetSize - currentSize) / totalStretch;
                        for (int i = anchor.pos[axis];
                            i < anchor.pos[axis] + anchor.size[axis]; ++i) {
                            grid[i] += (int)std::round(amt * stretch[i]);
                        }
                    }
                }

                int currentSize = std::accumulate(grid.begin(), grid.end(), 0);
                float totalStretch = std::accumulate(stretch.begin(), stretch.end(), 0.0f);
                if (currentSize >= containerSize[axis] || totalStretch == 0)
                    continue;
                float amt = (containerSize[axis] - currentSize) / totalStretch;
                for (size_t i = 0; i < grid.size(); ++i)
                    grid[i] += (int)std::round(amt * stretch[i]);
            }
        }

        void UIAdvancedGridLayout::Serialize(Json::Value& aSerializer) const
        {
            //TODO implement it!!
            assert(false);
        }

        void UIAdvancedGridLayout::Deserialize(const Json::Value& aSerializer)
        {
            //TODO implement it!!
            assert(false);
        }
    }
}
