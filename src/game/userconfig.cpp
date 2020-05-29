/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "userconfig.h"

#include "engine.h"
#include "appmanager.h"
#include "engine/script.h"

using namespace Framework;
using namespace LuaIntf;

namespace Game
{
    UserConfig::UserConfig()
        : Container(Utils::Demangling(typeid(UserConfig).name()))
        , mFTUEEnabled(false)
        , mAutosaveEnabled(false)
        , mAutosaveTime(0)
        , mLogLevel(0)
        , mLanguage("DEFAULT")
        , mLoggedIn(false)
    {

    }

    UserConfig::~UserConfig()
    {
       
    }

    void UserConfig::Initialize()
    {
        LuaContext& lLuaContext = Engine::Instance()->Script().GetLuaContext();
        LuaState lLuaState = lLuaContext.state();

        LuaBinding(lLuaState).addFunction("GetUserConfig", [this]() -> UserConfig&
        {
            string lContainerName = Utils::Demangling(typeid(UserConfig).name());
            return static_cast<UserConfig&>(Engine::Instance()->Container(lContainerName));
        });

        LuaBinding(lLuaState).beginClass<UserConfig>("UserConfig")
            .addFunction("GetLogLevel", &UserConfig::GetLogLevel)
            .addFunction("SetLogLevel", &UserConfig::SetLogLevel, LUA_ARGS(int))
            .addFunction("GetLanguage", &UserConfig::GetLanguage)
            .addFunction("SetLanguage", &UserConfig::SetLanguage, LUA_ARGS(std::string))
            .addFunction("GetAutosaveEnabled", &UserConfig::GetAutosaveEnabled)
            .addFunction("SetAutosaveEnabled", &UserConfig::SetAutosaveEnabled, LUA_ARGS(bool))
            .addFunction("GetAutosaveTime", &UserConfig::GetAutosaveTime)
            .addFunction("SetAutosaveTime", &UserConfig::SetAutosaveTime, LUA_ARGS(int))
            .addFunction("GetLoggedIn", &UserConfig::GetLoggedIn)
            .addFunction("SetLoggedIn", &UserConfig::SetLoggedIn, LUA_ARGS(bool))
            .endClass();

        Json::Value lSerializer;
        bool lIsOk = Json::JsonUtils::OpenAndParseJsonFromFile(lSerializer, "data/config/user.config");

        if (!lIsOk) {
            CRASH("Cannot load user.config");
            return;
        }

        if (!lSerializer.isMember("FTUE")) {
            CRASH("user.config contains no FTUE data");
            return;
        }

        Json::Value lFTUEData = lSerializer["FTUE"];
        if (lFTUEData.isMember("enable"))
            mFTUEEnabled = lFTUEData["enable"].asBool();

        if (lSerializer.isMember("AUTOSAVE"))
        {
            Json::Value lAutosaveData = lSerializer["AUTOSAVE"];
            mAutosaveEnabled = lAutosaveData.get("enable", mAutosaveEnabled).asBool();
            mAutosaveTime = lAutosaveData.get("autosaveTime", mAutosaveTime).asInt();
        }

        mLogLevel = lSerializer.get("LOGLEVEL", 1).asInt();
        mLanguage = lSerializer.get("LANGUAGE", "DEFAULT").asString();
        mLoggedIn = lSerializer.get("LOGGEDIN", false).asBool();
    }

    void UserConfig::DeInitialize()
    {
        Json::Value lSerializer;
        Json::Value& lFTUE = lSerializer["FTUE"];
        lFTUE["enable"] = mFTUEEnabled;

        Json::Value& lAutosave = lSerializer["AUTOSAVE"];
        lAutosave["enable"] = mAutosaveEnabled;
        lAutosave["autosaveTime"] = mAutosaveTime;
        lSerializer["LOGLEVEL"] = mLogLevel;
        lSerializer["LANGUAGE"] = mLanguage;
        lSerializer["LOGGEDIN"] = mLoggedIn;

        Json::StyledWriter writer;
        string lOutput = writer.write(lSerializer);
        fstream lFileOut("data/config/user.config", ios_base::out);
        lFileOut << lOutput;
    }
}
