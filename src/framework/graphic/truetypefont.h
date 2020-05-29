/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Allows to load and render to a bitmap a true type font
 *                It also implements a cache of pre-renderered letters for later use. The cache is empty upon construction
 ********************************************************************************/

#pragma once

#include <stdint.h>
#include <string>

namespace Framework
{
    class TrueTypeFont
    {
      public:
        static TrueTypeFont *New();
        static void Delete(TrueTypeFont *aFont);

        virtual ~TrueTypeFont() = default;
        virtual bool Init(const std::string &aFontPath, uint32_t aSize) = 0;
        virtual const uint8_t *GetBitmap(char aLetter, uint32_t &aWidth, uint32_t &aHeight, int32_t &aOffsetLeft, int32_t &aOffsetTop, uint32_t &aAdvance) const = 0;
    };
}
