/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once

#include "core/utils.h"
#include "core/serialization/jsoncpputils.h"
#include "engine/basemanager.h"
#include "graphic/scene.h"
#include "floorplanner3d.h"
#include "catalogview.h"

namespace Game
{
    class Framework::Timer;

    class AppManager : public Framework::BaseManager
    {
    public:
                            AppManager();
                            ~AppManager() override;

        void                Initialize() override;
        void                DeInitialize() override { };

        void                ApplyUserConfig();

        bool                CheckLogin(const std::string aUserName, const std::string aPassword);
        std::string         NewProject(const std::string aProjectName);
        bool                LoadProject();
        void                UnloadProject();
        void                SaveProject();
        void                SetProjectSaved();
        void                SetProjectUnsaved();

        void                OnAutosaveEvent();

        std::string         AddScene(const std::string aSceneName);
        void                SetSceneToRemove(std::string aSceneToRemove) { mSceneToRemove = aSceneToRemove; }
        std::string         GetSceneToRemove() { return mSceneToRemove; }
        void                RemoveScene(std::string aSceneName);
        
        void                ToggleSceneCamera();
        void                ToggleCatalogView();
        void                ZoomSceneCamera(float aAmount);
        void                DeleteSceneItems();
        void                OnChangedSceneSelection();

        void                DisplayLog();
        void                QuitApp();

    private:

        Framework::GUI::UIWidget*           mRemoveButton;
        Game::UIFloorPlanner3D*             mFloorPlanner;
        std::string                         mSceneToRemove;
        Framework::Timer*                   mAutosaveTimer;
        UICatalogView*                      mCatalogView;
        UIButton*                           mBtnToggleCatalogView;
    };
}
