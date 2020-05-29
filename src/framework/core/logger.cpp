/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#include "precompiled.h"
#include "logger.h"

#include <stdarg.h>
#include <time.h>

namespace Framework
{
    LogLevel Logger::mLogLevel = LogLevel::eLEVEL1;
    std::ofstream* Logger::mFileStream = nullptr;
    std::vector<std::string> Logger::mLogEntries = vector<string>();

    std::string GetExePath()
    {
        char buffer[MAX_PATH];
        GetModuleFileName(NULL, buffer, MAX_PATH);

        string::size_type pos = string(buffer).find_last_of("\\/");
        if (pos == string::npos)
            return "";
        return string(buffer).substr(0, pos);
    }

    bool CreateLogDirectory(std::string aDirPath)
    {
        DWORD dwAttrib = GetFileAttributes(aDirPath.c_str());

        if (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
            return true; //  Directory already exists so return true
        else
            return CreateDirectory(aDirPath.c_str(), NULL); //  Try to create the Logs directory
    }

    bool Logger::Initialize(std::string aLogFile, LogLevel aLogLevel)
    {
        if (aLogLevel < 0 || aLogLevel > LogLevel::eLEVEL3)
        {
            WARNING("Incorrect Log Level - Setting to default level ");
            mLogLevel = LogLevel::eLEVEL1;
        }
        else
        {
            mLogLevel = aLogLevel;
        }

        string lExePath = GetExePath();
        string lLogDirPath = lExePath + "\\Logs";
        string lLogFilePath = lLogDirPath + "\\" + aLogFile;

        if (!CreateLogDirectory(lLogDirPath))
        {
            WARNING("Cannot create Logs directory %s", lLogDirPath.c_str());
            return false;
        }
        
        mFileStream = new std::ofstream(lLogFilePath, std::ios::out);
        if (!mFileStream->is_open())
        {
            WARNING("Cannot open log file for writing %s", lLogFilePath.c_str());
            return false;
        }

        return true;
    }

    bool Logger::DeInitialize()
    {
        if (!mFileStream || !mFileStream->is_open())
            return false;

        mFileStream->close();
        delete mFileStream;
        mFileStream = nullptr;
        return true;
    }

    vector<string> Logger::GetLogEntires()
    {
        return mLogEntries;
    }

    void Logger::LoggerPrint(const std::string& aPrettyFunction, const std::string& aInfo/* = ""*/)
    {
#ifdef WIN32
        string lOutputStr;
        lOutputStr.append(GetLocalTime());
        lOutputStr.append("[" + ClassMethodName(aPrettyFunction) + "] ");
        lOutputStr.append(aInfo);
        lOutputStr.append("\n");

        OutputDebugString(lOutputStr.c_str());
        mLogEntries.push_back(lOutputStr);

        if (mFileStream && mFileStream->is_open())
        {
            mFileStream->write(lOutputStr.c_str(), lOutputStr.size());
        }

#elif __linux
        cout << GetLocalTime();
        cout << ClassMethodName(aPrettyFunction);
        cout << aInfo << endl;
#endif
    }

    inline string Logger::GetLocalTime()
    {
        //Time
        int   n = 0;
        char  lBuffer[4096];
        char* lBufferPtr = lBuffer;

        struct timespec ts;
        timespec_get(&ts, TIME_UTC);
        n = strftime(lBuffer, sizeof lBuffer, "%D %T", gmtime(&ts.tv_sec));
        n += snprintf(lBuffer + n, sizeof lBuffer - n, ".%01ld UTC|", ts.tv_nsec / 1000);

        //Insert '\n' to the tail
        lBufferPtr += (n < 0) ? sizeof lBuffer - 3 : n;
        *lBufferPtr = '\0';

        return string(lBuffer);
    }

    string Logger::MethodName(const string& aPrettyFunction)
    {
        size_t end = aPrettyFunction.find("(") - 1;
        size_t begin = aPrettyFunction.substr(0, end).rfind("::") + 2;

        return aPrettyFunction.substr(begin, end - begin + 1);
    }

    string Logger::ClassName(const string& aPrettyFunction)
    {
        size_t parentesis  = aPrettyFunction.find("(") - 1;
        size_t end = aPrettyFunction.substr(0, parentesis).rfind("::");
        size_t begin = aPrettyFunction.substr(0, end).rfind(" ") + 1;

        return aPrettyFunction.substr(begin, end - begin);
    }

    string Logger::ClassMethodName(const string& aPrettyFunction)
    {
        return ClassName(aPrettyFunction) + "::" + MethodName(aPrettyFunction);
    }
}
