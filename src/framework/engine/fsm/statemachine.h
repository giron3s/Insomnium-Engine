/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/
#pragma once

#include "state.h"

namespace Framework
{
    class State;

    class StateMachine
    {
    public:
        StateMachine();

        void              AddState(const std::string&              aStateName,
                                   const std::vector<std::string>& aActionList,
                                   const std::vector<std::string>& aUIList);

        void              BindInterface(LuaIntf::LuaState& aLuaState);

        void              Update();

        void              ExecuteAction(const std::string& aActionName) const;

        template <typename T>
        void ExecuteActionWithArgs(const string& aActionName, T args) const
        {
            if (!mCurrState)
            {
                WARNING("The current state is null! Can not start the action\n");
                return;
            }

            mCurrState->ExecuteActionWithArgs(aActionName, args);
        }

        void              ChangeStateByName(const std::string& aStateName);
        void              ChangeState(shared_ptr<State> aNewState);

    private:
        shared_ptr<State> FindState(const std::string& aStateName);

    protected:
        bool                                          mDirty;
        std::shared_ptr<State>                        mPrevState;
        std::shared_ptr<State>                        mCurrState;
        std::map<std::string, std::shared_ptr<State>> mStates;


        LuaIntf::LuaState                             mLuaState;
    };
}

