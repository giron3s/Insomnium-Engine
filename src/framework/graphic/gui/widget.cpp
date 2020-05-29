/*
    src/widget.cpp -- Base class of all widgets

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
#include "precompiled.h"
#include "engine.h"
#include "json/json.h"
#include "engine.h"
#include "graphic/gui/cursor.h"
#include "graphic/gui/widget.h"
#include "graphic/gui/layout.h"
#include "graphic/gui/theme.h"
#include "graphic/gui/window.h"
#include "graphic/gui/screen.h"
#include "graphic/gui/dragdropdata.h"
#include "graphic/gui/textbox.h"

namespace Framework
{
    namespace GUI
    {
        UIWidget::UIWidget()
            : mOwner(nullptr), mParent(nullptr), mTheme(nullptr), mLayout(nullptr),
            mPos(glm::ivec2(0)), mSize(glm::ivec2(0)),
            mFixedSize(glm::ivec2(0)), mVisible(true), mEnabled(true),
            mFocused(false), mMouseFocus(false), mTooltip(""), mFontSize(-1.0f),
            mCursor(Cursor::eArrow)
        {

        }

        UIWidget::UIWidget(UIWidget *parent)
            : mOwner(nullptr), mParent(nullptr), mTheme(nullptr), mLayout(nullptr),
              mPos(glm::ivec2(0)), mSize(glm::ivec2(0)),
              mFixedSize(glm::ivec2(0)), mVisible(true), mEnabled(true),
              mFocused(false), mMouseFocus(false), mTooltip(""), mFontSize(-1.0f),
              mCursor(Cursor::eArrow)
        {
            if (parent)
                parent->addChild(this);
        }

        UIWidget::~UIWidget() 
        {
        }

        void UIWidget::destroyUIObject()
        {
            for (auto child : mChildren) {
                if (child)
                    child->decRef();
            }

            if(mOwner) mOwner->DestroyUIWidget(shared_from_this());
        }

        void UIWidget::setTheme(UITheme *theme)
        {
            //if (mTheme.get() == theme)
            //    return;
            mTheme = theme;
            for (auto child : mChildren)
                child->setTheme(theme);
        }

        int UIWidget::fontSize() const 
        {
            return (mFontSize < 0 && mTheme) ? mTheme->mStandardFontSize : mFontSize;
        }

        glm::ivec2 UIWidget::preferredSize(NVGcontext *ctx) const {
            if (mLayout)
                return mLayout->preferredSize(ctx, this);
            else
                return mSize;
        }

        void UIWidget::performLayout(NVGcontext *ctx) {
            if (mLayout) {
                mLayout->performLayout(ctx, this);
            } else {
                for (auto c : mChildren) {
                    glm::ivec2 pref = c->preferredSize(ctx), fix = c->fixedSize();
                    c->setSize(glm::ivec2(
                        fix[0] ? fix[0] : pref[0],
                        fix[1] ? fix[1] : pref[1]
                    ));
                    c->performLayout(ctx);
                }
            }
        }

        UIWidget *UIWidget::findWidget(const glm::ivec2 &p) {
            for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) {
                UIWidget *child = *it;
                if (child->visible() && child->contains(p - mPos))
                    return child->findWidget(p - mPos);
            }
            return contains(p) ? this : nullptr;
        }

        UIWidget *UIWidget::findWidget(const std::string &aWidgetId)
        {
            if (mId.compare(aWidgetId) == 0)
                return this;

            for (auto lIter = mChildren.rbegin(); lIter != mChildren.rend(); ++lIter)
            {
                UIWidget *lChild = *lIter;
                if (lChild->mId.compare(aWidgetId) == 0)
                    return lChild;
                else
                {
                    UIWidget* lFound = lChild->findWidget(aWidgetId);;
                    if (lFound)
                        return lFound;
                }
            }
            return nullptr;
        }

        bool UIWidget::mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers) {
            for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) {
                UIWidget *child = *it;
                if (child->visible() && child->contains(p - mPos) &&
                    child->mouseButtonEvent(p - mPos, button, down, modifiers))
                    return true;
            }
            if (button == GLFW_MOUSE_BUTTON_1 && down && !mFocused)
                requestFocus();
            return false;
        }

        bool UIWidget::mouseMotionEvent(const glm::ivec2 &p, const glm::ivec2 &rel, int button, int modifiers) {
            for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) {
                UIWidget *child = *it;
                if (!child->visible())
                    continue;
                bool contained = child->contains(p - mPos), prevContained = child->contains(p - mPos - rel);
                if (contained != prevContained)
                    child->mouseEnterEvent(p, contained);
                if ((contained || prevContained) &&
                    child->mouseMotionEvent(p - mPos, rel, button, modifiers))
                    return true;
            }
            return false;
        }

        bool UIWidget::scrollEvent(const glm::ivec2 &p, const glm::vec2 &rel) {
            for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) {
                UIWidget *child = *it;
                if (!child->visible())
                    continue;
                if (child->contains(p - mPos) && child->scrollEvent(p - mPos, rel))
                    return true;
            }
            return false;
        }

        bool UIWidget::mouseDragEvent(const glm::ivec2 &, const glm::ivec2 &, int, int) {
            return false;
        }

        bool UIWidget::mouseEnterEvent(const glm::ivec2 &, bool enter) {
            mMouseFocus = enter;
            return false;
        }

        bool UIWidget::focusEvent(bool focused) {
            mFocused = focused;
            return false;
        }

        bool UIWidget::dragEvent(const glm::ivec2 &p, int button, int modifiers, UIDragDropData* aData)
        {
            return false;
        }

        bool UIWidget::dropEvent(const glm::ivec2 &p, int button, int modifiers, UIDragDropData* aData)
        {
            return false;
        }

        bool UIWidget::keyboardEvent(int, int, int, int) {
            return false;
        }

        bool UIWidget::keyboardCharacterEvent(unsigned int) {
            return false;
        }

        void UIWidget::addChild(int index, UIWidget * widget) {
            assert(index <= childCount());
            mChildren.insert(mChildren.begin() + index, widget);
            widget->incRef();
            widget->setParent(this);
            widget->setTheme(mTheme);
        }

        void UIWidget::addChild(UIWidget * widget) {
            addChild(childCount(), widget);
        }

        void UIWidget::removeChild(UIWidget *widget) {
            mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), widget), mChildren.end());
            widget->decRef();
        }

        void UIWidget::removeChild(int index) {
            UIWidget *widget = mChildren[index];
            mChildren.erase(mChildren.begin() + index);
            widget->decRef();
        }

        int UIWidget::childIndex(UIWidget *widget) const {
            auto it = std::find(mChildren.begin(), mChildren.end(), widget);
            if (it == mChildren.end())
                return -1;
            return (int) (it - mChildren.begin());
        }

        bool UIWidget::isChild(UIWidget *widget) const
        {
            for (auto lChildren : mChildren)
            {
                if (lChildren == widget)
                    return true;
                else
                    lChildren->isChild(widget);
            }
            return false;
        }


        UIWindow *UIWidget::window() {
            UIWidget *widget = this;
            while (true) {
                if (!widget)
                    throw std::runtime_error(
                        "UIWidget:internal error (could not find parent window)");
                UIWindow *window = dynamic_cast<UIWindow *>(widget);
                if (window)
                    return window;
                widget = widget->parent();
            }
        }

        UIScreen *UIWidget::screen() {
            UIWidget *widget = this;
            while (true) {
                if (!widget)
                    throw std::runtime_error(
                        "UIWidget:internal error (could not find parent screen)");
                UIScreen *screen = dynamic_cast<UIScreen *>(widget);
                if (screen)
                    return screen;
                widget = widget->parent();
            }
        }

        void UIWidget::requestFocus() {
            UIWidget *widget = this;
            while (widget->parent())
                widget = widget->parent();
            ((UIScreen *) widget)->updateFocus(this);
        }

        void UIWidget::draw(NVGcontext *ctx) 
        {
            //DEBUG TOOL - Render the WIDGET BOUNDS
            #if NANOGUI_SHOW_WIDGET_BOUNDS
            {
                nvgStrokeWidth(ctx, 3.0f);
                nvgBeginPath(ctx);
                nvgRect(ctx, mPos.x - 0.5f, mPos.y - 0.5f, mSize.x + 1, mSize.y + 1);
                nvgStrokeColor(ctx, nvgRGBA(255, 0, 0, 255));
                nvgStroke(ctx);
            }
            #endif

            if (mChildren.empty())
                return;

            nvgSave(ctx);
            nvgTranslate(ctx, mPos.x, mPos.y);
            for (auto child : mChildren) {
                if (child->visible()) {
                    nvgSave(ctx);
                    nvgIntersectScissor(ctx, child->mPos.x, child->mPos.y, child->mSize.x, child->mSize.y);
                    child->draw(ctx);
                    nvgRestore(ctx);
                }
            }
            nvgRestore(ctx);
        }

        void UIWidget::Serialize(Json::Value& aSerializer) const
        {
        //    s.set("position", mPos);
        //    s.set("size", mSize);
        //    s.set("fixedSize", mFixedSize);
        //    s.set("visible", mVisible);
        //    s.set("enabled", mEnabled);
        //    s.set("focused", mFocused);
        //    s.set("tooltip", mTooltip);
        //    s.set("fontSize", mFontSize);
        //    s.set("cursor", (int) mCursor);
        }
        
        void UIWidget::Deserialize(const Json::Value& aSerializer)
        {
            if (!aSerializer.isMember("name"))
            {
                CRASH("Not defined the 'name' attribute in: \n %s", aSerializer.toStyledString().c_str());
                return;
            }

            if ( (aSerializer.isMember("fixedWidth") || aSerializer.isMember("fixedHeight") ) && aSerializer.isMember("fixedSize") )
            {
                CRASH("Can not define the 'fixedWidth' or 'fixedHeight' with the 'fixedSize' attribute.");
                return;
            }

            mId = aSerializer.get("name", "undefined").asString();
            mVisible = aSerializer.get("visible", mVisible).asBool();
            mEnabled = aSerializer.get("enabled", mEnabled).asBool();
            mFocused = aSerializer.get("focused", mFocused).asBool();

            string lManagerName = Utils::Demangling(typeid(LanguageManager).name());
            LanguageManager& lLanguageManager = static_cast<LanguageManager&>(Engine::Instance()->Manager(lManagerName));
            mTooltip = lLanguageManager.GetString(aSerializer.get("tooltip", mTooltip).asString());

            mFontSize = aSerializer.get("fontSize", mFontSize).asInt();
            string lCursorStr = aSerializer.get("cursor", CursorToString(mCursor)).asString();
            mCursor = StringToCursor(lCursorStr.c_str());

            if (aSerializer.isMember("position"))
            {
                mPos.x = aSerializer["position"][0].asInt();
                mPos.y = aSerializer["position"][1].asInt();
            }

            if (aSerializer.isMember("size"))
            {
                mSize.x = aSerializer["size"][0].asInt();
                mSize.y = aSerializer["size"][1].asInt();
            }

            if (aSerializer.isMember("fixedSize"))
            {
                mFixedSize.x = aSerializer["fixedSize"][0].asInt();
                mFixedSize.y = aSerializer["fixedSize"][1].asInt();
            }
            if (aSerializer.isMember("fixedWidth"))
                mFixedSize.x = aSerializer.get("fixedWidth", 1).asInt();
            if (aSerializer.isMember("fixedHeight"))
                mFixedSize.y = aSerializer.get("fixedHeight", 1).asInt();


            if( aSerializer.isMember("layout"))
            {
                Json::Value lLayoutSerializer = aSerializer["layout"];
                std::string lUILayoutType = lLayoutSerializer["type"].asString();
                if(lUILayoutType.compare("BoxLayout") == 0)
                    mLayout = new UIBoxLayout();
                else if(lUILayoutType.compare("GroupLayout") == 0)
                    mLayout = new UIGroupLayout();
                else if(lUILayoutType.compare("GridLayout") == 0)
                    mLayout = new UIGridLayout();
                else if(lUILayoutType.compare("AdvancedGridLayout") == 0)
                    mLayout = new UIAdvancedGridLayout();
                else
                    CRASH("Invalid layout format");
                mLayout->Deserialize(lLayoutSerializer);
            }
        }

        void UIWidget::setTooltip(const std::string &tooltip)
        {
            string lManagerName = Utils::Demangling(typeid(LanguageManager).name());
            LanguageManager& lLanguageManager = static_cast<LanguageManager&>(Engine::Instance()->Manager(lManagerName));
            mTooltip = lLanguageManager.GetString(tooltip);
        }
    }
}
