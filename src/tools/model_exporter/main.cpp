/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "obj2engine.h"
#include "model-inspector.h"
#include "zcompress.h"

using namespace Framework;
using namespace Tool;

void info( )
{
    INFO(LogLevel::eLEVEL2, "Insomnium Engine Tools\n\n");
    INFO(LogLevel::eLEVEL2, "Usage: [OPTION] ... PARAMERTERS\n");
    INFO(LogLevel::eLEVEL2, "\n");

    INFO(LogLevel::eLEVEL2, "Options:\n");
    INFO(LogLevel::eLEVEL2, "  -i, --inspect-asset <input_asset>                Shows statistics about an internal asset\n");
    INFO(LogLevel::eLEVEL2, "                                                   <input_asset>: internal asset path and name\n\n");

    INFO(LogLevel::eLEVEL2, "  -o, --obj2engine <input_obj> <output_engine>     OBJ asset files to engine internal asset file converter\n");
    INFO(LogLevel::eLEVEL2, "                                                   <input_obj>: directory containing the geometry.obj, material.mtl and all textures files\n");
    INFO(LogLevel::eLEVEL2, "                                                   <output_engine>: filename for the engine binary representation file\n\n");

    INFO(LogLevel::eLEVEL2, "  -z, --zcompress <operation> <input> <output>     Tool to lCompress and decompress files\n");
    INFO(LogLevel::eLEVEL2, "                                                   <operation> - 'c' for compress and 'd' for decompress, without quotes\n");
    INFO(LogLevel::eLEVEL2, "                                                   <input>     - Input file\n");
    INFO(LogLevel::eLEVEL2, "                                                   <output>    - Output file\n\n");

    INFO(LogLevel::eLEVEL2, "  -h, --help                                       Display this help and exit");
    exit(1);
}

int main(int argc, char **argv)
{

    if (argc < 2)
    {
        INFO(LogLevel::eLEVEL2, "Missing the options");
        INFO(LogLevel::eLEVEL2, "Try '--help' for more information.");
    }
    else if (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--inspect-asset") == 0)
    {
        Tool::ModelInspector(argc, argv);
    }
    else if(strcmp(argv[1], "-o") == 0 || strcmp(argv[1], "--obj2engine") == 0)
    {
        Tool::Obj2Engine(argc, argv);
    }
    else if(strcmp(argv[1], "-z") == 0 || strcmp(argv[1], "--zcompress") == 0)
    {
        Tool::ZCompress(argc, argv);
    }
    else if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        INFO(LogLevel::eLEVEL2, );
    }
    else
    {
        INFO(LogLevel::eLEVEL2, "Unrecognized option: '%s'", argv[1]);
        INFO(LogLevel::eLEVEL2, "Try '--help' for more information.");
    }
    return 0;
}
