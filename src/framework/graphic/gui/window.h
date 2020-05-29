/*
    nanogui/window.h -- Top-level window widget

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
         * \class Window window.h nanogui/window.h
         *
         * \brief Top-level window widget.
         */
        class UIWindow : public UIWidget
        {
            friend class UIPopup;
        public:
            UIWindow();

            UIWindow(UIWidget *parent, const std::string &title = "@UNTITLED");

            virtual ~UIWindow();

            /// Return the window title
            const std::string &title() const { return mTitle; }
            /// Set the window title
            void setTitle(const std::string &title);

            /// Is this a model dialog?
            bool modal() const { return mModal; }
            /// Set whether or not this is a modal dialog
            void setModal(bool modal) { mModal = modal; }

            /// Return whether or not this widget is centered
            inline bool centered() const { return mCentered; }
            /// Set whether or not this widget is currently centered
            inline void setCentered(bool centered) { mCentered = centered; }

            /// Return whether or not this widget fill the parent
            inline bool fillParent() const { return mFillParent; }
            /// Set whether or not this widget fill the parent
            inline void setFillParent(bool fillParent) { mFillParent = fillParent; }

            /// Return the panel used to house window buttons
            UIWidget *buttonPanel();

            /// Dispose the window
            void dispose();

            /// Draw the window
            virtual void draw(NVGcontext *ctx) override;
            /// Handle window drag events
            virtual bool mouseDragEvent(const glm::ivec2 &p, const glm::ivec2 &rel, int button, int modifiers) override;
            /// Handle mouse events recursively and bring the current window to the top
            virtual bool mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers) override;
            /// Accept scroll events and propagate them to the widget under the mouse cursor
            virtual bool scrollEvent(const glm::ivec2 &p, const glm::vec2 &rel) override;
            /// Compute the preferred size of the widget
            virtual glm::ivec2 preferredSize(NVGcontext *ctx) const override;
            /// Invoke the associated layout generator to properly place child widgets, if any
            virtual void performLayout(NVGcontext *ctx) override;
            /// Save the state of the window into the given \ref Serializer instance
            virtual void Serialize(Json::Value& aSerializer) const override;
            /// Restore the state of the window from the given \ref Serializer instance
            virtual void Deserialize(const Json::Value& aSerializer) override;

        protected:
            /// Internal helper function to maintain nested window position values; overridden in \ref Popup
            virtual void refreshRelativePlacement();

            /// Center the window in the current \ref Screen
            void center();


        protected:
            std::string mTitle;
            UIWidget*   mButtonPanel;
            bool        mModal;
            bool        mDrag;
            bool        mCentered;
            bool        mFillParent;
        //public:
        //    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };
    }
}

