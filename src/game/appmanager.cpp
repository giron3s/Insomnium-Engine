/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "engine/script.h"
#include "engine/project.h"

#include "appmanager.h"
#include "engine/components/motioncmp.h"
#include "engine/gamecontroller.h"
#include "graphic/motion.h"

#include "core/timer.h"
#include "userconfig.h"
#include "graphic/gui/label.h"
#include "graphic/gui/vscrollpanel.h"
#include "graphic/gui/button.h"
#include "graphic/gui/theme.h"

using namespace Framework;
using namespace LuaIntf;

namespace Game
{
    AppManager::AppManager()
        : BaseManager(Utils::Demangling(typeid(AppManager).name()))
    {
        mRemoveButton = nullptr;
        mFloorPlanner = nullptr;
        mAutosaveTimer = nullptr;
        mSceneToRemove = "";
        mCatalogView = nullptr;
        mBtnToggleCatalogView = nullptr;
    }

    AppManager::~AppManager()
    {
        if (mRemoveButton)
        {
            delete mRemoveButton;
            mRemoveButton = nullptr;
        }

        if (mFloorPlanner)
        {
            delete mFloorPlanner;
            mFloorPlanner = nullptr;
        }

        if (mAutosaveTimer)
        {
            delete mAutosaveTimer;
            mAutosaveTimer = nullptr;
        }
    }

    void AppManager::Initialize()
    {
        LuaContext& lLuaContext = Engine::Instance()->Script().GetLuaContext();
        LuaState lLuaState = lLuaContext.state();

        LuaBinding(lLuaState).addFunction("GetAppManager", [this]() -> AppManager&
        {
            string lManagerName = Utils::Demangling(typeid(AppManager).name());
            return static_cast<AppManager&>(Engine::Instance()->Manager(lManagerName));
        });

        LuaBinding(lLuaState).beginClass<AppManager>("AppManager")
            .addFunction("ApplyUserConfig", &AppManager::ApplyUserConfig)
            .addFunction("CheckLogin", &AppManager::CheckLogin, LUA_ARGS(std::string, std::string))
            .addFunction("NewProject", &AppManager::NewProject, LUA_ARGS(std::string))
            .addFunction("LoadProject", &AppManager::LoadProject)
            .addFunction("SaveProject", &AppManager::SaveProject)
            .addFunction("SetProjectSaved", &AppManager::SetProjectSaved)
            .addFunction("SetProjectUnsaved", &AppManager::SetProjectUnsaved)
            .addFunction("AddScene", &AppManager::AddScene, LUA_ARGS(std::string))
            .addFunction("SetSceneToRemove", &AppManager::SetSceneToRemove, LUA_ARGS(std::string))
            .addFunction("GetSceneToRemove", &AppManager::GetSceneToRemove)
            .addFunction("RemoveScene", &AppManager::RemoveScene, LUA_ARGS(std::string))
            .addFunction("DeleteSceneItems", &AppManager::DeleteSceneItems)
            .addFunction("OnChangedSceneSelection", &AppManager::OnChangedSceneSelection)
            .addFunction("ToggleSceneCamera", &AppManager::ToggleSceneCamera)
            .addFunction("ToggleCatalogView", &AppManager::ToggleCatalogView)
            .addFunction("ZoomSceneCamera", &AppManager::ZoomSceneCamera, LUA_ARGS(float)/*zoom amount*/)
            .addFunction("DisplayLog", &AppManager::DisplayLog)
            .addFunction("QuitApp", &AppManager::QuitApp)
            .endClass();

        ApplyUserConfig();
    }

