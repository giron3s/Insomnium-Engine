/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Holds the texture data
 *******************************************************************************/

#pragma once

#include "precompiled.h"
#include "texture.h"
#include "core/logger.h"
#include <IL/il.h>

namespace Framework
{
    bool Texture::Load(const std::string& aFilename)
    {
        Clear();
        if ( aFilename.empty() )
            return false;
        static bool sCallILInit = true;
        if ( sCallILInit ) {
            ilInit();
            sCallILInit = false;
        }
        ilEnable(IL_ORIGIN_SET);
        ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
        ILenum lError = IL_NO_ERROR;
        ILuint lILImageId = -1;
        ilGenImages(1, &lILImageId);
        ilBindImage(lILImageId);
        if ( !ilLoadImage(aFilename.c_str()) )
            goto end_label;
        if ( ilGetInteger(IL_IMAGE_FORMAT) == IL_COLOR_INDEX ) {
            if ( !ilConvertImage(IL_BGR, IL_UNSIGNED_BYTE) )
                goto end_label;
        }

        mName = Utils::GetFileName(aFilename);
        mWidth = ilGetInteger(IL_IMAGE_WIDTH);
        mHeight = ilGetInteger(IL_IMAGE_HEIGHT);
        mBytesPerPixel = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
        mFormat = ilGetInteger(IL_IMAGE_FORMAT); // RGB8, RGBA8, RED8, RG8
        mType = ilGetInteger(IL_IMAGE_TYPE); // RGB, BGR, RGBA, BGRA, RED, RG
        uint32_t lDataSize = mWidth * mBytesPerPixel * mHeight;
        mPixels.resize(lDataSize, 0);
        memcpy(mPixels.data(), ilGetData(), lDataSize);
end_label:;
        if ( lError == IL_NO_ERROR )
            lError = ilGetError();
        ilDeleteImages(1, &lILImageId);
        if ( lError != IL_NO_ERROR )
            WARNING("Failed to load image \"%s\". IL error code 0x%X", aFilename.c_str(), lError);
        return lError == IL_NO_ERROR;
    }
}
