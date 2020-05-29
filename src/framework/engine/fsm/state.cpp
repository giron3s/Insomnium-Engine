/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/

#include "precompiled.h"
#include "engine/fsm/action.h"
#include "engine/fsm/state.h"

using namespace LuaIntf;

namespace Framework
{

    State::State( const std::string& aName,
                  const std::vector<std::string>& aActionList,
                  const std::vector<std::string>& aUIList,
                  LuaIntf::LuaState& aLuaState )

        : mName(aName)
        , mLuaState(aLuaState)
    {

        if(aName.empty())
        {
            CRASH("You cannot have a State with a null name");
        }

        for(auto lIter : aActionList)
        {
            AddAction(lIter);
        }

        for(auto lIter : aUIList)
        {
            AddUI(lIter);
        }
    }


    State::~State()
    {

    }


    void State::AddUI(const string& aUIName)
    {
        if(!FindUI(aUIName).empty())
        {
            CRASH("UI '%s' declared more than once", aUIName.c_str());
            return;
        }
        mUIs.push_back(aUIName);
    }

    const string& State::FindUI(const string& aUIName) const
    {
        auto lResult = find(mUIs.begin(), mUIs.end(), aUIName);
        if(lResult == mUIs.end())
        {
            static const string sEmptyString("");
            return sEmptyString;
        }
        return *lResult;
    }

    void State::ExecuteAction(const string& aActionName) const
    {
        for (const auto& lAction : mActions)
        {
            if (lAction->GetName() == aActionName)
            {
                lAction->Execute(mName);
                return;
            }
        }
    }

    void State::AddAction(const string& aActionName)
    {
        if(FindAction(aActionName))
        {
            CRASH("Action '%s' declared more than once", aActionName.c_str());
            return;
        }

        mActions.push_back(std::make_shared<const Action>(aActionName, mLuaState));
    }

    shared_ptr<const Action> State::FindAction(const string& aActionName) const
    {
        for ( auto& lAction : mActions )
        {
            if ( lAction->GetName() == aActionName )
                return lAction;
        }
        return shared_ptr<const Action>();
    }

    /*
     * Format of the action list has to follow the next rules
     *      - The action list has to start with '{' and has to end '}' character
     *      - The action names has to be separated with ","
     *        E.g. "{OnStart, OnAction1, OnAction2, OnEnd }"
     */
    vector<string> State::ParseActionList(const string& aActionList)
    {
        size_t lInitPos = aActionList.find("{");
        size_t lEndPos  =  aActionList.find("}");
        vector<string> lResult;


        if(lInitPos == string::npos || lEndPos == string::npos)
        {
            CRASH("Badly formatted the list of actions of the '%s' state", mName.c_str());
            return lResult;
        }

        //Remove the special characters
        char lSpecialChars[] = "()-{}[]. ";
        string lActionList = aActionList.substr(lInitPos + 1, lEndPos);
        for (unsigned int i = 0; i < strlen(lSpecialChars); ++i)
            lActionList.erase(remove(lActionList.begin(), lActionList.end(), lSpecialChars[i]), lActionList.end());

        //Split the action list at ',' delimiter
        char lDelimiter = ',';
        size_t lCurrent, lPrevious = 0;
        lCurrent = lActionList.find(lDelimiter);
        while (lCurrent != string::npos)
        {
            lResult.push_back(lActionList.substr(lPrevious, lCurrent - lPrevious));
             lPrevious = lCurrent + 1;
             lCurrent = lActionList.find(lDelimiter, lPrevious);
        }

        lResult.push_back(lActionList.substr(lPrevious, lCurrent - lPrevious));
        return lResult;
    }
}
