/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

#include <vector>
#include <string.h>
#include "core/utils.h"
#include "core/types.h"
#include "core/signal.h"

#include "engine/ui.h"
#include "graphic/renderer.h"
#include "graphic/gui/screen.h"

namespace Framework
{
    class Display : public GUI::UIScreen
    {
            NON_COPYABLE_CLASS( Display );

        public:

            enum class RenderLayer : int
            {
                Layer_0 = 0,
                Layer_1,
                Layer_2,
                Layer_4,
                Layer_6,
                Layer_7,
                Layer_8,
                Layer_9,
                Layer_Max
            };

                                                Display();
            virtual                            ~Display();

            void                               Initialize( const int aWindowWidth, const int aWindowHeight, bool aFullscreen, bool aResizable, const std::string& aWindowName);
            void                               DeInitialize();
            void                               Update();

            void                               LoadUI(const std::string& aUIFileName);
            void                               UnloadUI(const std::string& aUIFileName);

            int                                GetScreenWidth() const { return mWindowWidth; };
            int                                GetScreenHeight() const { return mWindowHeight; };

            Renderer*                          GetRenderer() { return mRenderer; }

        private:
            using uiContainerType = std::vector<std::shared_ptr<Framework::UI> >;
            using iteratorType = uiContainerType::iterator;

            void                               LoadUIFromFile(const std::string& fileName);
            void                               UnloadUI(std::shared_ptr<const UI> aUI);
            static std::shared_ptr<const UI>   FindUI(const std::string& aFileName, const uiContainerType& aUIContainer);
            bool                               IsLoadedUI(const std::string& name );

            void                               OnMouseMotionEvent(const InputEventType aEventType, const MouseMotionEventData& aMouseMotionEvent);
            void                               OnMouseButtonEvent(const InputEventType aEventType, const MouseButtonEventData& aMouseButtonEvent);
            void                               OnKeyboardEvent(const InputEventType aEventType, const KeyboardEventData& aKeyboardEventData);
            void                               OnFileDropEvent(const InputEventType aEventType, const FileDropEventData& aFileDropEventData);
            void                               OnScrollEvent(const InputEventType aEventType, const ScrollEventData& aScrollEventData);
            void                               OnResizeEvent(const InputEventType aEventType, const ResizeEventData& aResizeEventData);
            void                               OnFocusEvent(const InputEventType aEventType, const FocusEventData& aFocusEventData);

            Framework::GUI::UIWidget&          GetUIWidget(const string& aWidgetId);
            Framework::GUI::UITextBox&         GetUITextBox(const string& aWidgetId);
            Framework::GUI::UILabel&           GetUILabel(const string& aWidgetId);
            Framework::GUI::UIButton&          GetUIButton(const string& aWidgetId);
            Framework::GUI::UIImageView&       GetUIImageView(const string& aWidgetId);
            Framework::GUI::UIComboBox&        GetUIComboBox(const string& aWidgetId);
            Framework::GUI::UICheckBox&        GetUICheckBox(const string& aWidgetId);

        protected:
            int                                mWindowWidth;
            int                                mWindowHeight;
            std::string                        mWindowName;
            bool                               mResizable;
            bool                               mFullScreen;

            bool                               mVisualListDirty;
            std::vector<std::shared_ptr<UI> >  mUIToActivate;
            std::vector<std::shared_ptr<UI> >  mActiveUIs;
            std::vector<std::shared_ptr<UI> >  mUIToDeactivate;

            EventHandler                       mMouseMotionEventHandler;
            EventHandler                       mMouseButtonEventHandler;
            EventHandler                       mKeyboardEventHandler;
            EventHandler                       mFileDropEvent;
            EventHandler                       mScrollEvent;
            EventHandler                       mResizeEvent;
            EventHandler                       mFocusEvent;

            Renderer*                          mRenderer;
    };
}
