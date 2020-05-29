/*
    src/screen.cpp -- Top-level widget and interface between NanoGUI and GLFW

    A significant redesign of this code was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
#include "precompiled.h"

#include "core/logger.h"
#include "engine.h"
#include "graphic/gui/screen.h"
#include "graphic/gui/theme.h"
#include "graphic/gui/window.h"
#include "graphic/gui/popupbutton.h"

#if defined(_WIN32)
#  define NOMINMAX
#  undef APIENTRY

#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>

#  define GLFW_EXPOSE_NATIVE_WGL
#  define GLFW_EXPOSE_NATIVE_WIN32
#  include <GLFW/glfw3native.h>
#endif

/* Allow enforcing the GL2 implementation of NanoVG */
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg/nanovg_gl.h>

namespace Framework
{
    namespace GUI
    {
        std::map<GLFWwindow *, UIScreen *> __nanogui_screens;

#if defined(NANOGUI_GLAD)
        static bool gladInitialized = false;
#endif

        /* Calculate pixel ratio for hi-dpi devices. */
        static float get_pixel_ratio(GLFWwindow *window) {
#if defined(_WIN32)
            HWND hWnd = glfwGetWin32Window(window);
            HMONITOR monitor = nullptr;
#if defined(MONITOR_DEFAULTTONEAREST)
            monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
#else
            static HMONITOR(WINAPI *MonitorFromWindow_)(HWND, DWORD) = nullptr;
            static bool MonitorFromWindow_tried = false;
            if (!MonitorFromWindow_tried) {
                auto user32 = LoadLibrary(TEXT("user32"));
                if (user32)
                    MonitorFromWindow_ = (decltype(MonitorFromWindow_))GetProcAddress(user32, "MonitorFromWindow");
                MonitorFromWindow_tried = true;
            }
            if (MonitorFromWindow_)
                monitor = MonitorFromWindow_(hWnd, 2);
#endif  // defined(MONITOR_DEFAULTTONEAREST)
            /* The following function only exists on Windows 8.1+, but we don't want to make that a dependency */
            static HRESULT(WINAPI *GetDpiForMonitor_)(HMONITOR, UINT, UINT*, UINT*) = nullptr;
            static bool GetDpiForMonitor_tried = false;

            if (!GetDpiForMonitor_tried) {
                auto shcore = LoadLibrary(TEXT("shcore"));
                if (shcore)
                    GetDpiForMonitor_ = (decltype(GetDpiForMonitor_))GetProcAddress(shcore, "GetDpiForMonitor");
                GetDpiForMonitor_tried = true;
            }

            if (GetDpiForMonitor_ && monitor) {
                uint32_t dpiX, dpiY;
                if (GetDpiForMonitor_(monitor, 0 /* effective DPI */, &dpiX, &dpiY) == S_OK)
                    return dpiX / 96.0;
            }
            return 1.f;
#elif defined(__linux__)
            (void)window;

            float ratio = 1.0f;
            FILE *fp;
            /* Try to read the pixel ratio from KDEs config */
            auto currentDesktop = std::getenv("XDG_CURRENT_DESKTOP");
            if (currentDesktop && currentDesktop == std::string("KDE")) {
                fp = popen("kreadconfig5 --group KScreen --key ScaleFactor", "r");
                if (!fp)
                    return 1;

                if (fscanf(fp, "%f", &ratio) != 1)
                    return 1;
            }
            else {
                /* Try to read the pixel ratio from GTK */
                fp = popen("gsettings get org.gnome.desktop.interface scaling-factor", "r");
                if (!fp)
                    return 1;

                int ratioInt = 1;
                if (fscanf(fp, "uint32 %i", &ratioInt) != 1)
                    return 1;
                ratio = ratioInt;
            }
            if (pclose(fp) != 0)
                return 1;
            return ratio >= 1 ? ratio : 1;

#else
            glm::ivec2 fbSize, size;
            glfwGetFramebufferSize(window, &fbSize[0], &fbSize[1]);
            glfwGetWindowSize(window, &size[0], &size[1]);
            return (float)fbSize[0] / (float)size[0];
#endif
        }

        UIPopupButton * UIScreen::sActivePopupButton = nullptr;

