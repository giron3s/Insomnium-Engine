 /*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential

 * Brief        : Contains classes to perform compression/decompression using zlib
 *******************************************************************************/
#pragma once

#include <fstream>
#include <stdint.h>
#include "zlib.h"

namespace Framework
{
    /**
    * @class   ZCompression
    * @brief   Compresses and writes the given input to the given output stream
    */
    class ZCompression
    {
    public:
        /**
        * Size of the output buffer provided to zlib to
        * write the compressed output
        */
        static const uint32_t ChunkSize = 16384;

        /**
        * Constructor
        */
        ZCompression();

        /**
        * Destructor
        */
        ~ZCompression();

        /**
        * Initializes the compression
        */
        bool Init();

        /**
        * Compresses and writes 'size' bytes from 'data' to 'file'
        *
        * @param file  Output file to write data to
        * @param data  Data to compress and write to 'file'
        * @param size  Number of bytes in 'data' to compress and write to 'file'
        * @param end   True if this is the last chunk of data to be compressed
        *
        * @return true if the data was compressed and written correctly, false
        *         otherwise
        */
        bool Write(std::ofstream &file, const void *data, uint32_t size, bool end = false);

        /**
        * Finishes the compression. To use the compression
        * object again, init must be called
        */
        void Finish();

    private:
        z_stream mStream;    /**< ZLib compression state */
        uint8_t *mOutBuffer; /**< Output buffer provided to ZLib */
    };

    /**
    * @class   ZDecompression
    * @brief   Reads and decompresses data from the given input stream
    */
    class ZDecompression
    {
    public:
        /**
        * Size of the output buffer provided to zlib to
        * write the compressed output
        */
        static const uint32_t ChunkSize = 16384;

        /**
        * Constructor
        */
        ZDecompression();

        /**
        * Destructor
        */
        ~ZDecompression();

        /**
        * Initializes the decompression
        */
        bool Init();

        /**
        * Reads bytes from 'file' decompressing them until a maximum of
        * 'size' bytes have been decompressed, then writes those bytes to 'data'
        *
        * @param file  Input file to read data from
        * @param data  Buffer where the decompressed data will be written to
        * @param size  Maximum number of decompressed bytes of data to obtain from 'file'
        *
        * @return true if the data was read and decompressed correctly, false
        *         otherwise
        */
        bool Read(std::ifstream &file, void *data, const uint32_t &size);
        bool Read(std::ifstream &file, void *data, uint32_t &size);

        /**
        * Finishes the decompression. To use the decompression
        * object again, init must be called
        */
        void Finish();

    private:
        z_stream mStream;      /**< ZLib compression state */
        uint8_t *mInputBuffer; /**< Input buffer provided to ZLib */
    };

}