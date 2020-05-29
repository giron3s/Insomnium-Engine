/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 * Brief	    : Defines a viewport for rendering. The viewport will be used to determine the
 *                clip-coordinates to window projection. It allows to have several viewports
 *                on the main window
*******************************************************************************/

#pragma once

#include <stdint.h>

namespace Framework
{
    class Viewport
    {
      public:
        /**
         * Constructors
         */
        Viewport(uint32_t aX, uint32_t aY, uint32_t aWidth, uint32_t aHeight)
          : mX(aX)
          , mY(aY)
          , mWidth(aWidth)
          , mHeight(aHeight)
        {

        }

        /**
         * Getters
         */
        uint32_t getX() const { return mX; }
        uint32_t getY() const { return mY; }
        uint32_t getWidth() const { return mWidth; }
        uint32_t getHeight() const { return mHeight; }

        /**
         * Setters
         */
        void GetX(uint32_t aX) { mX = aX; }
        void GetY(uint32_t aY) { mY = aY; }
        void GetWidth(uint32_t aWidth) { mWidth = aWidth; }
        void GetHeight(uint32_t aHeight) { mHeight = aHeight; }
      private:
        uint32_t mX;      /**< X origin of the viewport in the window */
        uint32_t mY;      /**< Y origin of the viewport in the window */
        uint32_t mWidth;  /**< Width of the viewport */
        uint32_t mHeight; /**< Height of the viewport */
    };
}
