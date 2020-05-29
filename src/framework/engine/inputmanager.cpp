/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "engine/inputmanager.h"

namespace Framework
{
    InputManager::InputManager()
    {
        RegisterKey(GLFW_KEY_A);
        RegisterKey(GLFW_KEY_S);
        RegisterKey(GLFW_KEY_D);
        RegisterKey(GLFW_KEY_W);
        RegisterKey(GLFW_KEY_LEFT_ALT);
        RegisterKey(GLFW_KEY_RIGHT_ALT);
        RegisterKey(GLFW_KEY_LEFT_CONTROL);
        RegisterKey(GLFW_KEY_RIGHT_CONTROL);

        mMousePos = glm::ivec2(0);
        mPrevMousePos = glm::ivec2(0);
    }

    InputManager::~InputManager()
    {
    }


    void InputManager::HandleInputEvent( const EventData& aInputEvent )
    {
        const InputEventType lType = static_cast<InputEventType>( aInputEvent.type );

        if( lType == InputEventType::KeyboardEvent )
        {
            map<int, bool>::iterator lFind = mKeysHeld.find(aInputEvent.data.key.keyboardKey); // Find the key in the map
            if (lFind != mKeysHeld.end())
            {
                (aInputEvent.data.key.action == GLFW_PRESS || aInputEvent.data.key.action == GLFW_REPEAT ) ? lFind->second = true : lFind->second = false;
            }
            KeyboardEvent.Emit( lType, aInputEvent.data.key );
        }
        if ( lType == InputEventType::MouseMotionEvent )
        {
            mPrevMousePos = mMousePos;
            mMousePos.x = aInputEvent.data.motion.x;
            mMousePos.y = aInputEvent.data.motion.y;

            MouseMotionEventData lMotionData = aInputEvent.data.motion;
            lMotionData.dx = mMousePos.x - mPrevMousePos.x;
            lMotionData.dy = mMousePos.y - mPrevMousePos.y;
            MouseMotionEvent.Emit( lType, lMotionData );
        }
        if ( lType == InputEventType::MouseButtonEvent)
        {
            MouseButtonEvent.Emit( lType, aInputEvent.data.button );
        }
        if( lType == InputEventType::FileDropEvent )
        {
            FileDropEvent.Emit(lType, aInputEvent.data.filedrop);
        }
        if( lType == InputEventType::ScrollEvent)
        {
            ScrollEvent.Emit(lType, aInputEvent.data.scrool);
        }
        if (lType == InputEventType::ResizeEvent)
        {
            ResizeEvent.Emit(lType, aInputEvent.data.resize);
        }
        if (lType == InputEventType::FocusEvent)
        {
            FocusEvent.Emit(lType, aInputEvent.data.focus);
        }
    }

    void InputManager::RegisterKey(int32_t aKeycode)
    {
        std::pair<int, bool> lKey(aKeycode, false);
        std::pair<std::map<int, bool>::iterator, bool> lNewlyAddedPair = mKeysHeld.insert(lKey);

        if(lNewlyAddedPair.second == false)
        {
            CRASH("Key '%s' is already registered and can't be registered again!", std::to_string(aKeycode).c_str());
        }
    }

    bool InputManager::IsPressed(int32_t aKeycode) const
    {
        map<int, bool>::const_iterator lFind = mKeysHeld.find(aKeycode);
        if(lFind == mKeysHeld.end())
        {
            WARNING("Key not found! Please register before using it");
            return false;
        }
        return lFind->second;
    }

    bool InputManager::IsReleased(int32_t aKeycode) const
    {
        return !IsPressed(aKeycode);
    }
}
