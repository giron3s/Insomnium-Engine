/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/
#pragma once

#include "LuaRef.h"
#include "core/logger.h"
#include <iostream>

namespace Framework
{
    class State;

    class Action final
    {
    public:
        static const std::string START;
        static const std::string UPDATE;
        static const std::string EXIT;

        Action(const std::string& aName,
               LuaIntf::LuaState& aLuaState)

                   : mName(aName)
                   , mLuaState(aLuaState)
        {
            if(mName.empty())
            {
                CRASH("Cannot create an ACTION with null name");
            }
        }

        inline const std::string& GetName() const { return mName; }

        void Execute(std::string aStateName) const
        {
            //Launch the action
            string lFunctionName(aStateName + "_" + mName);
            try
            {
                INFO(LogLevel::eLEVEL2, "Launch the '%s' action", lFunctionName.c_str());
                LuaIntf::LuaRef lLuaRef(mLuaState, lFunctionName.c_str());
                lLuaRef();
            }
            // Checks for errors
            catch(const std::exception& e)
            {
                std::string lScriptErrorMsg = e.what();
                CRASH("Error occurred during running '%s' LUA function!\nError: '%s'\n", lFunctionName.c_str(), lScriptErrorMsg.c_str());
            }
        }

        template <typename T>
        void ExecuteWithArgs(std::string aStateName, T args) const
        {
            //Launch the action
            string lFunctionName(aStateName + "_" + mName);
            try
            {
                INFO(LogLevel::eLEVEL2, "Launch the '%s' action", lFunctionName.c_str());
                LuaIntf::LuaRef lLuaRef(mLuaState, lFunctionName.c_str());
                lLuaRef.call(args);
            }
            // Checks for errors
            catch (const std::exception& e)
            {
                std::string lScriptErrorMsg = e.what();
                CRASH("Error occurred during running '%s' LUA function!\nError: '%s'\n", lFunctionName.c_str(), lScriptErrorMsg.c_str());
            }
        }

    protected:
        std::string            mName;
        LuaIntf::LuaState      mLuaState;
    };
}