        UIScreen::UIScreen()
            : UIWidget(nullptr), mGLFWWindow(nullptr), mNVGContext(nullptr),
            mCursor(Cursor::eArrow), mBackground(0.3f, 0.3f, 0.32f, 1.f),
            mShutdownGLFWOnDestruct(false), mFullscreen(false) {
            memset(mCursors, 0, sizeof(GLFWcursor *) * (int)Cursor::eCursorCount);
        }

        void UIScreen::initialize(const glm::ivec2 &aSize, const std::string &aCaption, bool aResizable,
            bool aFullscreen, int aColorBits, int aAlphaBits, int aDepthBits,
            int aStencilBits, int nSamples, unsigned int aGLMajor, unsigned int aGLMinor)
        {
            mCaption = aCaption;
            mFullscreen = aFullscreen;
            mId = Logger::ClassName(__FUNCSIG__);

            /* Request a forward compatible OpenGL glMajor.glMinor core profile context.
            Default value is an OpenGL 3.3 core profile context. */
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, aGLMajor);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, aGLMinor);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // we need glLineWidth !
            glfwWindowHint(GLFW_SRGB_CAPABLE, GL_FALSE);
            glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);


            glfwWindowHint(GLFW_SAMPLES, nSamples);
            glfwWindowHint(GLFW_RED_BITS, aColorBits);
            glfwWindowHint(GLFW_GREEN_BITS, aColorBits);
            glfwWindowHint(GLFW_BLUE_BITS, aColorBits);
            glfwWindowHint(GLFW_ALPHA_BITS, aAlphaBits);
            glfwWindowHint(GLFW_STENCIL_BITS, aStencilBits);
            glfwWindowHint(GLFW_DEPTH_BITS, aDepthBits);
            glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
            glfwWindowHint(GLFW_RESIZABLE, aResizable ? GL_TRUE : GL_FALSE);

            if (aFullscreen)
            {
                GLFWmonitor *monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode *mode = glfwGetVideoMode(monitor);
                mGLFWWindow = glfwCreateWindow(mode->width, mode->height, aCaption.c_str(), monitor, nullptr);
            }
            else
            {
                mGLFWWindow = glfwCreateWindow(aSize.x, aSize.y, aCaption.c_str(), nullptr, nullptr);
            }

            if (!mGLFWWindow)
                throw std::runtime_error("Could not create an OpenGL " +
                    std::to_string(aGLMajor) + "." +
                    std::to_string(aGLMinor) + " context!");

            glfwMakeContextCurrent(mGLFWWindow);

#if defined(NANOGUI_GLAD)
            if (!gladInitialized) {
                gladInitialized = true;
                if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
                    throw std::runtime_error("Could not initialize GLAD!");
                glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM
            }
#endif
            if (glewInit() != GLEW_OK) {
                throw std::runtime_error("Could not initialize GLEW!");
                glGetError();
            }


            glfwGetFramebufferSize(mGLFWWindow, &mFBSize[0], &mFBSize[1]);
            glViewport(0, 0, mFBSize[0], mFBSize[1]);
            glClearColor(mBackground.r(), mBackground.g(), mBackground.b(), mBackground.a());
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glfwSwapInterval(0);
            glfwSwapBuffers(mGLFWWindow);

            float lSmoothLineWidthRange[2] = { 0.0f, 0.0f };
            glGetFloatv(GL_SMOOTH_LINE_WIDTH_RANGE, lSmoothLineWidthRange);
            float lLineWidth = Engine::Instance()->Config().GetFocusLineWidth();
            if (!(lSmoothLineWidthRange[0] <= lLineWidth && lLineWidth <= lSmoothLineWidthRange[1]))
                CRASH("Invalid 'focusLineWidth' config value. The 'focusLineWidth' = %f but valid range is [%f, %f]",
                    lLineWidth, lSmoothLineWidthRange[0], lSmoothLineWidthRange[1]);
            lLineWidth = Engine::Instance()->Config().GetSelectionLineWidth();
            if (!(lSmoothLineWidthRange[0] <= lLineWidth && lLineWidth <= lSmoothLineWidthRange[1]))
                CRASH("Invalid 'selectionLineWidth' config value. The 'selectionLineWidth' = %f but valid range is [%f, %f]",
                    lLineWidth, lSmoothLineWidthRange[0], lSmoothLineWidthRange[1]);

#if defined(__APPLE__)
            /* Poll for events once before starting a potentially
            lengthy loading process. This is needed to be
            classified as "interactive" by other software such
            as iTerm2 */

            glfwPollEvents();
