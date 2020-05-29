/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#include "precompiled.h"
#include "core/graphic/asset3dloaders.h"

using namespace Framework;

namespace Tool
{
    int Obj2Engine(int argc, char **argv)
    {
        if (argc < 4)
        {
            INFO(LogLevel::eLEVEL2, "Insomnium Engine Tools\n\n");
            INFO(LogLevel::eLEVEL2, "Usage: [OPTION] ... PARAMERTERS\n");
            INFO(LogLevel::eLEVEL2, "\n");

            INFO(LogLevel::eLEVEL2, "Options:\n");
            INFO(LogLevel::eLEVEL2, "  -o, --obj2engine <input_obj> <output_engine>      OBJ asset files to engine internal asset file converter\n");
            INFO(LogLevel::eLEVEL2, "                                                    <input_obj>: directory containing the geometry.obj, material.mtl and all textures files\n");
            INFO(LogLevel::eLEVEL2, "                                                    <output_engine>: filename for the engine binary representation file\n\n");
            exit(1);
        }

        Asset3D lAsset("","");

        if (Asset3DLoaders::LoadOBJ(lAsset, argv[2]) == false)
        {
            CRASH("ERROR opening input OBJ asset %s\n", argv[2]);
            exit(2);
        }

        INFO(LogLevel::eLEVEL2, "Asset info:");
        printf("%s", lAsset.ToString().c_str());

        if (lAsset.Save(argv[3]) == false)
        {
            CRASH("ERROR storing asset to output file %s\n", argv[3]);
            exit(3);
        }

        INFO(LogLevel::eLEVEL2, "Created %s succesfully\n\n", argv[3]);

        return 0;
    }
}
