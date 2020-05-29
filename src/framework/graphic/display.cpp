/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#include "precompiled.h"

#include "engine.h"
#include "graphic/display.h"
#include "graphic/renderer.h"
#include "graphic/gui/canvasgl.h"
#include "graphic/gui/label.h"
#include "graphic/gui/layout.h"
#include "graphic/gui/popupbutton.h"
#include "graphic/gui/textbox.h"
#include "graphic/gui/imageview.h"
#include "graphic/gui/combobox.h"
#include "graphic/gui/checkbox.h"
#include "graphic/gui/widget.h"
#include "graphic/scene.h"


using namespace LuaIntf;

namespace Framework
{    
    Display::Display()
        : mWindowWidth( 0 )
        , mWindowHeight( 0 )
        , mWindowName("Undefined window name")
        , mResizable(false)
        , mFullScreen(false)
        , mVisualListDirty(false)
        , mRenderer(nullptr)
    {	 
    }

    Display::~Display()
    {

    }

    void Display::Initialize(const int aWindowWidth, const int aWindowHeight, bool aFullscreen, bool aResizable, const string& aWindowName)
    {
        mWindowWidth  = aWindowWidth;
        mWindowHeight = aWindowHeight;
        mWindowName   = aWindowName;
        mFullScreen   = aFullscreen;
        mResizable    = aResizable;

        /* Use our gui system */
        GUI::Initialize();
        initialize(glm::ivec2(aWindowWidth, aWindowHeight), aWindowName, aResizable, aFullscreen);

        /* Use our system renderer. This only supports OpenGL for now */
        mRenderer = Renderer::GetInstance();
        if (mRenderer == NULL || Renderer::GetInstance()->Init() == false)
        {
            CRASH("ERROR initializing the renderer");
            return;
        }

        mMouseMotionEventHandler = Engine::Instance()->Input().MouseMotionEvent.Connect([this](const InputEventType aEventType, const MouseMotionEventData& aMouseMotionEvent) { OnMouseMotionEvent(aEventType, aMouseMotionEvent); });
        mMouseButtonEventHandler = Engine::Instance()->Input().MouseButtonEvent.Connect([this](const InputEventType aEventType, const MouseButtonEventData& aMouseButtonEvent) { OnMouseButtonEvent(aEventType, aMouseButtonEvent); });
        mKeyboardEventHandler = Engine::Instance()->Input().KeyboardEvent.Connect([this](const InputEventType aEventType, const KeyboardEventData& aKeyboardEventData) { OnKeyboardEvent(aEventType, aKeyboardEventData); });
        mFileDropEvent = Engine::Instance()->Input().FileDropEvent.Connect([this](const InputEventType aEventType, const FileDropEventData& aFileDropEventData) { OnFileDropEvent(aEventType, aFileDropEventData); });
        mScrollEvent = Engine::Instance()->Input().ScrollEvent.Connect([this](const InputEventType aEventType, const ScrollEventData& aScrollEventData) { OnScrollEvent(aEventType, aScrollEventData); });
        mResizeEvent = Engine::Instance()->Input().ResizeEvent.Connect([this](const InputEventType aEventType, const ResizeEventData& aResizeEventData) { OnResizeEvent(aEventType, aResizeEventData); });
        mFocusEvent = Engine::Instance()->Input().FocusEvent.Connect([this](const InputEventType aEventType, const FocusEventData& aFocusEventData) { OnFocusEvent(aEventType, aFocusEventData); });
    
        LuaContext& lLuaContext = Engine::Instance()->Script().GetLuaContext();
        LuaState lLuaState = lLuaContext.state();
       

        LuaBinding(lLuaState).beginClass<Display>("Display")
            .addFunction("GetUIWidget",  &Display::GetUIWidget,  LUA_ARGS(string))
            .addFunction("GetUILabel",   &Display::GetUILabel,   LUA_ARGS(string))
            .addFunction("GetUITextBox", &Display::GetUITextBox, LUA_ARGS(string))
            .addFunction("GetUIButton", &Display::GetUIButton, LUA_ARGS(string))
            .addFunction("GetUIImageView", &Display::GetUIImageView, LUA_ARGS(string))
            .addFunction("GetUIComboBox", &Display::GetUIComboBox, LUA_ARGS(string))
            .addFunction("GetUICheckBox", &Display::GetUICheckBox, LUA_ARGS(string))
            .endClass();

        LuaBinding(lLuaState).beginClass<Framework::GUI::UILabel>("UILabel")
            .addFunction("SetCaption", &Framework::GUI::UILabel::setCaption, LUA_ARGS(string, bool))
            .addFunction("GetCaption", &Framework::GUI::UILabel::caption)
            .endClass();
    
        LuaBinding(lLuaState).beginClass<Framework::GUI::UITextBox>("UITextBox")
            .addFunction("SetText", &Framework::GUI::UITextBox::setValue, LUA_ARGS(string))
            .addFunction("GetText", &Framework::GUI::UITextBox::value)
            .endClass();

        LuaBinding(lLuaState).beginClass<Framework::GUI::UIImageView>("UIImageView")
            .addFunction("SetImage", &Framework::GUI::UIImageView::setImage, LUA_ARGS(int))
            .endClass();

        LuaBinding(lLuaState).beginClass<Framework::GUI::UIButton>("UIButton")
            .addFunction("SetCaption", &Framework::GUI::UIButton::setCaption, LUA_ARGS(string))
            .endClass();
			
        LuaBinding(lLuaState).beginClass<Framework::GUI::UIComboBox>("UIComboBox")
            .addFunction("GetSelectedValue", &Framework::GUI::UIComboBox::getSelectedValue)
            .addFunction("SetSelectedValue", &Framework::GUI::UIComboBox::setSelectedValue, LUA_ARGS(string))
            .endClass();

        LuaBinding(lLuaState).beginClass<Framework::GUI::UICheckBox>("UICheckBox")
            .addFunction("GetIsChecked", &Framework::GUI::UICheckBox::checked)
            .addFunction("SetIsChecked", &Framework::GUI::UICheckBox::setChecked, LUA_ARGS(bool))
            .endClass();
    }