#endif

            /* Propagate GLFW events to the appropriate UIScreen instance */
            glfwSetWindowCloseCallback(mGLFWWindow,
                [](GLFWwindow *w) {
                Engine::Instance()->GameController().OnQuit();
            }
            );


            glfwSetCursorPosCallback(mGLFWWindow,
                [](GLFWwindow *w, double x, double y)
            {
                EventData lEventData;
                lEventData.type = static_cast<uint32_t>(InputEventType::MouseMotionEvent);
                lEventData.data.motion.x = x;
                lEventData.data.motion.y = y;
                Engine::Instance()->HandleInputEvent(lEventData);
            }
            );

            glfwSetMouseButtonCallback(mGLFWWindow,
                [](GLFWwindow *w, int button, int action, int modifiers)
            {
                EventData lEventData;
                lEventData.type = static_cast<uint32_t>(InputEventType::MouseButtonEvent);
                lEventData.data.button.button = button;
                lEventData.data.button.action = action;
                lEventData.data.button.modifiers = modifiers;
                Engine::Instance()->HandleInputEvent(lEventData);
            }
            );

            glfwSetKeyCallback(mGLFWWindow,
                [](GLFWwindow *w, int key, int scancode, int action, int mods)
            {
                EventData lEventData;
                lEventData.type = static_cast<uint32_t>(InputEventType::KeyboardEvent);
                lEventData.data.key.keyboardKey = key;
                lEventData.data.key.scancode = scancode;
                lEventData.data.key.action = action;
                lEventData.data.key.modifiers = mods;
                Engine::Instance()->HandleInputEvent(lEventData);
            }
            );

            glfwSetCharCallback(mGLFWWindow,
                [](GLFWwindow *w, unsigned int codepoint) {
                auto it = __nanogui_screens.find(w);
                if (it == __nanogui_screens.end())
                    return;
                UIScreen *s = it->second;
                if (!s->mProcessEvents)
                    return;
                s->charCallbackEvent(codepoint);
            }
            );

            glfwSetDropCallback(mGLFWWindow,
                [](GLFWwindow *w, int count, const char **filenames)
            {
                EventData lEventData;
                lEventData.type = static_cast<uint32_t>(InputEventType::FileDropEvent);
                lEventData.data.filedrop.count = count;
                lEventData.data.filedrop.filenames = filenames;
                Engine::Instance()->HandleInputEvent(lEventData);
            }
            );

            glfwSetScrollCallback(mGLFWWindow,
                [](GLFWwindow *w, double x, double y)
            {
                EventData lEventData;
                lEventData.type = static_cast<uint32_t>(InputEventType::ScrollEvent);
                lEventData.data.scrool.xoffset = x;
                lEventData.data.scrool.yoffset = y;
                Engine::Instance()->HandleInputEvent(lEventData);
            }
            );

            /* React to framebuffer size events -- includes window
            size events and also catches things like dragging
            a window from a Retina-capable screen to a normal
            screen on Mac OS X */
            glfwSetFramebufferSizeCallback(mGLFWWindow,
                [](GLFWwindow *w, int width, int height)
            {
                EventData lEventData;
                lEventData.type = static_cast<uint32_t>(InputEventType::ResizeEvent);
                lEventData.data.resize.width = width;
                lEventData.data.resize.height = height;
                Engine::Instance()->HandleInputEvent(lEventData);
            }
            );

            // notify when the screen has lost focus (e.g. application switch)
            glfwSetWindowFocusCallback(mGLFWWindow,
                [](GLFWwindow *w, int focused)
            {
                EventData lEventData;
                lEventData.type = static_cast<uint32_t>(InputEventType::FocusEvent);
                lEventData.data.focus.focused = focused;
                Engine::Instance()->HandleInputEvent(lEventData);
            }
            );

            mShutdownGLFWOnDestruct = true;
            glfwGetWindowSize(mGLFWWindow, &mSize[0], &mSize[1]);
            glfwGetFramebufferSize(mGLFWWindow, &mFBSize[0], &mFBSize[1]);

            mPixelRatio = get_pixel_ratio(mGLFWWindow);

#if defined(_WIN32) || defined(__linux__)
            if (mPixelRatio != 1 && !mFullscreen)
                glfwSetWindowSize(mGLFWWindow, mSize.x * mPixelRatio, mSize.y * mPixelRatio);
