/*
    nanogui/screen.h -- Top-level widget and interface between NanoGUI and GLFW

    A significant redesign of this code was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include "graphic/gui/cursor.h"
#include "graphic/gui/widget.h"
#include "graphic/gui/dragdropdata.h"

namespace Framework
{
    namespace GUI
    {
        /**
         * \class Screen screen.h nanogui/screen.h
         *
         * \brief Represents a display surface (i.e. a full-screen or windowed GLFW window)
         * and forms the root element of a hierarchy of nanogui widgets.
         */
        class UIScreen : public UIWidget

        {
            friend class UIWidget;
            friend class UIWindow;
        public:
            static class UIPopupButton * sActivePopupButton;

            /********* API for applications which manage GLFW themselves *********/

            /**
             * \brief Default constructor
             *
             * Performs no initialization at all. Use this if the application is
             * responsible for setting up GLFW, OpenGL, etc.
             *
             * In this case, override \ref Screen and call \ref initalize() with a
             * pointer to an existing \c GLFWwindow instance
             *
             * You will also be responsible in this case to deliver GLFW callbacks
             * to the appropriate callback event handlers below
             */
            UIScreen();

            /**
             * Initialize a Screen instance
             *
             * \param size
             *     Size in pixels at 96 dpi (on high-DPI screens, the actual resolution
             *     in terms of hardware pixels may be larger by an integer factor)
             *
             * \param caption
             *     Window title (in UTF-8 encoding)
             *
             * \param resizable
             *     If creating a window, should it be resizable?
             *
             * \param fullscreen
             *     Specifies whether to create a windowed or full-screen view
             *
             * \param colorBits
             *     Number of bits per pixel dedicated to the R/G/B color components
             *
             * \param alphaBits
             *     Number of bits per pixel dedicated to the alpha channel
             *
             * \param depthBits
             *     Number of bits per pixel dedicated to the Z-buffer
             *
             * \param stencilBits
             *     Number of bits per pixel dedicated to the stencil buffer (recommended
             *     to set this to 8. NanoVG can draw higher-quality strokes using a
             *     stencil buffer)
             *
             * \param nSamples
             *     Number of MSAA samples (set to 0 to disable)
             *
             * \param glMajor
             *     The requested OpenGL Major version number.  Default is 3, if changed
             *     the value must correspond to a forward compatible core profile (for
             *     portability reasons).  For example, set this to 4 and \ref glMinor to 1
             *     for a forward compatible core OpenGL 4.1 profile.  Requesting an
             *     invalid profile will result in no context (and therefore no GUI)
             *     being created.
             *
             * \param glMinor
             *     The requested OpenGL Minor version number.  Default is 3, if changed
             *     the value must correspond to a forward compatible core profile (for
             *     portability reasons).  For example, set this to 1 and \ref glMajor to 4
             *     for a forward compatible core OpenGL 4.1 profile.  Requesting an
             *     invalid profile will result in no context (and therefore no GUI)
             *     being created.
             */
            void initialize(const glm::ivec2 &size, const std::string &caption,
                            bool resizable = true, bool fullscreen = false, int colorBits = 8,
                            int alphaBits = 8, int depthBits = 24, int stencilBits = 8,
                            int nSamples = 0, unsigned int glMajor = 3, unsigned int glMinor = 3);

            /// Release all resources
            virtual ~UIScreen();

            /// Get the window title bar caption
            const std::string &caption() const { return mCaption; }

            /// Set the window title bar caption
            void setCaption(const std::string &caption);

            /// Return the screen's background color
            const Color &background() const { return mBackground; }

            /// Set the screen's background color
            void setBackground(const Color &background) { mBackground = background; }

            /// Set the top-level window visibility (no effect on full-screen windows)
            void setVisible(bool visible);

            /// Set window size
            void setSize(const glm::ivec2& size);

            /// Draw the Screen contents
            virtual void drawAll();

            /// Return the ratio between pixel and device coordinates (e.g. >= 2 on Mac Retina displays)
            float pixelRatio() const { return mPixelRatio; }

            /// Handle a file drop event
            virtual bool dropEvent(const std::vector<std::string> & /* filenames */) { return false; /* To be overridden */ }

            /// Default keyboard event handler
            virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);

            /// Text input event handler: codepoint is native endian UTF-32 format
            virtual bool keyboardCharacterEvent(unsigned int codepoint);

            /// Window resize event handler
            virtual bool resizeEvent(const glm::ivec2& size);

            /// Set the resize callback
            std::function<void(glm::ivec2)> resizeCallback() const { return mResizeCallback; }
            void setResizeCallback(const std::function<void(glm::ivec2)> &callback) { mResizeCallback = callback; }

            /// Return the last observed mouse position value
            glm::ivec2 mousePos() const { return mMousePos; }

            /// Return a pointer to the underlying GLFW window data structure
            GLFWwindow *glfwWindow() { return mGLFWWindow; }

            /// Return a pointer to the underlying nanoVG draw context
            NVGcontext *nvgContext() { return mNVGContext; }

            void setShutdownGLFWOnDestruct(bool v) { mShutdownGLFWOnDestruct = v; }
            bool shutdownGLFWOnDestruct() { return mShutdownGLFWOnDestruct; }

            using UIWidget::performLayout;

            /// Compute the layout of all widgets
            void performLayout() 
            {
                UIWidget::performLayout(mNVGContext);
            }

            /// Save the state of the scene into the given \ref Serializer instance
            virtual void Serialize(Json::Value& aSerialize) const override;
            /// Restore the state of the scene from the given \ref Serializer instance
            virtual void Deserialize(const Json::Value& aSerialize) override;

            /* Event handlers */
            bool cursorPosCallbackEvent(double x, double y);
            bool mouseButtonCallbackEvent(int button, int action, int modifiers);
            bool keyCallbackEvent(int key, int scancode, int action, int mods);
            bool charCallbackEvent(unsigned int codepoint);
            bool dropCallbackEvent(int count, const char **filenames);
            bool scrollCallbackEvent(double x, double y);
            bool resizeCallbackEvent(int width, int height);

            /* Internal helper functions */
            void updateFocus(UIWidget *widget);
            void disposeWindow(UIWindow *window);
            void centerWindow(UIWindow *window);
            void moveWindowToFront(UIWindow *window);
            void drawWidgets();

        protected:
            GLFWwindow*             mGLFWWindow;
            NVGcontext*             mNVGContext;
            GLFWcursor*             mCursors[(int) Cursor::eCursorCount];
            Cursor                  mCursor;
            std::vector<UIWidget*>  mFocusPath;
            glm::ivec2              mFBSize;
            float                   mPixelRatio;

            int                     mMouseState;
            int                     mModifiers;
            glm::ivec2              mMousePos;

            bool                    mDragActive;
            UIWidget*               mDragWidget = nullptr;
            UIDragDropData          mDragDropData;

            double                  mLastInteraction;
            bool                    mProcessEvents;
            Color                   mBackground;
            std::string             mCaption;
            bool                    mShutdownGLFWOnDestruct;
            bool                    mFullscreen;
            std::function<void(glm::ivec2)> mResizeCallback;

        //public:
        //    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };
    }
}

