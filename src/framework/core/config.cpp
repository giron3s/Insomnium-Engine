/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/
#include "precompiled.h"

#include "config.h"
#include "json/json.h"
#include "core/serialization/jsoncpputils.h"
#include "core/serialization/serializableobject.h"

#ifdef _WIN32
    #include <direct.h>
#elif __linux__
    #include <unistd.h>
#endif


namespace Framework
{
    Config::Config(const string& aConfigFileName)
        : mGameName("")
        , mInitialStateFile("")
        , mResolution{ 800, 600 }
        , mFullscreen(false)
        , mResizable(false)
        , mUIThemeFile("")
        , mLogFile("")
        , mLogLevel (LogLevel::eLEVEL1)
    {
        //Load the current directory
        char lBuffer[256] = { '\0' };
        getcwd(lBuffer, sizeof(lBuffer)); // current directory

#ifdef WIN32
    // replace annoying Windows backslashes in path
        for (char* p = lBuffer; *p; ++p)
        {
            if (*p == '\\')
                *p = '/';
        }
#endif
        mDataDirectory.assign(lBuffer);
        mDataDirectory.append("/data");


        //Load the configuration
        Json::Value lConfig;
        Json::JsonUtils::OpenAndParseJsonFromFile(lConfig, aConfigFileName);
        LoadFromJson(lConfig);
    }

    void Config::LoadFromJson(const Json::Value& aSerializer)
    {
        mGameName = aSerializer["game"]["gamename"].asString();
        mInitialStateFile = aSerializer["game"]["initialstatefile"].asString();
        mLanguageFile = aSerializer["languagefile"].asString();
        mUIThemeFile = aSerializer["game"]["uithemefile"].asString();
        mLogLevel = static_cast<LogLevel>(aSerializer["game"].get("loglevel", mLogLevel).asInt());
        mLogFile = aSerializer["game"].get("logfile", mLogFile).asString();
        Logger::Initialize(mLogFile, mLogLevel);

        LoadInputFromJson(aSerializer["input"]);
        LoadGraphicsFromJson(aSerializer["graphics"]);
        LoadResourcesFromJson(aSerializer["resources"]);
        LoadPrefabsFromJson(aSerializer["prefabs"]); 
        LoadCatalogsFromJson(aSerializer["catalogs"]);   
    }

    void Config::LoadInputFromJson(const Json::Value& aSerializer)
    {
        mKeyboardSensibility = aSerializer.get("keyboard_sensibility", 100.0f).asFloat(); 
        mMouseSensibility = aSerializer.get("mouse_sensibility", 100.0f).asFloat(); 
        mMouseZoomSensibility = aSerializer.get("mouse_zoom_sensibility", 100.0f).asFloat();
    }

    void Config::LoadGraphicsFromJson(const Json::Value& aSerializer)
    {
        DeserializeVec<glm::uvec2>(aSerializer["resolution"], mResolution);
        mResizable     = aSerializer["resizable"].asBool();
        mFullscreen    = aSerializer["fullscreen"].asBool();

        mSelectionLineWidth = aSerializer["selectionLineWidth"].asFloat();
        ASSERT(0.0f < mSelectionLineWidth && mSelectionLineWidth < 64.0f);
        mSelectionLineColor.r = aSerializer["selectionLineColor"][0].asUInt() / 255.0f;
        mSelectionLineColor.g = aSerializer["selectionLineColor"][1].asUInt() / 255.0f;
        mSelectionLineColor.b = aSerializer["selectionLineColor"][2].asUInt() / 255.0f;
        mSelectionLineColor.a = aSerializer["selectionLineColor"][3].asUInt() / 255.0f;
        ASSERT(0.0f <= mSelectionLineColor.r && mSelectionLineColor.r <= 1.0f &&
            0.0f <= mSelectionLineColor.g && mSelectionLineColor.g <= 1.0f &&
            0.0f <= mSelectionLineColor.b && mSelectionLineColor.b <= 1.0f &&
            0.0f <= mSelectionLineColor.a && mSelectionLineColor.a <= 1.0f);
        mFocusLineWidth = aSerializer["focusLineWidth"].asFloat();
        ASSERT(0.0f < mFocusLineWidth && mFocusLineWidth < 64.0f);
        mFocusLineColor.r = aSerializer["focusLineColor"][0].asUInt() / 255.0f;
        mFocusLineColor.g = aSerializer["focusLineColor"][1].asUInt() / 255.0f;
        mFocusLineColor.b = aSerializer["focusLineColor"][2].asUInt() / 255.0f;
        mFocusLineColor.a = aSerializer["focusLineColor"][3].asUInt() / 255.0f;
        ASSERT(0.0f <= mFocusLineColor.r && mFocusLineColor.r <= 1.0f &&
               0.0f <= mFocusLineColor.g && mFocusLineColor.g <= 1.0f &&
               0.0f <= mFocusLineColor.b && mFocusLineColor.b <= 1.0f &&
               0.0f <= mFocusLineColor.a && mFocusLineColor.a <= 1.0f);

        mFontInfo.path = aSerializer["font"]["path"].asString();
        ASSERT(!mFontInfo.path.empty());
        mFontInfo.size = aSerializer["font"]["size"].asUInt();
        ASSERT(mFontInfo.size > 1);
        mFontInfo.color.r = aSerializer["font"]["color"][0].asUInt() / 255.0f;
        mFontInfo.color.g = aSerializer["font"]["color"][1].asUInt() / 255.0f;
        mFontInfo.color.b = aSerializer["font"]["color"][2].asUInt() / 255.0f;
        mFontInfo.color.a = aSerializer["font"]["color"][3].asUInt() / 255.0f;
        ASSERT(0.0f <= mFontInfo.color.r && mFontInfo.color.r <= 1.0f &&
               0.0f <= mFontInfo.color.g && mFontInfo.color.g <= 1.0f &&
               0.0f <= mFontInfo.color.b && mFontInfo.color.b <= 1.0f &&
               0.0f <= mFontInfo.color.a && mFontInfo.color.a <= 1.0f);        
    }

