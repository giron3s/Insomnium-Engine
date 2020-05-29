/*
    nanogui/glcanvas.h -- Canvas widget for rendering OpenGL content

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/**
 * \file nanogui/canvas3D.h
 *
 * \brief Canvas widget for rendering OpenGL content.  This widget was
 *        contributed by Jan Winkler.
 */

#pragma once

#include <iostream>

#include "graphic/gui/widget.h"
#include "graphic/viewport.h"
#include "graphic/model2d.h"
#include "graphic/model3d.h"


namespace Framework
{
    class Scene;
    class Model3D;
}

namespace Framework
{
    namespace GUI
    {
        /**
        * \class UICanvasGL UICanvasGL.h
        * \brief Canvas widget for rendering OpenGL content
        *
        * Canvas widget that can be used to display arbitrary OpenGL content. This is
        * useful to display and manipulate 3D objects as part of an interactive
        * application. The implementation uses scissoring to ensure that rendered
        * objects don't spill into neighboring widgets.
        *
        * Usage: override `drawGL` in subclasses to provide custom drawing code.
        */
        class UICanvasGL : public UIWidget
        {
        public:
            /// Construct a new UICanvasGL
            UICanvasGL();

            /**
              * Creates a UICanvasGL attached to the specified parent.
              *
              * \param parent
              *     The Widget to attach this UICanvasGL to.
              */
            UICanvasGL(UIWidget *parent);

            ~UICanvasGL() override;

            bool keyboardEvent(int key, int scancode, int action, int modifiers) override;
            bool mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers) override;
            bool mouseMotionEvent(const glm::ivec2 &p, const glm::ivec2 &rel, int button, int modifiers) override;

            /// set the size of the widget
            void setSize(const glm::ivec2 &size) override;

            /// Handle a focus change event (default implementation: record the focus status, but do nothing)
            bool focusEvent(bool focused) override;

            /// Set the scene to render on this canvas
            void setScene(std::shared_ptr<Scene> aScene);

            /// Get the scene witch is render on this canvas
            std::shared_ptr<Scene> getScene() { return mScene; }

            /// Set whether to draw the widget border or not
            void setDrawBorder(const bool bDrawBorder) { mDrawBorder = bDrawBorder; }

            /// Return whether the widget border gets drawn or not
            const bool &drawBorder() const { return mDrawBorder; }

            /// Draw the canvas
            virtual void draw(NVGcontext *ctx) override;

            /// Draw the GL scene. Override this method to draw the actual GL content.
            void drawGL(const Viewport& aViewPort);

            /// Save the state of the widget into the given \ref Serializer instance
            void Serialize(Json::Value& aSerializer) const override;

            /// Restore the state of the widget from the given \ref Serializer instance
            void Deserialize(const Json::Value& aSerializer) override;

        protected:
            /// Internal helper function for drawing the widget border
            void drawWidgetBorder(NVGcontext* ctx) const;

            /// Enable or disable the camera movement
            void SetCameraMovementEnabled(bool aEnabled);

            ///Get the model from the (X,Y) screen coordinates
            Model::ReservedId GetModelAtPoint(int aX, int aY, __out Model*& aModel);

        protected:
            bool        mDrawBorder;            /// Whether to draw the widget border or not.
            bool        mRTNeedsResize;         /// this tells us if the canvas has been resized (in which case we need to resize the render target)

            bool        mRenderAABB;            /// Flag to enable model AABB rendering
            bool        mRenderOOBB;            /// Flag to enable model OOBB rendering
            bool        mRenderBoundingSphere;  /// Flag to enable model bounding sphere rendering

            std::shared_ptr<Scene> mScene;      /// Scene to render on this canvas

            //public:
            // EIGEN_MAKE_ALIGNED_OPERATOR_NEW 
        };
    }
}