    void Display::DeInitialize()
    {
        Engine::Instance()->Input().MouseMotionEvent.Disconnect( mMouseMotionEventHandler );
        Engine::Instance()->Input().MouseButtonEvent.Disconnect( mMouseButtonEventHandler );
        Engine::Instance()->Input().KeyboardEvent.Disconnect( mKeyboardEventHandler );
        Engine::Instance()->Input().FileDropEvent.Disconnect( mFileDropEvent );
        Engine::Instance()->Input().ScrollEvent.Disconnect( mScrollEvent );
        Engine::Instance()->Input().ResizeEvent.Disconnect( mResizeEvent );
        Engine::Instance()->Input().FocusEvent.Disconnect(mFocusEvent);

        setVisible(false);
        for (auto lUIIter : mActiveUIs)
        {
            lUIIter->DeInitialize();
        }
        GUI::Shutdown();
    }

    void Display::Update()
    {
        PROFILER_EVENT();

        while (!mUIToDeactivate.empty())
        {
            shared_ptr<UI> lUI = mUIToDeactivate.back();
            mUIToDeactivate.pop_back();

            lUI->DeInitialize();
            ASSERT(lUI.unique());
        }

        while (!mUIToActivate.empty())
        {
            shared_ptr<UI> lUI = mUIToActivate.back();
            mUIToActivate.pop_back();

            lUI->Initialize();
            mActiveUIs.push_back(lUI);
        }

        GUI::Mainloop();
    }

    void Display::OnMouseMotionEvent(const InputEventType aEventType, const MouseMotionEventData& aMouseMotionEvent)
    {
        if (!this->mProcessEvents)
            return;
        this->cursorPosCallbackEvent(aMouseMotionEvent.x, aMouseMotionEvent.y);
    }

    void Display::OnMouseButtonEvent(const InputEventType aEventType, const MouseButtonEventData& aMouseButtonEvent)
    {
        if (!this->mProcessEvents)
            return;
        this->mouseButtonCallbackEvent(aMouseButtonEvent.button, aMouseButtonEvent.action, aMouseButtonEvent.modifiers);
    }

    void Display::OnKeyboardEvent(const InputEventType aEventType, const KeyboardEventData& aKeyboardEventData)
    {
        if (!this->mProcessEvents)
            return;
        this->keyCallbackEvent(aKeyboardEventData.keyboardKey, aKeyboardEventData.scancode, aKeyboardEventData.action, aKeyboardEventData.modifiers);
    }

    void Display::OnFileDropEvent(const InputEventType aEventType, const FileDropEventData& aFileDropEventData)
    {
        if (!this->mProcessEvents)
            return;
        this->dropCallbackEvent(aFileDropEventData.count, aFileDropEventData.filenames);
    }

    void Display::OnScrollEvent(const InputEventType aEventType, const ScrollEventData& aScrollEventData)
    {
        if (!this->mProcessEvents)
            return;
        this->scrollCallbackEvent(aScrollEventData.xoffset, aScrollEventData.yoffset);
    }

    void Display::OnResizeEvent(const InputEventType aEventType, const ResizeEventData& aResizeEventData)
    {
        if (!this->mProcessEvents)
            return;

        mWindowWidth = aResizeEventData.width;
        mWindowHeight = aResizeEventData.height;
        this->resizeCallbackEvent(aResizeEventData.width, aResizeEventData.height);
    }

