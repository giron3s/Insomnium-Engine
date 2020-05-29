/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/


#pragma once

#include "core/utils.h"
#include "core/lua.h"

namespace Framework
{
    class Script
    {
        NON_COPYABLE_CLASS( Script );

    public:
                            Script() {}
        virtual             ~Script() {}

        void                Initialize(const std::string& aFile);
        void                DeInitialize();

        bool                Run();
        bool                RunString(const std::string& aScriptCode);
        LuaIntf::LuaRef     GetReference(const std::string& aReferenceName);
        LuaIntf::LuaContext& GetLuaContext() { return mLuaContext; }
        void                RunFunction(const std::string& aFunctionName);


    private:
        void                BindInterface();
        bool                SetLuaRequirePath(const std::string& aAbsolutePath);
        bool                ClarifyError(const std::string& aErrorMessage);

        // You cannot store a LuaIntf::LuaState for a long time! Those can only be temporary.
        // You can only store a LuaIntf::LuaContext. You can get a state from the context.
        // I learnt this the hard way.
        LuaIntf::LuaContext mLuaContext;

        std::string         mFile;
        std::string         mFileContents; // The contents of the main script file
    };
}
