/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once
#include "stdint.h"

namespace Framework
{
    enum class InputEventType
    {
        MouseMotionEvent,
        MouseButtonEvent,
        KeyboardEvent,
        ResizeEvent,
        FileDropEvent,
        ScrollEvent,
        FocusEvent,
        UnhandledEvent
    };

    /**
     *  \brief Mouse motion event structure (event.motion.*)
     */
    struct MouseMotionEventData
    {
        uint32_t type;        /**< MouseMotion */
        int32_t  x;           /**< X coordinate, relative to window */
        int32_t  y;           /**< Y coordinate, relative to window */
        int32_t  dx;          /**< Delta X coordinate, relative to window */
        int32_t  dy;          /**< Delta Y coordinate, relative to window */
    };

    /**
     *  \brief Mouse button event structure (event.button.*)
     */
    struct MouseButtonEventData
    {
        uint32_t type;        /**< MouseButtonEvent */
        int32_t  button;      /**< The mouse button index */
        int32_t  action;      /**< GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT. */
        int32_t  modifiers;   /**<  Bit field describing which modifier keys were held down. (GLFW_MOD_SHIFT /GLFW_MOD_CONTROL /GLFW_MOD_ALT/ GLFW_MOD_SUPER ) */
    };


    /**
     *  \brief Keyboard button event structure (event.key.*)
     */
    struct KeyboardEventData
    {
        uint32_t type;        /**< KeyboardEvent */
        int32_t  keyboardKey; /**< The keyboard key that was pressed or released. */
        int32_t  scancode;    /**< The system-specific scancode of the key. */
        int32_t  action;      /**< GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT. */
        int32_t  modifiers;   /**<  Bit field describing which modifier keys were held down. (GLFW_MOD_SHIFT /GLFW_MOD_CONTROL /GLFW_MOD_ALT/ GLFW_MOD_SUPER ) */
    };

    /**
     *  \brief Resize window event structure
     */
    struct ResizeEventData
    {
        uint32_t type;       /**< ResizeEvent */
        int32_t  width;      /**< Resized window width */
        int32_t  height;     /**< Resized window height */
    };

    /**
     *  \brief File Drop event structure
     */
    struct FileDropEventData
    {
        uint32_t   type;          /**< ResizeEvent */
        int32_t    count;         /**< The number of dropped files. */
        const char **filenames; /**< The UTF-8 encoded file and/or directory path names. */
    };


    /**
     *  \brief Scrool event structure
     */
    struct ScrollEventData
    {
        uint32_t type;          /**< ResizeEvent */
        int32_t  xoffset;       /**< The scroll offset along the x-axis */
        int32_t  yoffset;       /**< The scroll offset along the y-axis */
    };

    /**
     *  \brief Focus event structure
     */
    struct FocusEventData
    {
        bool focused;          /**< Focused: 0 when false, 1 when true */
    };


    struct EventData
    {
        uint32_t             type;      /**< Event type, shared with all events */
        union
        {
            MouseMotionEventData motion;    /**< Mouse motion event data */
            MouseButtonEventData button;    /**< Mouse button event data */
            KeyboardEventData    key;       /**< Keyboard event data */
            ResizeEventData      resize;    /**< Resize event data */
            FileDropEventData    filedrop;  /**< Filedrop event data */
            ScrollEventData      scrool;    /**< Scrool event data */
            FocusEventData       focus;     /**< Focus event data */
        } data;
    };
}
