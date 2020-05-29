/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once

#include <map>
#include "core/types.h"
#include "core/utils.h"
#include "core/signal.h"

namespace Framework
{

    class InputManager
    {
        NON_COPYABLE_CLASS( InputManager );
    public:
                InputManager();
                ~InputManager();


        void    HandleInputEvent( const EventData& aInputEvent );

        bool    IsPressed(int32_t aKeycode) const;
        bool    IsReleased(int32_t aKeycode) const;
        void    RegisterKey(int32_t aKeycode);

        Signal<InputEventType, MouseMotionEventData> MouseMotionEvent;
        Signal<InputEventType, MouseButtonEventData> MouseButtonEvent;
        Signal<InputEventType, KeyboardEventData> KeyboardEvent;
        Signal<InputEventType, FileDropEventData> FileDropEvent;
        Signal<InputEventType, ScrollEventData> ScrollEvent;
        Signal<InputEventType, ResizeEventData> ResizeEvent;
        Signal<InputEventType, FocusEventData> FocusEvent;

    protected:
        std::map<int,bool> mKeysHeld;
        glm::ivec2         mMousePos;
        glm::ivec2         mPrevMousePos;
    };
}
