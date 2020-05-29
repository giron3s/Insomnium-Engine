/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*  Brief       : Represents a single project and acts as a container for multiple the scenes/floors 
*                present in the project. Projects are saved in .sh3d files which will save all the 
*                project properties and all the scenes with all elements contained in those scenes.
*                This means that the .sh3d file will contain the complete scene data as well.
*******************************************************************************/

#pragma once
#include <vector>
#include <map>
#include <list>
#include <string>

#include "json/json.h"

namespace Framework
{
    class Scene;

    class Project
    {
    public:

        Project(string aFileName = "", bool aSample = false)
            : mName("")
            , mFileName(aFileName)
            , mSample(aSample)
        {}

        std::string                 GetName() const { return mName; }
        void                        SetName(const std::string& aName) { mName = aName; }

        void                        SetFileName(const std::string& aFileName){ mFileName = aFileName; }
        std::string                 GetFileName() const { return mFileName; }

        bool                        Load();
        void                        Unload();
        void                        Save(std::string aFileNameOverride = "");

        void                        SetSample(const bool aSample) { mSample = aSample; }
        bool                        IsSample() const { return mSample; }

        std::shared_ptr<Scene>      GetActiveScene();
        void                        SetActiveScene(shared_ptr<Scene> aScene);

        bool                        AddScene(const string& aSceneName, const string& aSceneFile);
        bool                        AddScene(const Json::Value& aSerializer);
        bool                        RemoveScene(const string& aSceneName);
        std::shared_ptr<Scene>      GetSceneByName(const string& aSceneName);
        std::vector<std::string>    GetSceneList();
        void                        ClearScenes();

     protected:
        std::string                     mName;
        std::string                     mFileName;
        bool                            mSample;

        vector<std::shared_ptr<Scene>>  mScenes;
        std::shared_ptr<Scene>          mActiveScene;
    };
}