    void Config::LoadResourcesFromJson(const Json::Value& aSerializer)
    {
        // TODO: check for duplicates in resources, prefabs etc
        Json::Value lConfigContent;
        if (aSerializer.isArray())
        { // an array of config items
            for (Json::ArrayIndex i = 0; i < aSerializer.size(); ++i)
            {
                ASSERT(aSerializer[i].isString());
                Json::JsonUtils::OpenAndParseJsonFromFile(lConfigContent, aSerializer[i].asString());
                if (lConfigContent.empty())
                {
                    WARNING("Json File %s is not well formed", aSerializer[i].asString().c_str());
                    ASSERT(false);
                }
                else
                    mResourceFiles.push_back(aSerializer[i].asString());
            }
        }
        else if (aSerializer.isString())
        { // one config item
            Json::JsonUtils::OpenAndParseJsonFromFile(lConfigContent, aSerializer.asString());
            if (lConfigContent.empty())
            {
                WARNING("Json File %s is not well formed", aSerializer.asString().c_str());
                ASSERT(false);
            }
            else
                mResourceFiles.push_back(aSerializer.asString());

        }
        else if (!aSerializer.isNull())
        {
            WARNING("'resources' has unknown type");
        }
    }

    void Config::LoadPrefabsFromJson(const Json::Value& aSerializer)
    {
        for (auto lPrefabFile : aSerializer["files"])
        {
            Json::Value lPrefabContent;
            Json::JsonUtils::OpenAndParseJsonFromFile(lPrefabContent, lPrefabFile.asString());
            string lName = lPrefabContent["prefabdefinition"]["name"].asString();
            if (lName.empty())
            {
                WARNING("Json File %s is not well formed", lPrefabFile.asString().c_str());
                ASSERT(false);
            }
            mPrefabFiles[lName] = lPrefabFile.asString();
        }
    }

    void Config::LoadCatalogsFromJson(const Json::Value& aSerializer)
    {
        for (auto lCatalogKey : aSerializer.getMemberNames())
        {
            Json::Value lCatalogContent;
            Json::JsonUtils::OpenAndParseJsonFromFile(lCatalogContent, aSerializer[lCatalogKey].asString());
            if (lCatalogContent.empty())
            {
                WARNING("Json File %s is not well formed", aSerializer[lCatalogKey].asString().c_str());
                ASSERT(false);
            }
            mCatalogFiles[lCatalogKey] = aSerializer[lCatalogKey].asString();
        }
	}

    static const std::string sEmptyString;

    const string& Config::GetPrefabFile(const string& aName)
    {
        map<string, string>::iterator lResult;
        lResult = mPrefabFiles.find(aName);
        if (lResult != mPrefabFiles.end())
        {
            return lResult->second;
        }
        return sEmptyString;
    }

    void Config::GetPrefabFiles(vector<string>& aFilesOut) const
    {
        for (const auto& it : mPrefabFiles)
        {
            aFilesOut.push_back(it.second);
        }
    }

    const string& Config::GetCatalogFile(const string& aName)
    {
        map<string, string>::iterator lResult;
        lResult = mCatalogFiles.find(aName);
        if (lResult != mCatalogFiles.end())
            return lResult->second;
        return sEmptyString;
    }
}
