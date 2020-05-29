/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "engine/fsm/state.h"
#include "engine/fsm/action.h"
#include "engine/fsm/statemachine.h"
#include "graphic/display.h"


using namespace LuaIntf;

namespace Framework
{

    StateMachine::StateMachine()
        : mDirty(false)
    {

    }

    void StateMachine::BindInterface(LuaState& aLuaState)
    {
        mLuaState = aLuaState;
        LuaBinding(aLuaState).beginClass<StateMachine>("StateMachine")
            .addFunction("AddState",      &StateMachine::AddState)
            .addFunction("ChangeState",   &StateMachine::ChangeStateByName, LUA_ARGS(string))
            .addFunction("ExecuteAction", &StateMachine::ExecuteAction,     LUA_ARGS(string))
        .endClass();
    }


    void StateMachine::Update()
    {
        PROFILER_EVENT();

        if (!mDirty && mCurrState)
            mCurrState->ExecuteAction(Action::UPDATE);

        //Transaction between state
        else if(mDirty)
        {
            mDirty = false;
            if (mPrevState != nullptr)
            {
                for(auto lUIIter : mPrevState->GetUIs())
                {
                    Engine::Instance()->Display().UnloadUI(lUIIter);
                }
            }
            //OnStart new action
            if (mCurrState != nullptr)
            {
                for(auto lUIIter : mCurrState->GetUIs())
                {
                    Engine::Instance()->Display().LoadUI(lUIIter);
                }
                mCurrState->ExecuteAction(Action::START);
            }
        }
    }


    void StateMachine::AddState(const string&         aStateName,
                                const vector<string>& aActionList,
                                const vector<string>& aUIList)
    {
        shared_ptr<State> aNewState = FindState(aStateName);
        if(aNewState != nullptr)
        {
            CRASH("The '%s' state is exist! Cannot create the state with same name!", aStateName.c_str());
        }

        shared_ptr<State> lState( new State(aStateName, aActionList, aUIList, mLuaState ));
        pair< string, shared_ptr<State> > lStatePair(aStateName, lState);
        mStates.insert(lStatePair);
    }


    void StateMachine::ChangeStateByName(const string& aStateName)
    {
        shared_ptr<State> aNewState = FindState(aStateName);
        if(aNewState == nullptr)
        {
            CRASH("The '%s' state not exist! Cannot set the state!", aStateName.c_str());
        }

        ChangeState(aNewState);
    }


    void StateMachine::ChangeState(shared_ptr<State> aNewState)
    {
        mPrevState = mCurrState;
        mCurrState = aNewState;
        mDirty = true;

        //Launch the exit action
        if (mPrevState)
            mPrevState->ExecuteAction(Action::EXIT);
    }


    void StateMachine::ExecuteAction(const string& aActionName) const
    {
        if (!mCurrState)
        {
            WARNING("The current state is null! Can not start the action\n");
            return;
        }

        mCurrState->ExecuteAction(aActionName);
    }

    


    shared_ptr<State> StateMachine::FindState(const string& aStateName)
    {
        map<string, shared_ptr<State>>::iterator lIter = mStates.find(aStateName);
        if(lIter == mStates.end())
        {
            return nullptr;
        }
        return lIter->second;
    }
}
