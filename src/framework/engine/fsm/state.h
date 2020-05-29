/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/
#pragma once

#include "action.h"

namespace Framework
{
    class Action;

    class State final
    {
    public:
        State(const std::string& aName,
              const std::vector<std::string>& aActionList,
              const std::vector<std::string>& aUIList,
              LuaIntf::LuaState& aLuaState);

        ~State();

        inline const std::string&               GetName() const { return mName; }
        inline const std::vector<std::string>&  GetUIs() const { return mUIs; }

        void                                    ExecuteAction(const std::string& aActionName) const;

        template <typename T>
        void ExecuteActionWithArgs(const string& aActionName, T args) const
        {
            for (const auto& lAction : mActions)
            {
                if (lAction->GetName() == aActionName)
                {
                    lAction->ExecuteWithArgs(mName, args);
                    return;
                }
            }
        }

    private:
        void                AddUI(const std::string& aUIName);
        const std::string&  FindUI(const std::string& aUIName) const;

        void                          AddAction(const std::string& aActionName);
        std::shared_ptr<const Action> FindAction(const std::string& aActionName) const;
        std::vector<std::string> ParseActionList(const std::string& aActionList);

    protected:
        std::string                                 mName;
        std::vector<std::shared_ptr<const Action>>  mActions;
        std::vector<std::string>                    mUIs;
        LuaIntf::LuaState                           mLuaState;
    };
}
