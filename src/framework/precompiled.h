/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/


#pragma once

#define NOMINMAX //avoid windef.h to interfer with std::min std::max
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc

// Check windows
#if _WIN32 || _WIN64
    #if _WIN64
        #define ENVIRONMENT64
    #else
        #define ENVIRONMENT32
    #endif
#endif

// Check GCC
#if __GNUC__
    #if __x86_64__ || __ppc64__
        #define ENVIRONMENT64
    #else
        #define ENVIRONMENT32
    #endif
#endif

//System wise includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <sstream>      // std::stringstream
#include <string>      // std::stringstream
#include <vector>
#include <algorithm>
#include <exception> // For handling the exceptions
#include <memory>    // For smart pointers

#ifdef  _WIN32
    #include <tchar.h>
    #include <windows.h>
#elif __linux__
//JSON includes
    #include "json/allocator.h"
    #include "json/assertions.h"
    #include "json/autolink.h"
    #include "json/config.h"
    #include "json/features.h"
    #include "json/forwards.h"
    #include "json/json.h"
    #include "json/reader.h"
    #include "json/value.h"
    #include "json/version.h"
    #include "json/writer.h"
#endif

#include <math.h>

//Render
#include "core/opengl.h"

//Project includes
#include "core/profiler.h"
#include "core/types.h"
#include "core/logger.h"
#include "core/math.h"
#include "core/signal.h"
#include "core/utils.h"
#include "core/lua.h"

using namespace std;
