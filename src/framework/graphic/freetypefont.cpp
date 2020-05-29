/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *
 * Brief        : Allows to load and render to a bitmap a true type font
 *                It also implements a cache of pre-renderered letters for
 *                later use. The cache is empty upon construction
 ********************************************************************************/
#include "precompiled.h"
#include "graphic/freetypefont.h"

#define FREETYPE_FONT_NUM_GLYPHS 128
#define FREETYPE_FONT_DPI 96

namespace Framework
{
    FreeTypeFont::FreeTypeFont()
    {}

    FreeTypeFont::~FreeTypeFont()
    {
        for(const auto& i : mFtCache)
            if(i.glyph)
                FT_Done_Glyph((FT_Glyph)i.glyph);
        if(mFtFace)
            FT_Done_Face(mFtFace);
        if(mFtLibrary)
            FT_Done_Library(mFtLibrary);
    }

    bool FreeTypeFont::Init(const string &aFontPath, uint32_t aSize)
    {
        ASSERT(!mFtLibrary); // must be called just once!
        FT_Error err;
        if ( (err = FT_Init_FreeType(&mFtLibrary)) != 0 )
        {
            WARNING("FT_Init_FreeType failed with error %d ", err);
            return false;
        }


        if ( (err = FT_New_Face(mFtLibrary, aFontPath.c_str(), 0, &mFtFace)) != 0 )
        {
            WARNING("FT_New_Face failed with error %d ", err);
            return false;
        }
        /* As explained in NeHe tutorial we need to multiply by 64
         * to achieve 1 unit pixel height as height is specified
         * as 1/64th of a pixel
         * 
         * The function sets the font's width and height parameters.
         * Setting the width to 0 lets the face dynamically calculate the width based on the given height.
         */
        if ( (err = FT_Set_Char_Size(mFtFace, 0, aSize << 6, FREETYPE_FONT_DPI, FREETYPE_FONT_DPI)) != 0 )
        {
            WARNING("FT_Set_Char_Size failed with error %d ", err);
            return false;
        }

        mSize = aSize;
        mFtCache.resize(FREETYPE_FONT_NUM_GLYPHS, {nullptr, 0});

        FT_Library_Version(mFtLibrary, &mFtVersionMajor, &mFtVersionMinor, &mFtVersionPatch);
        INFO(LogLevel::eLEVEL1, "Initialized the FT library correctly. VERSION: %d.%d.%d", mFtVersionMajor, mFtVersionMinor, mFtVersionPatch)
        INFO(LogLevel::eLEVEL1, "Load the '%s' TTF font correctly", aFontPath.c_str());
        return true;
    }

    const uint8_t *FreeTypeFont::GetBitmap(char aLetter, uint32_t &aWidth, uint32_t &aHeight, int32_t &aOffsetLeft, int32_t &aOffsetTop, uint32_t &aAdvance) const
    {
        ASSERT(mFtCache.size() == FREETYPE_FONT_NUM_GLYPHS);

        if ((size_t)aLetter > mFtCache.size())
            return nullptr;

        if ( !mFtCache[aLetter].glyph )
        { // Load and cache the glyph
            FT_Error err = 0;
            FT_Glyph glyph = nullptr;

            if ( (err = FT_Load_Glyph(mFtFace, FT_Get_Char_Index(mFtFace, aLetter), FT_LOAD_DEFAULT)) != 0 )
            {
                WARNING("FT_Load_Glyph for character %hu failed with error %d ", (uint16_t)aLetter, err);
                return nullptr;
            }

            if ( (err = FT_Get_Glyph(mFtFace->glyph, &glyph)) != 0 )
            {
                WARNING("FT_Get_Glyph for character %hu failed with error %d ", (uint16_t)aLetter, err);
                return nullptr;
            }


            if( (err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1)) != 0 )
            {
                WARNING("FT_Get_Glyph for character %hu failed with error %d ", (uint16_t)aLetter, err);
                return nullptr;
            }

            mFtCache[aLetter].glyph = (FT_BitmapGlyph)glyph;
            mFtCache[aLetter].advance = mFtFace->glyph->advance.x / 64;
            ASSERT(mFtCache[aLetter].glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY);
            ASSERT(mFtCache[aLetter].glyph->bitmap.num_grays == 256);
        }

        aWidth = mFtCache[aLetter].glyph->bitmap.width;
        aHeight = mFtCache[aLetter].glyph->bitmap.rows;
        aOffsetLeft = mFtCache[aLetter].glyph->left;
        aOffsetTop = mSize - mFtCache[aLetter].glyph->top;
        aAdvance = mFtCache[aLetter].advance;
        return mFtCache[aLetter].glyph->bitmap.buffer;
    }
}
