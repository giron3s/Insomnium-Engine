/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2020 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once
#include "CmpManagerRegistry.h"

namespace Framework
{
    class BaseCmp;

    enum MsgType
    {
        eMSG_INVALID
        , eMSG_ENTITY_CREATED
        , COUNT
    };

    struct Msg
    {
        MsgType mMsgType;

        Msg() : mMsgType(eMSG_INVALID) { }
        Msg(MsgType aMsgType) : mMsgType(aMsgType) { }
    };

    

    struct IFunctorBase
    {
        virtual ~IFunctorBase() { }
        virtual void Execute(BaseCmp* aBaseCmp, const Msg& aMsg) = 0;
    };

    template< class Obj >
    struct Functor : public IFunctorBase
    {
        std::function<void (Obj*, const Msg&)> mCallback;

        // The cunstuctor that save the callback
        Functor(std::function<void(Obj*, const Msg&)> aCallback) : mCallback(aCallback)
        {

        }

        //The virtual implementation, where convert the BaseCmp to the template and call the recieve message function with the message as a argument
        void Execute(BaseCmp* aBaseCmp, const Msg& aMsg)
        {
            Obj* lObjOfMyType = static_cast<Obj*>(aBaseCmp);
            if (mCallback)
                mCallback(lObjOfMyType, aMsg);
        }
    };

    // The information stored for each msg registered
    struct ComponentCallbackInfo
    {
        ComponentID   mComponentId;     // component id registered
        IFunctorBase* mMethod;          // method of the class to be called
    };
};
