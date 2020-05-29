/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 * Brief        : Allows to load and render to a bitmap a true type font
 *                It also implements a cache of pre-renderered letters for
 *                later use. The cache is empty upon construction
 *******************************************************************************/
#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include "graphic/truetypefont.h"

// FreeType Headers
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_TRIGONOMETRY_H
#include FT_MODULE_H

namespace Framework
{
    class FreeTypeFont : public TrueTypeFont
    {
      public:
        FreeTypeFont();
        ~FreeTypeFont() override;

        bool Init(const std::string &aFontPath, uint32_t aSize) override;
        const uint8_t* GetBitmap(char aLetter, uint32_t &aWidth, uint32_t &aHeight, int32_t &aOffsetLeft, int32_t &aOffsetTop, uint32_t &aAdvance) const;

      private:
        FT_Library                  mFtLibrary = nullptr;
        FT_Int                      mFtVersionMajor = 0;
        FT_Int                      mFtVersionMinor = 0;
        FT_Int                      mFtVersionPatch = 0;
        FT_Face                     mFtFace = nullptr;

        uint32_t                    mSize = 0;

        struct CacheEntry
        {
            FT_BitmapGlyph glyph;
            uint32_t       advance;
        };
        mutable std::vector<CacheEntry> mFtCache;
    };
}
