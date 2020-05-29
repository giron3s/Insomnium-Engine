/*
    nanogui/imagepanel.h -- Image panel widget which shows a number of
    square-shaped icons

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
        * \class ImagePanel imagepanel.h nanogui/imagepanel.h
        *
        * \brief Image panel widget which shows a number of square-shaped icons.
        */
        class UIImagePanel : public UIWidget 
        {
        public:
            typedef std::vector<std::pair<int, std::string>> Images;

        public:
            UIImagePanel();

            UIImagePanel(UIWidget *aParent);

            const Images& images() const { return mImages; }
            void setImages(const Images &data) { mImages = data; }

            int thumbSize() const { return mThumbSize; }
            void setThumbSize(const int& aThumbSize) { mThumbSize = aThumbSize; }

            int spacing() const { return mSpacing; }
            void setSpacing(const int& aSpacing) { mSpacing = aSpacing; }

            int margin() const { return mMargin; }
            void setMargin(const int& aMargin) { mMargin = aMargin; }

            //std::function<void(int)> callback() const { return mCallback; }
            //void setCallback(const std::function<void(int)> &callback) { mCallback = callback; }

            virtual bool mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers) override;
            virtual glm::ivec2 preferredSize(NVGcontext *ctx) const override;
            virtual void draw(NVGcontext* ctx) override;

            virtual void Serialize(Json::Value& aSerializer) const override;
            virtual void Deserialize(const Json::Value& aSerializer) override;

            inline int currentIndex() const { return mMouseIndex; }

        protected:
            glm::ivec2 gridSize() const;
            int indexForPosition(const glm::ivec2 &p) const;

        protected:
            Images                   mImages;
            //std::function<void(int)> mCallback;
            int                      mThumbSize;
            int                      mSpacing;
            int                      mMargin;
            int                      mMouseIndex;
//        public:
//            EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };
    }
}
