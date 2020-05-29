/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/

#include <stdio.h>
#include <fstream>

#include "precompiled.h"
#include "core/graphic/zcompression.h"

#define CHUNK_SIZE 2133

using namespace Framework;

namespace Tool
{
    int ZCompress(int argc, char **argv)
    {
        if (argc < 3)
        {
            INFO(LogLevel::eLEVEL2, "Insomnium Engine Tools\n\n");
            INFO(LogLevel::eLEVEL2, "Usage: [OPTION] ... PARAMERTERS\n");
            INFO(LogLevel::eLEVEL2, "\n");

            INFO(LogLevel::eLEVEL2, "Options:\n");
            INFO(LogLevel::eLEVEL2, "  -z, --zcompress <operation> <input> <output>     Tool to lCompress and decompress files\n");
            INFO(LogLevel::eLEVEL2, "                                                   <operation> - 'c' for compress and 'd' for decompress, without quotes\n");
            INFO(LogLevel::eLEVEL2, "                                                   <input>     - Input file\n");
            INFO(LogLevel::eLEVEL2, "                                                   <output>    - Output file\n\n");

            INFO(LogLevel::eLEVEL2, "  -h, --help                                       Display this help and exit");
            exit(1);
        }

        uint8_t lBuffer[CHUNK_SIZE];
        uint32_t lReturn = -1;

        switch (argv[2][0])
        {
             case 'c':
             {
                 ZCompression lComp;
                 FILE *lInput = fopen(argv[3], "rb");
                 std::ofstream lOutput(argv[4], std::ios::binary | std::ios::trunc | std::ios::out);

                 if (lInput == NULL) {
                     fprintf(stderr, "ERROR opening lInput file %s\n", argv[3]);
                     goto error_comp;
                 }

                 if (lOutput.is_open() == false) {
                     fprintf(stderr, "ERROR opening output file %s\n", argv[3]);
                     goto error_comp;
                 }

                 if (lComp.Init() == false) {
                     fprintf(stderr, "ERROR initializing lCompressor\n");
                     goto error_comp;
                 }

                 while (feof(lInput) == 0) {
                     size_t bytesRead = fread(lBuffer, 1, CHUNK_SIZE, lInput);

                     if (lComp.Write(lOutput, lBuffer, (uint32_t)bytesRead, feof(lInput) != 0) == false) {
                         fprintf(stderr, "ERROR writing lCompressed data\n");
                         goto error_comp;
                     }
                 }

                 /* Everything OK */
                 lReturn = 0;

     error_comp:
                 lComp.Finish();
                 fclose(lInput);
                 lOutput.close();
             }
             break;


             case 'd':
             {
                 ZDecompression lDComp;
                 uint32_t lBytesRead;
                 std::ifstream lInput(argv[3], std::ios::binary | std::ios::in);
                 FILE *lOutput= fopen(argv[4], "wb");

                 if (lInput.is_open() == false) {
                     fprintf(stderr, "ERROR opening lInput file %s\n", argv[3]);
                     goto error_dec;
                 }

                 if (lOutput == NULL) {
                     fprintf(stderr, "ERROR opening output file %s\n", argv[3]);
                     goto error_dec;
                 }

                 if (lDComp.Init() == false) {
                     fprintf(stderr, "ERROR initializing decompressor\n");
                     goto error_dec;
                 }

                 do {
                     lBytesRead = CHUNK_SIZE;

                     if (lDComp.Read(lInput, lBuffer, lBytesRead) == false) {
                         fprintf(stderr, "ERROR writing decompressed data\n");
                         goto error_dec;
                     }

                     if (fwrite(lBuffer, 1, lBytesRead, lOutput) != lBytesRead) {
                         fprintf(stderr, "ERROR writing to file %s\n", argv[4]);
                         goto error_dec;
                     }
                 } while (lBytesRead != 0);

                 /* Everything OK */
                 lReturn = 0;

     error_dec:
                 lDComp.Finish();
                 lInput.close();
                 fclose(lOutput);
             }
             break;
             default:
                 fprintf(stderr, "ERROR wrong operation option, use 'c' or 'd' without quotes\n");
         }

         if (lReturn == 0)
         {
             fprintf(stderr, "Operation finished succesfully\n");
         }
         exit(lReturn);
    }
}