    void AppManager::ApplyUserConfig()
    {
        string lUserConfigName = Utils::Demangling(typeid(UserConfig).name());
        UserConfig& lUserConfig = static_cast<UserConfig&>(Engine::Instance()->Container(lUserConfigName));

        Logger::mLogLevel = static_cast<LogLevel>(lUserConfig.GetLogLevel());

        string lManagerName = Utils::Demangling(typeid(LanguageManager).name());
        LanguageManager& lLanguageManager = static_cast<LanguageManager&>(Engine::Instance()->Manager(lManagerName));
        lLanguageManager.SetLanguage(lUserConfig.GetLanguage());

        if (mAutosaveTimer)
        {
            delete mAutosaveTimer;
            mAutosaveTimer = nullptr;
        }

        if (lUserConfig.GetAutosaveEnabled())
        {
            mAutosaveTimer = new Timer();

            function<void(void)> lAutosaveFunction;
            lAutosaveFunction = [this](void) { OnAutosaveEvent(); };
            mAutosaveTimer->OnIteration.Connect(lAutosaveFunction);

            mAutosaveTimer->Start(lUserConfig.GetAutosaveTime() / 1000.0f, 0);
        }
    }

    bool AppManager::CheckLogin(const std::string aUserName, const std::string aPassword)
    {
        if (aUserName.compare("admin") || aPassword.compare("admin"))
            return false;
        return true;
    }

    std::string AppManager::NewProject(const std::string aProjectName)
    {
        if (aProjectName.compare("") == 0)
            return "@ERROR_PROJECT_NAME_EMPTY";

        for (std::string::const_iterator s = aProjectName.begin(); s != aProjectName.end(); s++)
            if (!isalnum(*s) && !isspace(*s))
                return "@ERROR_PROJECT_NAME_FORMAT";

        //  If default project is not added to project list, then add it now
        std::string lDefaultProjectFile = "data/resources/projects/default.project";
        if (!(Engine::Instance()->ResourceManager().GetProjectByFile(lDefaultProjectFile)))
            Engine::Instance()->ResourceManager().AddProject(lDefaultProjectFile);

        mFloorPlanner->LoadProject(lDefaultProjectFile);
        mFloorPlanner->GetCurrentProject()->SetName(aProjectName);
        mFloorPlanner->GetCurrentProject()->SetSample(true);
        mFloorPlanner->LoadSceneList(0);
        return "Success";
    }

    bool AppManager::LoadProject()
    {
        char buf[1024];
        GetCurrentDirectoryA(256, buf);
        std::string lFileName = file_dialog({{"sh3d", "Sweet Home Project files"}}, false);
        SetCurrentDirectoryA(buf);

        if (!lFileName.empty())
        {
            if (mFloorPlanner == NULL)
                mFloorPlanner = (Game::UIFloorPlanner3D*)(Engine::Instance()->Display().findWidget("canvasGL"));
       
            Engine::Instance()->ResourceManager().AddProject(lFileName);
            mFloorPlanner->LoadProject(lFileName);
            mFloorPlanner->LoadSceneList(0);
        }

        return false;
    }

    void AppManager::UnloadProject()
    {
        if (mFloorPlanner == NULL)
            mFloorPlanner = (Game::UIFloorPlanner3D*)(Engine::Instance()->Display().findWidget("canvasGL"));

        shared_ptr<Framework::Project> lCurrentProject = mFloorPlanner->GetCurrentProject();
        if (lCurrentProject == nullptr)
        {
            CRASH("No project set as current.");
            return;
        }
            
        mFloorPlanner->GetCurrentProject()->Unload();
    }

    void AppManager::SaveProject()
    {
        if (mFloorPlanner == NULL)
            mFloorPlanner = (Game::UIFloorPlanner3D*)(Engine::Instance()->Display().findWidget("canvasGL"));

        shared_ptr<Framework::Project> lCurrentProject = mFloorPlanner->GetCurrentProject();
        if (lCurrentProject == nullptr)
        {
            CRASH("No project set as current.");
            return;
        }

        // Project file is exist. Save the file directly
        if (!lCurrentProject->GetFileName().empty() && !lCurrentProject->IsSample())
        {
            lCurrentProject->Save();
        }
        // Project file does not exist. So open the file dialog to get new file name
        else
        {
            char buf[1024];
            GetCurrentDirectoryA(256, buf);
            std::string lFileName = file_dialog({ {"sh3d", "Sweet Home Project files"}, {"*", "All files"} }, true);
            SetCurrentDirectoryA(buf);

            if (!lFileName.empty())
            {
                //  The file name does not include file extension. So add it here.
                if (lFileName.find(".sh3d") == std::string::npos)
                    lCurrentProject->SetFileName(lFileName + ".sh3d");
                else
                    lCurrentProject->SetFileName(lFileName);

                lCurrentProject->SetSample(false);
                lCurrentProject->Save();
            }   
        }
    }

