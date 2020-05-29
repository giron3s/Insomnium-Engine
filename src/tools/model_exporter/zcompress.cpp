/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/

#include "precompiled.h"
#include "zcompress.h"

using namespace Framework;
using namespace Tool;


#if _WIN32
int main(int argc, char **argv)
{
    ZCompress(argc, argv);
    return 0;
}
#endif

