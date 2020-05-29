/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

#include <windows.h>
#include <assert.h>
#include <stdarg.h>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <fstream>
#include <stdlib.h>
#include <vector>

namespace Framework
{
    enum LogLevel
    {
        eLEVEL1,
        eLEVEL2,
        eLEVEL3
    };

    class Logger
    {
    public:
        static LogLevel mLogLevel;
        static std::ofstream* mFileStream;
        static std::vector<std::string> mLogEntries;

        static bool Initialize(std::string aLogFile, LogLevel aLogLevel);
        static bool DeInitialize();
        static std::vector<std::string> GetLogEntires();

        static void LoggerPrint(const std::string& aPrettyFunction, const std::string& aTraceParams = "");
        static inline std::string GetLocalTime();

        static std::string MethodName(const std::string& aPrettyFunction);
        static std::string ClassName(const std::string& aPrettyFunction);
        static std::string ClassMethodName(const std::string& aPrettyFunction);
    };
}

//CLASS NAME
#ifdef  _WIN32
    #define __CLASS_NAME__ Logger::ClassName(__FUNCSIG__)
#elif __linux__
    #define __CLASS_NAME__ Logger::ClassName(__PRETTY_FUNCTION__)
#endif

//METHOD NAME
#ifdef  _WIN32
    #define __METHOD_NAME__ Logger::MethodName(__FUNCSIG__)
#elif __linux__
    #define __METHOD_NAME__ Logger::MethodName(__PRETTY_FUNCTION__)
#endif

//CLASS & METHOD NAME
#ifdef  _WIN32
    #define __CLASS_METHOD_NAME__ Logger::ClassMethodName(__FUNCSIG__)
#elif __linux__
    #define __CLASS_METHOD_NAME__ Logger::ClassMethodName(__PRETTY_FUNCTION__)
#endif


#define VA_ARGS_NUM_PRIV(P1, P2, P3, P4, P5, P6, Pn, ...) Pn




//ASSERT
#define STR(x) #x
#ifndef NDEBUG
    #define ASSERT(expression) \
    do { \
        if ( !(expression)) \
        { \
            char lBuffer[1024]; int lBufferLen = 0; \
            lBufferLen += sprintf(lBuffer + lBufferLen, "Debug Assertion Failed!\n\n"); \
            lBufferLen += sprintf(lBuffer + lBufferLen, "Expression: %s\n\n", STR(expression)); \
            lBufferLen += sprintf(lBuffer + lBufferLen, "File: %s\n", Logger::ClassMethodName(__FUNCSIG__).c_str()); \
            lBufferLen += sprintf(lBuffer + lBufferLen, "Line: %d\n\n", __LINE__); \
            MessageBox(NULL, (LPCSTR)(lBuffer), (LPCSTR)("ASSERT FAILED!"), MB_ICONERROR | MB_OK); \
        } \
    } while (false);
#else
#   define ASSERT(expression, ...) do { } while (false);
#endif


//CRASH
#ifdef  _WIN32
    #define CRASH(format, ...) { \
        if( VA_ARGS_NUM_PRIV(__VA_ARGS__, 5, 4, 3, 2, 1, 0) >= 0)       \
        { \
            char lBuf[1024]; sprintf(lBuf, "CRASH: " format, ## __VA_ARGS__); \
            Logger::LoggerPrint(__FUNCSIG__, lBuf); \
        } \
        else \
            Logger::LoggerPrint(__FUNCSIG__); \
        assert(false); \
        exit(1); }
#elif __linux__
    #define CRASH(format, ...) { \
        if( VA_ARGS_NUM_PRIV(__VA_ARGS__, 5, 4, 3, 2, 1, 0) >= 0)       \
        { \
            char lBuf[1024]; sprintf(lBuf, "CRASH: " format, ## __VA_ARGS__); \
            Logger::LoggerPrint(__PRETTY_FUNCTION__, lBuf); \
        } \
        else \
            Logger::LoggerPrint(__PRETTY_FUNCTION__); \
        assert(false); \
        exit(1); }
#endif

//INFO
#ifdef  _WIN32
    #define INFO(level, format, ...) { \
        if( VA_ARGS_NUM_PRIV(__VA_ARGS__, 5, 4, 3, 2, 1, 0) >= 0)       \
        { \
            char lBuf[1024]; sprintf(lBuf, "INFO: " format, ## __VA_ARGS__); \
            if(level <= Logger::mLogLevel)           \
                Logger::LoggerPrint(__FUNCSIG__, lBuf); \
        } \
        else if(level <= Logger::mLogLevel)           \
                Logger::LoggerPrint(__FUNCSIG__); }
#elif __linux__
    #define INFO(level, format, ...) { \
        if( VA_ARGS_NUM_PRIV(__VA_ARGS__, 5, 4, 3, 2, 1, 0) >= 0)       \
        { \
            char lBuf[1024]; sprintf(lBuf, "INFO: " format, ## __VA_ARGS__); \
            if(level <= Logger::mLogLevel)   \
                Logger::LoggerPrint(__PRETTY_FUNCTION__, lBuf); \
        } \
        else if(level <= Logger::mLogLevel)       \
                Logger::LoggerPrint(__PRETTY_FUNCTION__); }
#endif


//WARNING
#ifdef  _WIN32
    #define WARNING(format, ...) { \
        if( VA_ARGS_NUM_PRIV(__VA_ARGS__, 5, 4, 3, 2, 1, 0) >= 0)       \
        { \
            char lBuf[1024]; sprintf(lBuf, "WARN: " format, ## __VA_ARGS__); \
            Logger::LoggerPrint(__FUNCSIG__, lBuf); \
        } \
        else \
            Logger::LoggerPrint(__FUNCSIG__); }
#elif __linux__
    #define WARNING(format, ...) { \
        if( VA_ARGS_NUM_PRIV(__VA_ARGS__, 5, 4, 3, 2, 1, 0) >= 0)       \
        { \
            char lBuf[1024]; sprintf(lBuf, "WARN: " format, ## __VA_ARGS__); \
            Logger::LoggerPrint(__PRETTY_FUNCTION__, lBuf); \
        } \
        else \
            Logger::LoggerPrint(__PRETTY_FUNCTION__); }
#endif