    void Display::OnFocusEvent(const InputEventType aEventType, const FocusEventData& aFocusEventData)
    {
        if (!this->mProcessEvents)
            return;
        this->focusEvent(aFocusEventData.focused != 0);
    }
    
    shared_ptr<const UI> Display::FindUI(const string& aFileName, const uiContainerType& aUIContainter)
    {
        auto it = find_if(aUIContainter.begin(), aUIContainter.end(), [aFileName](shared_ptr<UI> other) { return aFileName == other->GetFileName(); });
        if (it == aUIContainter.end())
            return shared_ptr<const UI>();
        return *it;
    }

    bool Display::IsLoadedUI(const string& aName)
    {
        if (FindUI(aName, mUIToActivate) || FindUI(aName, mUIToDeactivate) || FindUI(aName, mActiveUIs))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void Display::LoadUI(const string& aUIName)
    {
        //Find the UI at the activated UI list
        if(FindUI(aUIName, mActiveUIs))
        {
            WARNING("UI %s was already loaded\n", aUIName.c_str());
            return;
        }
        //Find the UI at the ToActivated list
        else if(FindUI(aUIName, mUIToActivate))
        {
            WARNING("UI %s is already on the mUIToActivate list\n", aUIName.c_str());
            return;
        }
        //Find the UI at the ToDectivated list - >  UI at the ToDectivated list and add to the ActivateUI list
        else if(FindUI(aUIName, mUIToDeactivate))
        {
            vector<shared_ptr<UI>>::iterator lIter = mUIToDeactivate.begin();
            while (lIter != mUIToDeactivate.end())
            {
                if (aUIName.compare((*lIter)->GetFileName()) != 0)
                {
                    lIter++;
                }
                else
                {
                    mActiveUIs.push_back(*lIter);
                    lIter = mUIToDeactivate.erase(lIter);
                }
            }
        }
        //Otherwise load the UI
        else
        {
            LoadUIFromFile(aUIName);
        }
    }


    void Display::UnloadUI(shared_ptr<const UI> aUI)
    {
        for (auto it = mActiveUIs.begin(); it != mActiveUIs.end(); ++it)
        {
            if (*it == aUI)
            {
                mUIToDeactivate.push_back(*it);
                mActiveUIs.erase(it);
                break;
            }
        }
    }

    void Display::UnloadUI(const string& aUIName)
    {
        shared_ptr<const UI> lUI = FindUI(aUIName, mActiveUIs);
        if (!lUI)
        {
            WARNING("Scene %s is not active and cannot be unloaded\n", aUIName.c_str());
            return;
        }
        UnloadUI(lUI);
    }

    UITextBox& Display::GetUITextBox(const string& aWidgetId)
    {
        UIWidget& lUIWidget = GetUIWidget(aWidgetId);
        return dynamic_cast<UITextBox&>(lUIWidget);
    }

    UIImageView& Display::GetUIImageView(const string& aWidgetId)
    {
        UIWidget& lUIWidget = GetUIWidget(aWidgetId);
        return dynamic_cast<UIImageView&>(lUIWidget);
    }

    UILabel& Display::GetUILabel(const string& aWidgetId)
    {
        UIWidget& lUIWidget = GetUIWidget(aWidgetId);
        return dynamic_cast<UILabel&>(lUIWidget);
    }

    UIButton& Display::GetUIButton(const string& aWidgetId)
    {
        UIWidget& lUIWidget = GetUIWidget(aWidgetId);
        return dynamic_cast<UIButton&>(lUIWidget);
	}

    UIComboBox& Display::GetUIComboBox(const string& aWidgetId)
    {
        UIWidget& lUIWidget = GetUIWidget(aWidgetId);
        return dynamic_cast<UIComboBox&>(lUIWidget);
    }

    UICheckBox& Display::GetUICheckBox(const string& aWidgetId)
    {
        UIWidget& lUIWidget = GetUIWidget(aWidgetId);
        return dynamic_cast<UICheckBox&>(lUIWidget);
    }

    UIWidget& Display::GetUIWidget(const string& aWidgetId)
    {
        UIWidget* lUIWidget = Engine::Instance()->Display().findWidget(aWidgetId);
        ASSERT(lUIWidget)

        if (lUIWidget == nullptr)
            WARNING("Widget named %s does not exist", aWidgetId.c_str());

        return *lUIWidget;
    }

    void Display::LoadUIFromFile(const string& aFileName)
    {
        Json::Value lUIRes;
        Json::JsonUtils::OpenAndParseJsonFromFile(lUIRes, aFileName);

        shared_ptr<UI> lUI(new Framework::UI(aFileName));
        lUI->Deserialize(lUIRes);
        mUIToActivate.push_back(lUI);
    }
};
