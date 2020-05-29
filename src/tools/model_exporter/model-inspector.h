/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "graphic/asset3d.h"
#include "core/graphic/asset3dloaders.h"

using namespace Framework;

namespace Tool
{
    int ModelInspector(int argc, char **argv)
    {
        if (argc < 3)
        {
            INFO(LogLevel::eLEVEL2, "Insomnium Engine Tools\n\n");
            INFO(LogLevel::eLEVEL2, "Usage: [OPTION] ... PARAMERTERS\n");
            INFO(LogLevel::eLEVEL2, "\n");

            INFO(LogLevel::eLEVEL2, "Options:\n");
            INFO(LogLevel::eLEVEL2, "  -i, --inspect-asset <input_asset>                 Shows statistics about an internal asset\n\n\n");
            INFO(LogLevel::eLEVEL2, "                                                    <input_asset>: internal asset path and name\n\n");
            exit(1);
        }

        Asset3D lAsset("","");

        if (lAsset.Load(argv[2]) == false)
        {
            CRASH("ERROR loading asset from file %s\n", argv[2]);
            exit(3);
        }

        INFO(LogLevel::eLEVEL2, "\nAsset3D:   %s\n", argv[2]);
        INFO(LogLevel::eLEVEL2, "Asset data:");
        printf("%s", lAsset.ToString().c_str());

        return 0;
    }
}
