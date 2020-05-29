/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once

#include <string>
#include <fstream>
#include <utility>
#include <algorithm>    // std::transform

#include "logger.h"
#include "nanovg/nanovg.h"

#ifdef __linux__
    #include <cxxabi.h>
    #include <dirent.h>
#endif


#define NON_COPYABLE_CLASS(C) private: \
C( const C& ) = delete; \
C& operator=( const C& ) = delete


namespace Framework
{
    namespace Utils
    {
        static std::string Demangling(const char* aMangledStr)
        {
            std::string lDemangledStr;
#ifdef __linux__
            int lStatus;
            char* lDemangledClassName = abi::__cxa_demangle(aMangledStr, 0, 0, &lStatus);
            lDemangledStr.assign("class ");
            lDemangledStr.append(lDemangledClassName);
            free(lDemangledClassName);
#elif _WIN32
            lDemangledStr.assign(aMangledStr);
#endif
            return lDemangledStr;
        };

        //Load the Image file
        static std::pair<int, std::string> LoadImageFile(const std::string &aFileName, NVGcontext *aCtx)
        {
            std::pair<int, std::string> lResult;
            int img = nvgCreateImage(aCtx, aFileName.c_str(), 0);
            if (img == 0)
            {
                CRASH("Could not open image data!");
            }
            lResult = std::make_pair(img, aFileName.substr(0, aFileName.length() - 4));
            return lResult;
        }


        //Load the Image Directory
        static std::vector<std::pair<int, std::string>> LoadImageDirectory(const std::string &aImageDirectory, NVGcontext *aCtx)
        {
            std::vector<std::pair<int, std::string> > result;
#if !defined(_WIN32)
            DIR *dp = opendir(aImageDirectory.c_str());
            if (!dp)
            {
                CRASH("Could not open image directory!");
            }
            struct dirent *ep;
            while ((ep = readdir(dp))) {
                const char *lFileName = ep->d_name;
#else
            WIN32_FIND_DATA ffd;
            std::string searchPath = aImageDirectory + "/*.*";
            HANDLE handle = FindFirstFileA(searchPath.c_str(), &ffd);
            if (handle == INVALID_HANDLE_VALUE)
                throw std::runtime_error("Could not open image directory!");
            do 
            {
                const char *lFileName = ffd.cFileName;
#endif
                if (strstr(lFileName, "png") == nullptr)
                    continue;
                std::string lFullName = aImageDirectory + "/" + std::string(lFileName);
                result.push_back( LoadImageFile(lFullName, aCtx) );
#if !defined(_WIN32)
            }
            closedir(dp);
#else
            } while (FindNextFileA(handle, &ffd) != 0);
            FindClose(handle);
#endif
            return result;
        }

        // Does checks and returns the contents of the file
        // If it failed, it returns an empty string
        static std::string GetFileContents(const std::string& aFilePath)
        {
            std::string content;
            std::ifstream fileStream(aFilePath, std::ios::in);

            if(!fileStream.is_open())
            {
                WARNING("'%s' doesn't exist or cannot be opened!", aFilePath.c_str());
                return std::string();
            }

            std::string line = "";
            while(!fileStream.eof()) {
                std::getline(fileStream, line);
                content.append(line + "\n");
            }

            fileStream.close();
            return content;
        };

        //Return with the file extension
        static std::string GetFileExtension(const std::string& aFileName)
        {
            if (aFileName.find_last_of(".") != std::string::npos)
            {
                std::string lFileExtension = aFileName.substr(aFileName.find_last_of(".") + 1);
                std::transform(lFileExtension.begin(), lFileExtension.end(), lFileExtension.begin(), ::tolower);
                return lFileExtension;
            }
                
            return "";
        }

        //Get the path separator
        inline char GetPathSeparator()
        {
            #ifdef _WIN32
                        return '\\';
            #else
                        return '/';
            #endif
        }

        //Return with the name of the file
        static std::string GetFileName(const std::string& aFileName)
        {
            size_t lInitPos = aFileName.find_last_of(GetPathSeparator());
            size_t lFinPos  = aFileName.find_last_of(".");

            if (std::string::npos == lFinPos)
            {
                ASSERT("The filename has not any extension");
                return std::string("");
            }

            if (std::string::npos == lInitPos)
                lInitPos = 0;
            else
                lInitPos += 1;
            return aFileName.substr(lInitPos, lFinPos);
        }

        //Return with the file folder
        static std::string GetFilePath(const std::string& aFileName)
        {
            size_t lFound;
            lFound = aFileName.find_last_of(GetPathSeparator());

            if (std::string::npos == lFound)
                return std::string("");
            else 
                return aFileName.substr(0, lFound);
        }

        #ifdef __unix
            #define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL
        #endif
    }
};