#endif

#if defined(NANOGUI_GLAD)
            if (!gladInitialized) {
                gladInitialized = true;
                if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
                    throw std::runtime_error("Could not initialize GLAD!");
                glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM
            }
#endif

            /* Detect framebuffer properties and set up compatible NanoVG context */
            GLint lStencilBits = 0, lSamples = 0;
            glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER,
                GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &lStencilBits);
            glGetIntegerv(GL_SAMPLES, &lSamples);

            int flags = 0;
            if (lStencilBits >= 8)
                flags |= NVG_STENCIL_STROKES;
            if (lSamples <= 1)
                flags |= NVG_ANTIALIAS;
#if !defined(NDEBUG)
            flags |= NVG_DEBUG;
#endif

            mNVGContext = nvgCreateGL3(flags);
            if (mNVGContext == nullptr)
                throw std::runtime_error("Could not initialize NanoVG!");

            mVisible = glfwGetWindowAttrib(mGLFWWindow, GLFW_VISIBLE) != 0;

            mMousePos = glm::ivec2(0);
            mMouseState = mModifiers = 0;
            mDragActive = false;
            mLastInteraction = Engine::Instance()->GameController().GetThisTime();
            mProcessEvents = true;
            __nanogui_screens[mGLFWWindow] = this;

            for (int i = 0; i < (int)Cursor::eCursorCount; ++i)
                mCursors[i] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR + i);

            /// Fixes retina display-related font rendering issue (#185)
            nvgBeginFrame(mNVGContext, mSize[0], mSize[1], mPixelRatio);
            nvgEndFrame(mNVGContext);

            setTheme(new UITheme(mNVGContext, Engine::Instance()->Config().GetThemeFile()));
        }

        UIScreen::~UIScreen() {
            __nanogui_screens.erase(mGLFWWindow);
            for (int i = 0; i < (int)Cursor::eCursorCount; ++i) {
                if (mCursors[i])
                    glfwDestroyCursor(mCursors[i]);
            }
            if (mNVGContext)
                nvgDeleteGL3(mNVGContext);
            if (mGLFWWindow && mShutdownGLFWOnDestruct)
                glfwDestroyWindow(mGLFWWindow);
        }

        void UIScreen::setVisible(bool visible) {
            if (mVisible != visible) {
                mVisible = visible;

                if (visible)
                    glfwShowWindow(mGLFWWindow);
                else
                    glfwHideWindow(mGLFWWindow);
            }
        }

        void UIScreen::setCaption(const std::string &caption) {
            if (caption != mCaption) {
                glfwSetWindowTitle(mGLFWWindow, caption.c_str());
                mCaption = caption;
            }
        }

        void UIScreen::setSize(const glm::ivec2 &size)
        {
            UIWidget::setSize(size);

#if defined(_WIN32) || defined(__linux__)
            glfwSetWindowSize(mGLFWWindow, size.x * mPixelRatio, size.y * mPixelRatio);
#else
            glfwSetWindowSize(mGLFWWindow, size.x, size.y);
#endif
        }

        void UIScreen::drawAll() {
            glClearColor(mBackground.r(), mBackground.g(), mBackground.b(), mBackground.a());
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            //Render the screen widgets
            drawWidgets();

            glfwSwapBuffers(mGLFWWindow);
        }

        void UIScreen::drawWidgets() {
            if (!mVisible)
                return;

            glfwMakeContextCurrent(mGLFWWindow);

            glfwGetFramebufferSize(mGLFWWindow, &mFBSize[0], &mFBSize[1]);
            glfwGetWindowSize(mGLFWWindow, &mSize[0], &mSize[1]);

#if defined(_WIN32) || defined(__linux__)
            mSize.x = static_cast<int>(static_cast<float>(mSize.x) / mPixelRatio);
            mSize.y = static_cast<int>(static_cast<float>(mSize.y) / mPixelRatio);

            mFBSize.x = static_cast<int>(static_cast<float>(mSize.x) * mPixelRatio);
            mFBSize.y = static_cast<int>(static_cast<float>(mSize.y) * mPixelRatio);

#else
            /* Recompute pixel ratio on OSX */
            if (mSize[0])
                mPixelRatio = (float)mFBSize[0] / (float)mSize[0];
#endif

            glViewport(0, 0, mFBSize[0], mFBSize[1]);
            glBindSampler(0, 0);
            nvgBeginFrame(mNVGContext, mSize[0], mSize[1], mPixelRatio);

            draw(mNVGContext);

            /* Draw tooltips */
            /*double lElapsed = Engine::Instance()->GameController().GetThisTime() - mLastInteraction;
            if (lElapsed > 0.5f)
            {
                const UIWidget *widget = findWidget(mMousePos);
                if (widget && !widget->tooltip().empty()) {
                    int tooltipWidth = 150;

                    float bounds[4];
                    nvgFontFace(mNVGContext, "sans");
                    nvgFontSize(mNVGContext, 15.0f);
                    nvgTextAlign(mNVGContext, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
                    nvgTextLineHeight(mNVGContext, 1.1f);
                    glm::ivec2 pos = widget->absolutePosition() +
                        glm::ivec2(widget->width() / 2, widget->height() + 10);

                    nvgTextBounds(mNVGContext, pos.x, pos.y,
                        widget->tooltip().c_str(), nullptr, bounds);
                    int h = (bounds[2] - bounds[0]) / 2;
                    if (h > tooltipWidth / 2) {
                        nvgTextAlign(mNVGContext, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
                        nvgTextBoxBounds(mNVGContext, pos.x, pos.y, tooltipWidth,
                            widget->tooltip().c_str(), nullptr, bounds);

                        h = (bounds[2] - bounds[0]) / 2;
                    }
                    
                    float tooltipsAlpha = std::min(1.0, 3 * (lElapsed - 0.5f)) * 0.8;
                    nvgGlobalAlpha(mNVGContext, tooltipsAlpha);

                    nvgBeginPath(mNVGContext);
                    nvgFillColor(mNVGContext, Color(0, 255));
                    nvgRoundedRect(mNVGContext, bounds[0] - 4 - h, bounds[1] - 4,
                        (int)(bounds[2] - bounds[0]) + 8,
                        (int)(bounds[3] - bounds[1]) + 8, 3);

                    int px = (int)((bounds[2] + bounds[0]) / 2) - h;
                    nvgMoveTo(mNVGContext, px, bounds[1] - 10);
                    nvgLineTo(mNVGContext, px + 7, bounds[1] + 1);
                    nvgLineTo(mNVGContext, px - 7, bounds[1] + 1);
                    nvgFill(mNVGContext);

                    nvgFillColor(mNVGContext, Color(255, 255));
                    nvgFontBlur(mNVGContext, 0.0f);
                    nvgTextBox(mNVGContext, pos.x - h, pos.y, tooltipWidth,
                        widget->tooltip().c_str(), nullptr);
                }
            }*/

            nvgEndFrame(mNVGContext);
        }

        bool UIScreen::keyboardEvent(int key, int scancode, int action, int modifiers) {
            if (mFocusPath.size() > 0) {
                for (auto it = mFocusPath.rbegin() + 1; it != mFocusPath.rend(); ++it)
                    if ((*it)->focused() && (*it)->keyboardEvent(key, scancode, action, modifiers))
                        return true;
            }

            return false;
        }

        bool UIScreen::keyboardCharacterEvent(unsigned int codepoint) {
            if (mFocusPath.size() > 0) {
                for (auto it = mFocusPath.rbegin() + 1; it != mFocusPath.rend(); ++it)
                    if ((*it)->focused() && (*it)->keyboardCharacterEvent(codepoint))
                        return true;
            }
            return false;
        }

        bool UIScreen::resizeEvent(const glm::ivec2& size) {
            if (mResizeCallback) {
                mResizeCallback(size);
                return true;
            }
            return false;
        }

        bool UIScreen::cursorPosCallbackEvent(double x, double y) {
            glm::ivec2 p((int)x, (int)y);

#if defined(_WIN32) || defined(__linux__)
            p.x = static_cast<int>(static_cast<float>(p.x) / mPixelRatio);
            p.y = static_cast<int>(static_cast<float>(p.y) / mPixelRatio);
#endif

            bool ret = false;
            mLastInteraction = Engine::Instance()->GameController().GetThisTime();
            try {
                p -= glm::ivec2(1, 2);

                if (!mDragActive) {
                    UIWidget *widget = findWidget(p);
                    if (widget != nullptr && widget->cursor() != mCursor) {
                        mCursor = widget->cursor();
                        glfwSetCursor(mGLFWWindow, mCursors[(int)mCursor]);
                    }
                }
                else {
                    ret = mDragWidget->mouseDragEvent(
                        p - mDragWidget->parent()->absolutePosition(), p - mMousePos,
                        mMouseState, mModifiers);
                }

                if (!ret)
                    ret = mouseMotionEvent(p, p - mMousePos, mMouseState, mModifiers);

                mMousePos = p;

                return ret;
            }
            catch (const std::exception &e) {
                std::cerr << "Caught exception in event handler: " << e.what() << std::endl;
                return false;
            }
        }

        bool UIScreen::mouseButtonCallbackEvent(int button, int action, int modifiers)
        {
            mModifiers = modifiers;
            mLastInteraction = Engine::Instance()->GameController().GetThisTime();
            try
            {
                if (mFocusPath.size() > 1)
                {
                    const UIWindow *window = dynamic_cast<UIWindow *>(mFocusPath[mFocusPath.size() - 2]);
                    if (window && window->modal())
                    {
                        if (!window->contains(mMousePos))
                            return false;
                    }
                }

                if (action == GLFW_PRESS)
                    mMouseState |= 1 << button;
                else
                    mMouseState &= ~(1 << button);

                auto lDropWidget = findWidget(mMousePos);
                if (mDragActive && action == GLFW_RELEASE && lDropWidget != mDragWidget)
                {
                    mDragWidget->mouseButtonEvent(mMousePos - mDragWidget->parent()->absolutePosition(), button, false, mModifiers);
                }

                if (lDropWidget != nullptr && mDragWidget != nullptr && lDropWidget != mDragWidget)
                {
                    mCursor = lDropWidget->cursor();
                    glfwSetCursor(mGLFWWindow, mCursors[(int)mCursor]);

                    lDropWidget->dropEvent(mMousePos - mDragWidget->parent()->absolutePosition(), button, mModifiers, &mDragDropData);
                    updateFocus(lDropWidget);
                    mDragDropData.Clear();
                }

                if (action == GLFW_PRESS && (button == GLFW_MOUSE_BUTTON_1 || button == GLFW_MOUSE_BUTTON_2))
                {
                    UIWidget *lDragWidget = findWidget(mMousePos);

                    //Dragged widget is the screen
                    if (lDragWidget == this)
                        lDragWidget = nullptr;
                    //Dragged widget is the screen
                    else
                        lDragWidget->dragEvent(mMousePos - lDragWidget->parent()->absolutePosition(), button, mModifiers, &mDragDropData);

                    //Dragged item recieve the focus
                    updateFocus(lDragWidget);
                    mDragWidget = lDragWidget;
                    mDragActive = mDragWidget != nullptr;
                }
                else
                {
                    mDragActive = false;
                    mDragWidget = nullptr;
                    mDragDropData.Clear();
                }


                //ActivePopup on the scene and clicked widget is not contain popup, close the ActivePopup
                if (sActivePopupButton && (UIWidget*)sActivePopupButton != lDropWidget &&
                    action == GLFW_PRESS && (button == GLFW_MOUSE_BUTTON_1 || button == GLFW_MOUSE_BUTTON_2))
                {

                    if (!sActivePopupButton->popup()->isChild(lDropWidget))
                    {
                        sActivePopupButton->setPushed(false);
                        UIPopupButton::CollapseChildPopups(sActivePopupButton);
                        sActivePopupButton = nullptr;
                    }
                }


                return mouseButtonEvent(mMousePos, button, action == GLFW_PRESS, mModifiers);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Caught exception in event handler: " << e.what() << std::endl;
                return false;
            }
        }

        bool UIScreen::keyCallbackEvent(int key, int scancode, int action, int mods) {
            mLastInteraction = Engine::Instance()->GameController().GetThisTime();
            try {
                return keyboardEvent(key, scancode, action, mods);
            }
            catch (const std::exception &e) {
                std::cerr << "Caught exception in event handler: " << e.what() << std::endl;
                return false;
            }
        }

        bool UIScreen::charCallbackEvent(unsigned int codepoint) {
            mLastInteraction = Engine::Instance()->GameController().GetThisTime();
            try {
                return keyboardCharacterEvent(codepoint);
            }
            catch (const std::exception &e) {
                std::cerr << "Caught exception in event handler: " << e.what()
                    << std::endl;
                return false;
            }
        }

        bool UIScreen::dropCallbackEvent(int count, const char **filenames) {
            std::vector<std::string> arg(count);
            for (int i = 0; i < count; ++i)
                arg[i] = filenames[i];
            return dropEvent(arg);
        }

        bool UIScreen::scrollCallbackEvent(double x, double y) {
            mLastInteraction = Engine::Instance()->GameController().GetThisTime();
            try {
                if (mFocusPath.size() > 1) {
                    const UIWindow *window =
                        dynamic_cast<UIWindow *>(mFocusPath[mFocusPath.size() - 2]);
                    if (window && window->modal()) {
                        if (!window->contains(mMousePos))
                            return false;
                    }
                }
                return scrollEvent(mMousePos, glm::vec2(x, y));
            }
            catch (const std::exception &e) {
                std::cerr << "Caught exception in event handler: " << e.what()
                    << std::endl;
                return false;
            }
        }

        bool UIScreen::resizeCallbackEvent(int, int) {
            glm::ivec2 fbSize, size;
            glfwGetFramebufferSize(mGLFWWindow, &fbSize[0], &fbSize[1]);
            glfwGetWindowSize(mGLFWWindow, &size[0], &size[1]);

#if defined(_WIN32) || defined(__linux__)
            size.x = static_cast<int>(static_cast<float>(size.x) / mPixelRatio);
            size.y = static_cast<int>(static_cast<float>(size.y) / mPixelRatio);
#endif

            if (mFBSize == glm::ivec2(0, 0) || size == glm::ivec2(0, 0))
                return false;

            mFBSize = fbSize; mSize = size;
            mLastInteraction = Engine::Instance()->GameController().GetThisTime();


            for (auto lIter = mChildren.begin(); lIter != mChildren.end(); ++lIter)
            {
                UIWindow *lWidget = dynamic_cast<UIWindow *>(*lIter);
                if (lWidget != nullptr && lWidget->fillParent())
                    lWidget->setFixedSize(glm::ivec2(size.x, size.y));
            }
            performLayout();

            try {
                return resizeEvent(mSize);
            }
            catch (const std::exception &e) {
                std::cerr << "Caught exception in event handler: " << e.what()
                    << std::endl;
                return false;
            }
        }

        void UIScreen::updateFocus(UIWidget *widget)
        {
            for (auto w : mFocusPath) {
                if (!w->focused())
                    continue;
                w->focusEvent(false);
            }
            mFocusPath.clear();
            mDragWidget = nullptr;
            mDragActive = false;

            UIWidget *window = nullptr;
            while (widget) {
                mFocusPath.push_back(widget);
                if (dynamic_cast<UIWindow *>(widget))
                    window = widget;
                widget = widget->parent();
            }
            for (auto it = mFocusPath.rbegin(); it != mFocusPath.rend(); ++it)
                (*it)->focusEvent(true);
        }

        void UIScreen::disposeWindow(UIWindow *window) {
            if (std::find(mFocusPath.begin(), mFocusPath.end(), window) != mFocusPath.end())
                mFocusPath.clear();
            if (mDragWidget == window)
                mDragWidget = nullptr;
            removeChild(window);
        }

        void UIScreen::centerWindow(UIWindow *window)
        {
            if (window->size() == glm::ivec2(0)) {
                window->setSize(window->preferredSize(mNVGContext));
                window->performLayout(mNVGContext);
            }
            window->setPosition((mSize - window->size()) / 2);
        }

        void UIScreen::moveWindowToFront(UIWindow *window)
        {
            mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), window), mChildren.end());
            mChildren.push_back(window);
            /* Brute force topological sort (no problem for a few windows..) */
            bool changed = false;
            do {
                size_t baseIndex = 0;
                for (size_t index = 0; index < mChildren.size(); ++index)
                    if (mChildren[index] == window)
                        baseIndex = index;
                changed = false;
                for (size_t index = 0; index < mChildren.size(); ++index) {
                    UIPopup *pw = dynamic_cast<UIPopup *>(mChildren[index]);
                    if (pw && pw->parentWindow() == window && index < baseIndex) {
                        moveWindowToFront(pw);
                        changed = true;
                        break;
                    }
                }
            } while (changed);
        }

        void UIScreen::Serialize(Json::Value& aSerialize) const
        {

        }

        void UIScreen::Deserialize(const Json::Value& aSerialize)
        {


        }
    }
}
