/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "engine/project.h"
#include "graphic/scene.h"

#include "json/json.h"
#include "core/serialization/jsoncpputils.h"

namespace Framework
{
    void Project::Save(string aFileNameOverride)
    {
        if (mFileName.empty())
        {
            CRASH("The project filename is empty. Could not save the project file!");
            return;
        }

        string lFileName = mFileName;
        if (aFileNameOverride.compare(""))
            lFileName = aFileNameOverride;

        Json::Value lSerializer;
        lSerializer["projectName"] = mName;
        lSerializer["sample"] = mSample;
        Json::Value& v = lSerializer["scenes"];
        v = Json::Value(Json::ValueType::arrayValue);

        unsigned int i = 0;
        for (const auto lScene : mScenes)
        {
            lScene->Serialize(v[i++]);
        }

        Json::StyledWriter writer;
        string lOutput = writer.write(lSerializer);
        fstream lFileOut(lFileName, ios_base::out);
        lFileOut << lOutput;

        Engine::Instance()->StateMachine().ExecuteAction("SetProjectSaved");
    }


    bool Project::Load()
    {
        Json::Value lSerializer;
        bool lIsOk = Json::JsonUtils::OpenAndParseJsonFromFile(lSerializer, mFileName);

        if(!lIsOk)
        {
            WARNING("Cannot load '%s' file as a project!", mFileName.c_str())
            return false;
        }

        if (lSerializer.isMember("projectName"))
            mName = lSerializer["projectName"].asString();

        if (lSerializer.isMember("sample"))
            mSample = lSerializer["sample"].asBool();

        // Deserialize scenes
        const Json::Value& lScenes = lSerializer["scenes"];
        ASSERT(lScenes.isArray());
        for(size_t i = 0, lSize = lScenes.size(); i < lSize; ++i)
        {
            const Json::Value lSceneRes = lScenes[static_cast<int>(i)];
            AddScene(lSceneRes);
        }

        Engine::Instance()->StateMachine().ExecuteAction("SetProjectSaved");
        return true;
    }

    void Project::Unload()
    {
        for (auto& lScene : mScenes)
            lScene->Unload();
        mScenes.clear();

        // clear all stuff from entity manager
        Engine::Instance()->EntityManager().DeInitialize();
    }

    shared_ptr<Scene> Project::GetActiveScene()
    {
        return mActiveScene;
    }

    void Project::SetActiveScene(shared_ptr<Scene> aScene)
    {
        if(!aScene)
        {
            WARNING("Cannot set empty scene as active scene");
            return;
        }

        if (find(mScenes.begin(), mScenes.end(), aScene) == mScenes.end())
            CRASH("Scene being set as active is not part of the current project.");

        mActiveScene = aScene;
    }

    bool Project::AddScene(const string& aSceneName, const string& aSceneFile)
    {
        if (GetSceneByName(aSceneName))
        {
            WARNING("Scene '%s already exists and cannot be added again!", aSceneName.c_str());
            return false;
        }

        Json::Value lSerializer;
        bool lIsOk = Json::JsonUtils::OpenAndParseJsonFromFile(lSerializer, aSceneFile);
        if (!lIsOk)
        {
            WARNING("Cannot load scene from file %s", aSceneFile.c_str())
            return false;
        }

        auto lScene = std::make_shared<Scene>(aSceneName);
        lScene->Deserialize(lSerializer);
        mScenes.push_back(lScene);
        return true;
    }

    bool Project::AddScene(const Json::Value& aSerializer)
    {
        if (!aSerializer.isMember("sceneName"))
            CRASH("Scene has no 'sceneName' attribute!");

        string lName = aSerializer["sceneName"].asString();

        if (GetSceneByName(lName))
        {
            WARNING("Scene '%s already exists and cannot be added again!", lName.c_str());
            return false;
        }

        auto lScene = std::make_shared<Scene>(lName);
        lScene->Deserialize(aSerializer);
        mScenes.push_back(lScene);
        return true;
    }

    bool Project::RemoveScene(const string& aSceneName)
    {
        for (vector<std::shared_ptr<Scene>>::iterator lScene = mScenes.begin(); lScene != mScenes.end(); ++lScene)
        {
            if ((*lScene)->GetName().compare(aSceneName) == 0)
            {
                (*lScene)->Unload();
                mScenes.erase(lScene);
                Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
                return true;
            }
        }
        
        return false;
    }

    shared_ptr<Scene> Project::GetSceneByName(const string& aSceneName)
    {
        for (auto& lScene : mScenes)
            if (lScene->GetName().compare(aSceneName) == 0)
                return lScene;
        return std::shared_ptr<Scene>();
    }

    vector<string>  Project::GetSceneList()
    {
        std::vector<string> lSceneList;
        for (auto& lScene : mScenes)
        {
            lSceneList.push_back(lScene->GetName());
        }

        return lSceneList;
    }

    void Project::ClearScenes()
    {
        mScenes.clear();
    }
}
