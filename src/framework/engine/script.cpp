/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#include "precompiled.h"
#include "script.h"
#include "engine.h"

using namespace LuaIntf;

namespace Framework
{
    void Script::Initialize(const std::string& aFile)
    {
        mFile = aFile;
        mFileContents = Utils::GetFileContents(aFile);
        SetLuaRequirePath(aFile);
        BindInterface();

        try
        {
            LuaState luaState = mLuaContext.state();
            luaState.openLibs(); // Open all of Lua's basic libs, like io and math.
        }
        catch(const exception& e)
        {
            CRASH("Lua failed to load libraries. Error: %s\n", e.what());
        }
    }

    void Script::DeInitialize()
    {
        // Do nothing
    }

    // Will logprint a clarified error message and it's solutions if it was a know error
    // Returns true if there was a know error that was clarified
    bool Script::ClarifyError(const string& aErrorMessage)
    {
        // The error messages found here were found by trial and error
        // These are only guesses and might not be 100% acurate
        bool clarifiedError = false; // True if we clarified an error
        string clarifiedErrorMessage = "";

        // A function is returning an object that wasn't binded!
        if(aErrorMessage.find("bad argument #-2") != string::npos)
        {
            clarifiedError = true;
            clarifiedErrorMessage = "We suspect a function is returning an object of a non-binded class. If this is the case, this is a bug in this engine (!), please contact the developers.";
        }

        // A module fonction is being called using the ':' syntax instead of the '.' syntax!
        if(aErrorMessage.find("on bad self") != string::npos)
        {
            clarifiedError = true;
            clarifiedErrorMessage = "We suspect you are attempting to call a function that is not binded to an object using ':'. Please use the '.' syntax instead!";
        }

        if(clarifiedError)
        {
            WARNING("%s", clarifiedErrorMessage.c_str());
        }

        return clarifiedError;
    }

    // Lua will search in this path when using require().
    // Lua requires an absolute path!
    bool Script::SetLuaRequirePath(const string& aAbsolutePath)
    {
        try
        {
            LuaState luaState = mLuaContext.state();

            // From http://stackoverflow.com/questions/4125971/setting-the-global-lua-path-variable-from-c-c/4156038#4156038
            luaState.getGlobal("package");
            luaState.getField(-1, "path");

            string currentPath = luaState.toString(-1);
            currentPath.append(";");
            //currentPath.append(aAbsolutePath + "?.lua"); // ?.lua seems to be needed and works better for cross-platform

            luaState.pop(1);
            luaState.push(currentPath.c_str());
            luaState.setField(-2, "path");
            luaState.pop(1);
        }
        catch(const exception& e)
        {
            CRASH("Failed to set Lua's require path for script '%s'! Error: %s ", mFile.c_str(), e.what());
            return false;
        }

        return true;
    }

    // Binds all of the classes and functions. This creates our API!
    // We need a Game instance to give it to the scripts.
    void Script::BindInterface()
    {
        LuaState lLuaState = mLuaContext.state();
        Engine::Instance()->BindInterface(lLuaState);
        Engine::Instance()->StateMachine().BindInterface(lLuaState);
    }

    // Running a script will probably not be too heavy since it will probably only be defining a bunch of callbacks.
    // You should definitely call this after binding the interface.
    bool Script::Run()
    {
        try
        {
            mLuaContext.doString(mFileContents.c_str()); // Run the file!
        }
        catch(const exception& e)
        {
            string lScriptErrorMessage = e.what();
            ClarifyError(lScriptErrorMessage);
            CRASH( "Script '%s' failed!\nError: %s\n ", mFile.c_str(), lScriptErrorMessage.c_str());
            return false;
        }
        return true;
    }

    // Run a string of script code directly on this script
    bool Script::RunString(const string& aScriptCode)
    {
        try
        {
            mLuaContext.doString(aScriptCode.c_str()); // Run the file!
        }
        catch(const exception& e)
        {
            string lScriptErrorMessage = e.what();
            ClarifyError(lScriptErrorMessage);
            CRASH( "Running string on script '%s' failed!\nError: %s\n ", mFile.c_str(), lScriptErrorMessage.c_str());
            return false;
        }

        return true;
    }

    // Gets a reference from the script (for example, a variable or a function)
    LuaRef Script::GetReference(const string& aReferenceName)
    {
        LuaState luaState = mLuaContext.state();
        return LuaRef(luaState, aReferenceName.c_str());
    }

    void Script::RunFunction(const string& aFunctionName)
    {
        try
        {
            GetReference(aFunctionName)();
        }
        catch(const exception& e)
        {
            string lScriptErrorMessage = e.what();
            ClarifyError(lScriptErrorMessage);
            CRASH( "Error occurred during running '%s'function from C++ failed!\nError: %s\n ", aFunctionName.c_str(), lScriptErrorMessage.c_str());
        }
    }
}
