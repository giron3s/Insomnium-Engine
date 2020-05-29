/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Holds the texture data
 *******************************************************************************/

#pragma once

#include <stdint.h>
#include <string>
#include <vector>

namespace Framework
{
    struct Texture
    {
        std::string mName;
        uint32_t    mWidth = 0;
        uint32_t    mHeight = 0;
        uint32_t    mBytesPerPixel = 0;
        uint32_t    mType = 0;
        uint32_t    mFormat = 0;
        std::vector<uint8_t> mPixels;

        Texture() = default;

        Texture(const Texture&)  = delete;

        Texture(Texture&& aTempTexture)
            : mName(std::move(aTempTexture.mName))
            , mWidth(aTempTexture.mWidth)
            , mHeight(aTempTexture.mHeight)
            , mBytesPerPixel(aTempTexture.mBytesPerPixel)
            , mType(aTempTexture.mType)
            , mFormat(aTempTexture.mFormat)
            , mPixels(std::move(aTempTexture.mPixels))

        {
            aTempTexture.Clear();
        }

        Texture& operator =(const Texture&) = delete;

        Texture& operator =(Texture&& aTempTexture)
        {
            mName = std::move(aTempTexture.mName);
            mWidth = aTempTexture.mWidth;
            mHeight = aTempTexture.mHeight;
            mBytesPerPixel = aTempTexture.mBytesPerPixel;
            mType = aTempTexture.mType;
            mFormat = aTempTexture.mFormat;
            mPixels = std::move(aTempTexture.mPixels);
            aTempTexture.Clear();
            return *this;
        }

        void Clear() 
        {     
            mName.clear();
            mWidth = 0;
            mHeight = 0;
            mBytesPerPixel = 0;
            mType = 0;
            mFormat = 0;
            mPixels.clear();
        }

        bool Load(const std::string& aFilename);
    };
}
