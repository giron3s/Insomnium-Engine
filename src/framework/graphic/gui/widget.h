/*
    nanogui/widget.h -- Base class of all widgets

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <vector>
#include "graphic/gui/uiobject.h"
#include "core/math.h"
#include "core/serialization/serializableobject.h"


namespace Framework
{
    class UI;
};

namespace Framework
{
    namespace GUI
    {
        class UIDragDropData;
    }
};


namespace Framework
{
    namespace GUI
    {
        enum class Cursor;// do not put a docstring, this is already documented

        /**
         * \class UIWidget widget.h nanogui/widget.h
         *
         * \brief Base class of all widgets.
         *
         * \ref UIWidget is the base class of all widgets in \c nanogui. It can
         * also be used as an panel to arrange an arbitrary number of child
         * widgets using a layout generator (see \ref Layout).
         */
        class UIWidget : public UIObject,
                         public SerializableObject,
                         public std::enable_shared_from_this<UIWidget>
        {
        public:
            /// Construct a new widget
            UIWidget();

            /// Construct a new widget with the given parent widget
            UIWidget(UIWidget *parent);

            /// Free all resources used by the widget and any children
            virtual ~UIWidget();

            virtual void destroyUIObject();

            void setOwner( UI* aUI ) { mOwner = aUI; }

            /// Return the parent widget
            UIWidget *parent() { return mParent; }
            /// Return the parent widget
            const UIWidget *parent() const { return mParent; }
            /// Set the parent widget
            void setParent(UIWidget *parent) { mParent = parent; }

            /// Return the used \ref Layout generator
            UILayout* layout() { return mLayout; }
            /// Return the used \ref Layout generator
            const UILayout* layout() const { return mLayout; }
            /// Set the used \ref Layout generator
            void setLayout( UILayout* aLayout) { mLayout = aLayout; }

            /// Return the \ref Theme used to draw this widget
            UITheme* theme() { return mTheme; }
            /// Return the \ref Theme used to draw this widget
            const UITheme *theme() const { return mTheme.get(); }
            /// Set the \ref Theme used to draw this widget
            virtual void setTheme(UITheme *theme);

            /// Return the position relative to the parent widget
            const glm::ivec2 &position() const { return mPos; }
            /// Set the position relative to the parent widget
            void setPosition(const glm::ivec2 &pos) { mPos = pos; }

            /// Return the absolute position on screen
            glm::ivec2 absolutePosition() const {
                return mParent ?
                    (parent()->absolutePosition() + mPos) : mPos;
            }

            /// Return the size of the widget
            const glm::ivec2 &size() const { return mSize; }
            /// set the size of the widget
            virtual void setSize(const glm::ivec2 &size) { mSize = size; }

            /// Return the width of the widget
            int width() const { return mSize.x; }
            /// Set the width of the widget
            void setWidth(int width) { mSize.x = width; }

            /// Return the height of the widget
            int height() const { return mSize.y; }
            /// Set the height of the widget
            void setHeight(int height) { mSize.y = height; }

            /**
             * \brief Set the fixed size of this widget
             *
             * If nonzero, components of the fixed size attribute override any values
             * computed by a layout generator associated with this widget. Note that
             * just setting the fixed size alone is not enough to actually change its
             * size; this is done with a call to \ref setSize or a call to \ref performLayout()
             * in the parent widget.
             */
            void setFixedSize(const glm::ivec2 &fixedSize) { mFixedSize = fixedSize; }

            /// Return the fixed size (see \ref setFixedSize())
            const glm::ivec2 &fixedSize() const { return mFixedSize; }

            // Return the fixed width (see \ref setFixedSize())
            int fixedWidth() const { return mFixedSize.x; }
            // Return the fixed height (see \ref setFixedSize())
            int fixedHeight() const { return mFixedSize.y; }
            /// Set the fixed width (see \ref setFixedSize())
            void setFixedWidth(int width) { mFixedSize.x = width; }
            /// Set the fixed height (see \ref setFixedSize())
            void setFixedHeight(int height) { mFixedSize.y = height; }

            /// Return whether or not the widget is currently visible (assuming all parents are visible)
            bool visible() const { return mVisible; }
            /// Set whether or not the widget is currently visible (assuming all parents are visible)
            void setVisible(bool visible) { mVisible = visible; }

            /// Check if this widget is currently visible, taking parent widgets into account
            bool visibleRecursive() const {
                bool visible = true;
                const UIWidget *widget = this;
                while (widget) {
                    visible &= widget->visible();
                    widget = widget->parent();
                }
                return visible;
            }

            /// Return the number of child widgets
            int childCount() const { return (int) mChildren.size(); }

            /// Return the list of child widgets of the current widget
            const std::vector<UIWidget *> &children() const { return mChildren; }

            /**
             * \brief Add a child widget to the current widget at
             * the specified index.
             *
             * This function almost never needs to be called by hand,
             * since the constructor of \ref UIWidget automatically
             * adds the current widget to its parent
             */
            virtual void addChild(int index, UIWidget *widget);

            /// Convenience function which appends a widget at the end
            void addChild(UIWidget *widget);

            /// Remove a child widget by index
            void removeChild(int index);

            /// Remove a child widget by value
            void removeChild(UIWidget *widget);

            /// Retrieves the child at the specific position
            const UIWidget* childAt(int index) const { return mChildren[index]; }

            /// Retrieves the child at the specific position
            UIWidget* childAt(int index) { return mChildren[index]; }

            /// Returns the index of a specific child or -1 if not found
            int childIndex(UIWidget* widget) const;

            /// Check if it widget is child of the current widget
            bool isChild(UIWidget *widget) const;

            /// Variadic shorthand notation to construct and add a child widget
            template<typename UIWidgetClass, typename... Args>
            UIWidgetClass* add(const Args&... args) {
                return new UIWidgetClass(this, args...);
            }

            /// Walk up the hierarchy and return the parent window
            UIWindow *window();

            /// Walk up the hierarchy and return the parent screen
            UIScreen *screen();

            /// Associate this widget with an ID value (optional)
            void setId(const std::string &id) { mId = id; }
            /// Return the ID value associated with this widget, if any
            const std::string &id() const { return mId; }

            /// Return whether or not this widget is currently enabled
            bool enabled() const { return mEnabled; }
            /// Set whether or not this widget is currently enabled
            void setEnabled(bool enabled) { mEnabled = enabled; }

            /// Return whether or not this widget is currently focused
            inline bool focused() const { return mFocused; }
            /// Set whether or not this widget is currently focused
            inline void setFocused(bool focused) { mFocused = focused; }
            /// Request the focus to be moved to this widget
            void requestFocus();

            const std::string &tooltip() const { return mTooltip; }

            void setTooltip(const std::string &tooltip);

            /// Return current font size. If not set the default of the current theme will be returned
            int fontSize() const;
            /// Set the font size of this widget
            void setFontSize(int fontSize) { mFontSize = fontSize; }
            /// Return whether the font size is explicitly specified for this widget
            bool hasFontSize() const { return mFontSize > 0; }

            /// Return a pointer to the cursor of the widget
            Cursor cursor() const { return mCursor; }
            /// Set the cursor of the widget
            void setCursor(Cursor cursor) { mCursor = cursor; }

            /// Check if the widget contains a certain position
            bool contains(const glm::ivec2 &aPos) const 
            {
                glm::ivec2 lDiff = aPos - mPos;
                bool lResult = true;
                for(auto i = 0; i < lDiff.length(); ++i)
                    lResult &= lDiff[i] >= 0 && lDiff[i] < mSize[i];
                return lResult;
            }

            /// Determine the widget located at the given position value (recursive)
            UIWidget *findWidget(const glm::ivec2 &p);

            /// Determine the widget located at the given name (recursive)
            UIWidget *findWidget(const std::string &aWidgetId);

            /// Handle a mouse button event (default implementation: propagate to children)
            virtual bool mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers);

            /// Handle a mouse motion event (default implementation: propagate to children)
            virtual bool mouseMotionEvent(const glm::ivec2 &p, const glm::ivec2 &rel, int button, int modifiers);

            /// Handle a mouse drag event (default implementation: do nothing)
            virtual bool mouseDragEvent(const glm::ivec2 &p, const glm::ivec2 &rel, int button, int modifiers);

            /// Handle a mouse enter/leave event (default implementation: record this fact, but do nothing)
            virtual bool mouseEnterEvent(const glm::ivec2 &p, bool enter);

            /// Handle a mouse scroll event (default implementation: propagate to children)
            virtual bool scrollEvent(const glm::ivec2 &p, const glm::vec2 &rel);

            /// Handle a focus change event (default implementation: record the focus status, but do nothing)
            virtual bool focusEvent(bool focused);

            ///Handle a mouse drag and drop event (default implementation: do nothing)
            virtual bool dragEvent(const glm::ivec2 &p, int button, int modifiers, UIDragDropData* aData);
            virtual bool dropEvent(const glm::ivec2 &p, int button, int modifiers, UIDragDropData* aData);

            /// Handle a keyboard event (default implementation: do nothing)
            virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);

            /// Handle text input (UTF-32 format) (default implementation: do nothing)
            virtual bool keyboardCharacterEvent(unsigned int codepoint);

            /// Compute the preferred size of the widget
            virtual glm::ivec2 preferredSize(NVGcontext *ctx) const;

            /// Invoke the associated layout generator to properly place child widgets, if any
            virtual void performLayout(NVGcontext *ctx);

            /// Draw the widget (and all child widgets)
            virtual void draw(NVGcontext *ctx);

            /// Save the state of the widget into the given \ref Serializer instance
            virtual void Serialize(Json::Value& aSerializer) const override;

            /// Restore the state of the widget from the given \ref Serializer instance
            virtual void Deserialize(const Json::Value& aSerializer) override;

        protected:
            UI*                     mOwner;
            UIWidget*               mParent;
            ref<UITheme>            mTheme;
            UILayout*               mLayout;
            std::string             mId;
            glm::ivec2              mPos, mSize, mFixedSize;
            std::vector<UIWidget*>  mChildren;
            bool                    mVisible;
            bool                    mEnabled;
            bool                    mFocused;
            bool                    mMouseFocus;
            std::string             mTooltip;
            int                     mFontSize;
            Cursor                  mCursor;

        //public:
        //    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };
    }
}
