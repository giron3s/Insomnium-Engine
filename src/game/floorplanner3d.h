/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

#include "graphic/gui/canvasgl.h"

namespace Framework
{
    namespace GUI
    {
        class UIDragDropData;
    }
}

namespace Game
{
    class Project;

    class UIFloorPlanner3D : public Framework::GUI::UICanvasGL
    {
    public:

             UIFloorPlanner3D();
             ~UIFloorPlanner3D();

        bool dropEvent(const glm::ivec2 &p, int button, int modifiers, Framework::GUI::UIDragDropData* aData) override;

        void performLayout(NVGcontext* aCtx) override;
        void draw(NVGcontext* aCtx) override;
        bool mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers) override;

        void Deserialize(const Json::Value& aSerializer) override;

        void SetProjectSaved(bool aSaved);

        void LoadSceneList(int aSelectedSceneIndex);
        void LoadSceneList(std::string aSelectedSceneName);

        std::shared_ptr<Framework::Project> GetCurrentProject();
        bool LoadProject(std::string aProjectFile);

    private:

        void AddNewEntityFromPrefab(const std::string& aPrefabName);
        void ChangeComboScene(int aIndex);
        void RemoveComboScene(int aIndex);
        bool LoadScene(std::string aSceneName);
        bool CustomRenderSort2D(Framework::Model2D* aModel1, Framework::Model2D* aModel2);

    protected:

        std::unique_ptr<UILabel>    mToggleViewGroupLbl;
        std::unique_ptr<UIButton>   mToggleView2DBtn;
        std::unique_ptr<UIButton>   mToggleView3DBtn;
                                    
        std::unique_ptr<UILabel>    mZoomGroupLbl;
        std::unique_ptr<UIButton>   mZoomInBtn;
        std::unique_ptr<UIButton>   mZoomOutBtn;

        UIWidget*                   mSaveButton;
        bool                        mProjectSaved;

        std::unique_ptr<UIComboBox> mSceneSelectCombo;
        int                         mSceneSelectComboIndex;

        std::shared_ptr<Framework::Project>    mCurrentProject;
    };
}
