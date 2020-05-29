/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential

* Brief        : Contains classes to perform compression/decompression using zlib
*******************************************************************************/

#include "precompiled.h"
#include "core/graphic/zcompression.h"

namespace Framework
{
    ZCompression::ZCompression()
    {
        mOutBuffer = new uint8_t[ChunkSize];
        if (mOutBuffer == NULL) 
        {
            CRASH("ERROR allocating buffer in ZCompression constructor\n");
        }
    }

    ZCompression::~ZCompression() 
    {
        delete[] mOutBuffer;
    }

    bool ZCompression::Init()
    {
        /* allocate deflate state */
        mStream.zalloc = Z_NULL;
        mStream.zfree = Z_NULL;
        mStream.opaque = Z_NULL;

        if (deflateInit(&mStream, Z_DEFAULT_COMPRESSION) != Z_OK) {
            return false;
        }

        return true;
    }

    bool ZCompression::Write(std::ofstream &file, const void *data, uint32_t size, bool end)
    {
        mStream.avail_in = size;
        mStream.next_in = (unsigned char *)data;

        do {
            mStream.avail_out = ChunkSize;
            mStream.next_out = mOutBuffer;

            if (deflate(&mStream, end) == Z_STREAM_ERROR) 
            {
                CRASH("ERROR deflating data\n");
                Finish();
                return false;
            }

            file.write((char *)mOutBuffer, ChunkSize - mStream.avail_out);
        } while (mStream.avail_out == 0);

        if (mStream.avail_in != 0) {
            CRASH("ERROR compressing data, not all input was used\n");
            Finish();
            return false;
        }

        return true;
    }

    void ZCompression::Finish() 
    {
        (void)deflateEnd(&mStream); 
    }

    ZDecompression::ZDecompression()
    {
        mInputBuffer = new uint8_t[ChunkSize];
        if (mInputBuffer == NULL) 
        {
            CRASH("ERROR allocating input buffer in ZDecompression constructor\n");
        }
    }

    ZDecompression::~ZDecompression() 
    {
        delete[] mInputBuffer;
    }

    bool ZDecompression::Init()
    {
        /* allocate deflate state */
        mStream.zalloc = Z_NULL;
        mStream.zfree = Z_NULL;
        mStream.opaque = Z_NULL;
        mStream.avail_in = 0;
        mStream.next_in = Z_NULL;

        int lResult = inflateInit(&mStream);
        if (lResult == Z_STREAM_ERROR)
        {
            WARNING("Invalid parameter at inflateInit");
            return false;
        }
        else if (lResult == Z_MEM_ERROR)
        {
            WARNING("Insufficient memory available.");
            return false;
        }
        else if (lResult == Z_VERSION_ERROR)
        {
            WARNING("The version requested is not compatible with the library version, or the z_stream size differs from that used by the library.");
            return false;
        }
        return true;
    }

    bool ZDecompression::Read(std::ifstream &file, void *data, const uint32_t &size)
    {
        /* 'size' will be updated but change is lost as size is passed by value */
        uint32_t tmpSize = size;
        return Read(file, data, tmpSize);
    }

    bool ZDecompression::Read(std::ifstream &file, void *data, uint32_t &size)
    {
        uint32_t initSize = size;

        mStream.avail_out = size;
        mStream.next_out = (unsigned char *)data;

        while (mStream.avail_out != 0) {
            /* Check if we need to read more inptu data */
            if (mStream.avail_in == 0) {
                file.read((char *)mInputBuffer, ChunkSize);

                mStream.next_in = mInputBuffer;
                mStream.avail_in = (int)file.gcount();

                if (mStream.avail_in == 0) {
                    size = initSize - mStream.avail_out;
                    return true;
                }
            }

            /* Inflate until we ran out of input data or
            * output data is completed */
            do {
                int ret = inflate(&mStream, Z_NO_FLUSH);

                if (ret == Z_STREAM_ERROR) 
                {
                    CRASH("ERROR deflating data\n");
                    Finish();
                    return false;
                }
                switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR; /* and fall through */
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void)inflateEnd(&mStream);
                    return false;
                }
            } while (mStream.avail_out != 0 && mStream.avail_in != 0);
        }

        return true;
    }

    void ZDecompression::Finish()
    { 
        (void)inflateEnd(&mStream); 
    }
}