    void AppManager::SetProjectSaved()
    {
        if (mFloorPlanner == NULL)
            mFloorPlanner = (Game::UIFloorPlanner3D*)(Engine::Instance()->Display().findWidget("canvasGL"));

        if (mFloorPlanner)
            mFloorPlanner->SetProjectSaved(true);
    }

    void AppManager::SetProjectUnsaved()
    {
        if (mFloorPlanner == NULL)
            mFloorPlanner = (Game::UIFloorPlanner3D*)(Engine::Instance()->Display().findWidget("canvasGL"));

        if (mFloorPlanner)
            mFloorPlanner->SetProjectSaved(false);
    }

    void AppManager::OnAutosaveEvent()
    {
        if (mFloorPlanner == NULL)
            mFloorPlanner = (Game::UIFloorPlanner3D*)(Engine::Instance()->Display().findWidget("canvasGL"));

        shared_ptr<Framework::Project> lCurrentProject = mFloorPlanner->GetCurrentProject();
        if (lCurrentProject == nullptr)
            return;

       
        lCurrentProject->SetSample(false);
        lCurrentProject->Save("./data/resources/projects/autosave.project");
    }

    std::string AppManager::AddScene(const std::string aSceneName)
    {
        if (aSceneName.compare("") == 0)
            return "@ERROR_FLOOR_NAME_EMPTY";

        for (std::string::const_iterator s = aSceneName.begin(); s != aSceneName.end(); s++)
            if (!isalnum(*s) && !isspace(*s))
                return "@ERROR_FLOOR_NAME_FORMAT";

        if (mFloorPlanner == NULL)
            mFloorPlanner = (Game::UIFloorPlanner3D*)(Engine::Instance()->Display().findWidget("canvasGL"));

        shared_ptr<Framework::Project> lCurrentProject = mFloorPlanner->GetCurrentProject();
        if (lCurrentProject == nullptr)
        {
            CRASH("No project set as current.");
            return "@ERROR_FLOOR_UNSPECIFIED";
        }

        if (mFloorPlanner->GetCurrentProject()->GetSceneByName(aSceneName))
            return "@ERROR_FLOOR_NAME_EXISTS";

        bool lSuccess = mFloorPlanner->GetCurrentProject()->AddScene(aSceneName, "./data/resources/projects/default.scene");
        if (!lSuccess)
            return "@ERROR_FLOOR_UNSPECIFIED";

        mFloorPlanner->SetProjectSaved(false);
        mFloorPlanner->LoadSceneList(aSceneName);
        return "Success";
    }

    void AppManager::RemoveScene(std::string aSceneName)
    {
        if (mFloorPlanner == NULL)
            mFloorPlanner = (Game::UIFloorPlanner3D*)(Engine::Instance()->Display().findWidget("canvasGL"));

        shared_ptr<Framework::Project> lCurrentProject = mFloorPlanner->GetCurrentProject();
        if (lCurrentProject == nullptr)
        {
            CRASH("No project set as current.");
            return;
        }

        std::string lCurrentSceneName = lCurrentProject->GetActiveScene()->GetName();
        lCurrentProject->RemoveScene(aSceneName);

        //  If current scene was removed, then load the first scene in the project.
        //  Otherwise keep the current scene active.
        if (lCurrentSceneName.compare(aSceneName) == 0)
            mFloorPlanner->LoadSceneList(0);
        else
            mFloorPlanner->LoadSceneList(lCurrentSceneName);
    }

    void AppManager::ToggleSceneCamera()
    {
        if (mFloorPlanner == NULL)
            mFloorPlanner = (Game::UIFloorPlanner3D*)(Engine::Instance()->Display().findWidget("canvasGL"));

        mFloorPlanner->getScene()->ToggleCamera();
    }

    void AppManager::DeleteSceneItems()
    {
        if (mFloorPlanner == NULL)
            mFloorPlanner = (Game::UIFloorPlanner3D*)(Engine::Instance()->Display().findWidget("canvasGL"));

        mFloorPlanner->getScene()->DeleteSelectedModels();
    }

    void AppManager::OnChangedSceneSelection()
    {
        // Button will be searched and initialized only once at the first call to this function.
        // This has been added here instead of initialize because when initialize is called the button
        // has not been created yet and findWidget returns NULL

        if (mFloorPlanner == NULL)
            mFloorPlanner = (Game::UIFloorPlanner3D*)(Engine::Instance()->Display().findWidget("canvasGL"));

        if (mRemoveButton == NULL)
            mRemoveButton = Engine::Instance()->Display().findWidget("btnRemove");

        std::shared_ptr<Framework::Scene> lScene = mFloorPlanner->getScene();
        if (mRemoveButton)
        {
            if (lScene->mSelectedModels.size() > 0)
                mRemoveButton->setEnabled(true);
            else
                mRemoveButton->setEnabled(false);
        }
    }

    void AppManager::ZoomSceneCamera(float aAmount)
    {
        if (mFloorPlanner == NULL)
            mFloorPlanner = (Game::UIFloorPlanner3D*)(Engine::Instance()->Display().findWidget("canvasGL"));

        auto lCamera = mFloorPlanner->getScene()->GetActiveCamera();
        auto lCameraEnt = Engine::Instance()->EntityManager().GetEntityByID(lCamera->GetEntityID());
        auto lMotionCmp = lCameraEnt->GetComponent<MotionCmp>().lock();
        ASSERT(lMotionCmp);
        lMotionCmp->GetCameraMotion()->Forward(aAmount);
    }

    void AppManager::DisplayLog()
    {
        UIWidget* lLogDisplay = (UIWidget*)(Engine::Instance()->Display().findWidget("LogDisplay"));
        if (lLogDisplay == nullptr)
        {
            WARNING("Cannot get log display widget named LogDisplay");
            return;
        }
        
        UILabel* lLogEntryLabel;
        vector<string> lLogEntries = Logger::GetLogEntires();
        UIWidget* lLogDisplayWidget = static_cast<UIWidget*>(lLogDisplay);
        for (string lEntry : lLogEntries)
        {
            lLogEntryLabel = new UILabel(lLogDisplayWidget, lEntry, false, "sans", -1);
            lLogEntryLabel->setFixedSize(glm::ivec2(700, 30));
            lLogDisplay->addChild(lLogEntryLabel);

            //Set the log text color
            Color lLogColor;
            if (lEntry.find("WARN:") != string::npos)
                lLogColor = Color(200, 200, 0, 255);
            else if (lEntry.find("CRASH:") != string::npos)
                lLogColor = Color(255, 0, 0, 255);
            else
                lLogColor = Color(255, 255, 255, 255);
            lLogEntryLabel->setTextColor(lLogColor);
        }
        
        Engine::Instance()->Display().performLayout();
    }

    void AppManager::QuitApp()
    {
        Engine::Instance()->GameController().OnQuit();
    }

    void AppManager::ToggleCatalogView()
    {
        if (!mCatalogView)
        {
            mCatalogView = (UICatalogView*)(Engine::Instance()->Display().findWidget("catalogView"));
            if (!mCatalogView)
            {
                WARNING("Cannot find widget catalogView");
                return;
            }
        }
        
        if (!mBtnToggleCatalogView)
        {
            mBtnToggleCatalogView = (UIButton*)(Engine::Instance()->Display().findWidget("btnToggleCatalogView"));
            if (!mBtnToggleCatalogView) 
            {
                WARNING("Cannot find widget btnToggleCatalogView");
                return;
            }
        }

        mCatalogView->setCollapse(!mBtnToggleCatalogView->pushed());
        Engine::Instance()->Display().performLayout();
    }
}
