/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/
#include "precompiled.h"
#include "engine.h"
#include "catalogitem.h"
#include "floorplanner3d.h"

#include "engine/project.h"
#include "engine/components/cameracmp.h"
#include "engine/components/renderablecmp.h"
#include "engine/components/directlightcmp.h"
#include "engine/components/pointlightcmp.h"
#include "engine/components/spotlightcmp.h"
#include "engine/components/transformcmp.h"
#include "engine/components/motioncmp.h"
#include "engine/components/catalogcmp.h"

#include "graphic/gui/dragdropdata.h"
#include "graphic/gui/button.h"
#include "graphic/gui/label.h"
#include "graphic/gui/slider.h"
#include "graphic/gui/theme.h"
#include "graphic/gui/buttontype.h"
#include "graphic/boundingsphere.h"
#include "graphic/scene.h"
#include "graphic/PanAndZoomMotion.h"
#include "graphic/gui/combobox.h"

#include "glm/gtx/rotate_vector.hpp"

using namespace Framework;
using namespace Framework::GUI;
using namespace std::placeholders;

namespace Game
{

    constexpr float ADD_NEW_ENTITY_DISTANCE = 5.f;
    constexpr uint32_t eROOMSHAPE = Model::ModelFlags::eFIRST_CUSTOM_FLAG;

    UIFloorPlanner3D::UIFloorPlanner3D()
        : UICanvasGL()
        , mSceneSelectComboIndex(0)
        , mCurrentProject(nullptr)
        , mSaveButton(nullptr)
        , mProjectSaved(true)
    {
        // 2d/3d view
        mToggleViewGroupLbl = std::make_unique<UILabel>(this, "@SELECT_VIEW_MODE");
        mToggleView2DBtn = std::make_unique<UIButton>(this, "@2D", -1);
        mToggleView3DBtn = std::make_unique<UIButton>(this, "@3D", -1);

        mToggleViewGroupLbl->setFontSize(18);
        mToggleViewGroupLbl->setFixedSize({ 142, 60 });

        mToggleView2DBtn->setFlags(ButtonType::eRadioButton);
        mToggleView2DBtn->setFixedSize({ 35, 35 });
        mToggleView2DBtn->setLuaAction("ToggleSceneCamera");

        mToggleView3DBtn->setFlags(ButtonType::eRadioButton);
        mToggleView3DBtn->setFixedSize({ 35, 35 });
        mToggleView3DBtn->setLuaAction("ToggleSceneCamera");

        std::vector<UIButton*> lBtnGroup{ mToggleView2DBtn.get(), mToggleView3DBtn.get() };
        mToggleView2DBtn->setButtonGroup(lBtnGroup);
        mToggleView3DBtn->setButtonGroup(lBtnGroup);

        // default active view is 3D
        mToggleView3DBtn->setPushed(false);
        mToggleView3DBtn->setPushed(true);

        int lImageId = Engine::Instance()->ResourceManager().FindImage("ViewModeBtn");
        ASSERT(lImageId != -1);
        mToggleView2DBtn->setIcon(lImageId);
        mToggleView3DBtn->setIcon(lImageId);

        // zoom in/out
        mZoomGroupLbl = std::make_unique<UILabel>(this, "@ZOOM");
        mZoomInBtn = std::make_unique<UIButton>(this, "@PLUS_SIGN", -1);
        mZoomOutBtn = std::make_unique<UIButton>(this, "@MINUS_SIGN", -1);
        mZoomGroupLbl->setFixedSize({ 45, 5 + 35 + 35 + 5 });

        mZoomInBtn->setFixedSize({ 35, 35 });
        ASSERT(lImageId != -1);
        mZoomInBtn->setIcon(lImageId);
        mZoomInBtn->setLuaAction("ZoomInSceneCamera");

        mZoomOutBtn->setFixedSize({ 35, 35 });
        ASSERT(lImageId != -1);
        mZoomOutBtn->setIcon(lImageId);
        mZoomOutBtn->setLuaAction("ZoomOutSceneCamera");

        auto lSort2DFunc = std::bind(&UIFloorPlanner3D::CustomRenderSort2D, this, _1, _2);
        Renderer* lRenderer = Engine::Instance()->Display().GetRenderer();
        lRenderer->SetRenderSort2D(lSort2DFunc);

        SetProjectSaved(true);
    }


    UIFloorPlanner3D::~UIFloorPlanner3D()
    {
        if (mToggleViewGroupLbl)
            mToggleViewGroupLbl.release();

        if (mToggleView2DBtn)
            mToggleView2DBtn.release();

        if (mToggleView3DBtn)
            mToggleView3DBtn.release();

        if (mZoomGroupLbl)
            mZoomGroupLbl.release();

        if (mZoomInBtn)
            mZoomInBtn.release();

        if (mZoomOutBtn)
            mZoomOutBtn.release();

        if (mSceneSelectCombo)
            mSceneSelectCombo.release();
    }

    void UIFloorPlanner3D::performLayout(NVGcontext* aCtx)
    {
        UICanvasGL::performLayout(aCtx);
        //Toogle Label
        glm::ivec2 lPos = glm::ivec2(mSize.x / 2.f - mToggleViewGroupLbl->fixedSize().x / 2.f, mSize.y - mToggleViewGroupLbl->fixedSize().y);
        mToggleViewGroupLbl->setPosition(mPos + lPos);
        mToggleViewGroupLbl->performLayout(aCtx);

        //Toogle View 2D Button
        lPos = glm::ivec2(mSize.x / 2.f - mToggleView2DBtn->fixedSize().x - 5/*gap between buttons*/, mSize.y - mToggleView2DBtn->fixedSize().y);
        mToggleView2DBtn->setPosition(mPos + lPos);
        mToggleView2DBtn->performLayout(aCtx);

        //Toogle View 3D Button
        lPos = glm::ivec2(mSize.x / 2.f + 5 /*gap between buttons*/, mSize.y - mToggleView3DBtn->fixedSize().y);
        mToggleView3DBtn->setPosition(mPos + lPos);
        mToggleView3DBtn->performLayout(aCtx);

        mToggleViewGroupLbl->setTextColor(Color(0.0f, 1.0f));
        mToggleViewGroupLbl->setBackgroundColor(Color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
        mToggleView2DBtn->setTextColor(Color(0.0f, 1.0f));
        mToggleView2DBtn->setBackgroundColor(Color(1.0f, 1.0f));
        mToggleView3DBtn->setTextColor(Color(0.0f, 1.0f));
        mToggleView3DBtn->setBackgroundColor(Color(1.0f, 1.0f));

        /* make a copy of theme assigned for 2d view button,
        modify the copy and assign as new theme for 2d and 3d buttons */
        {
            Framework::GUI::ref<UITheme> lTheme(new UITheme(*mToggleView2DBtn->theme()));
            lTheme->mBorderLight.a() = 0.0f;
            lTheme->mBorderDark.a() = 0.0f;
            mToggleView2DBtn->setTheme(lTheme);
            mToggleView3DBtn->setTheme(lTheme);
        }

        // zoom in/out controls
        lPos = glm::ivec2(0, mSize.y / 2.f - mZoomGroupLbl->fixedSize().y / 2.f);
        mZoomGroupLbl->setPosition(mPos + lPos);
        mZoomGroupLbl->performLayout(aCtx);

        lPos = glm::ivec2(mZoomGroupLbl->position() + glm::ivec2(5, 5));
        mZoomInBtn->setPosition(lPos);
        mZoomInBtn->performLayout(aCtx);

        lPos = glm::ivec2(mZoomInBtn->position() + glm::ivec2(0, mZoomInBtn->fixedSize().y));
        mZoomOutBtn->setPosition(lPos);
        mZoomOutBtn->performLayout(aCtx);

        mZoomGroupLbl->setTextColor(Color(1.0f, 1.0f));
        mZoomGroupLbl->setBackgroundColor(Color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
        mZoomInBtn->setTextColor(Color(0.0f, 1.0f));
        mZoomInBtn->setBackgroundColor(Color(1.0f, 1.0f));
        mZoomOutBtn->setTextColor(Color(0.0f, 1.0f));
        mZoomOutBtn->setBackgroundColor(Color(1.0f, 1.0f));

        /* make a copy of theme assigned for zoom out button,
        modify the copy and assign as new theme for zoom in and zoom out buttons */
        {
            Framework::GUI::ref<UITheme> lTheme(new UITheme(*mZoomInBtn->theme()));
            lTheme->mBorderLight.a() = 0.0f;
            lTheme->mBorderDark.a() = 0.0f;
            mZoomInBtn->setTheme(lTheme);
            mZoomOutBtn->setTheme(lTheme);
        }

        //Render to the top left position
        mSceneSelectCombo->setPosition(mPos);
        mSceneSelectCombo->performLayout(aCtx);
    }

    void UIFloorPlanner3D::draw(NVGcontext* aCtx)
    {
        UICanvasGL::draw(aCtx);

        mToggleViewGroupLbl->draw(aCtx);
        mToggleView2DBtn->draw(aCtx);
        mToggleView3DBtn->draw(aCtx);

        mZoomGroupLbl->draw(aCtx);
        mZoomInBtn->draw(aCtx);
        mZoomOutBtn->draw(aCtx);

        mSceneSelectCombo->draw(aCtx);
    }

    bool UIFloorPlanner3D::mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers)
    {
        // this does not look good...
        if ( mToggleView2DBtn->contains(p) )
            return mToggleView2DBtn->mouseButtonEvent(p, button, down, modifiers);
        else if ( mToggleView3DBtn->contains(p) )
            return mToggleView3DBtn->mouseButtonEvent(p, button, down, modifiers);
        else if ( mZoomInBtn->contains(p) )
            return mZoomInBtn->mouseButtonEvent(p, button, down, modifiers);
        else if ( mZoomOutBtn->contains(p) )
            return mZoomOutBtn->mouseButtonEvent(p, button, down, modifiers);
        else if (mSceneSelectCombo->contains(p))
            return mSceneSelectCombo->mouseButtonEvent(p, button, down, modifiers);

        return UICanvasGL::mouseButtonEvent(p, button, down, modifiers);
    }

    bool UIFloorPlanner3D::dropEvent(const glm::ivec2 &p, int button, int modifiers, UIDragDropData* aData)
    {
        if ( aData->HasData<CatalogItem*>() )
        {
            auto lDraggedItem = aData->GetData<CatalogItem*>();
            INFO(LogLevel::eLEVEL2, "Dragged item: Name: %s\n", lDraggedItem->GetName().c_str());
            AddNewEntityFromPrefab(lDraggedItem->GetPrefabFile());
        }
        return true;
    }

    void UIFloorPlanner3D::AddNewEntityFromPrefab(const string& aPrefabName)
    {
        weak_ptr<Framework::Entity> lEntity = Engine::Instance()->EntityManager().CreateEntityFromPrefab(aPrefabName);

        Model2D* lModel2D = nullptr;
        Model3D* lModel3D = nullptr;
        uint32_t* lModelFlags = nullptr;
        if ( lEntity.lock()->HasComponent<CameraCmp>() )
        {
            weak_ptr<CameraCmp> lCamera = lEntity.lock()->GetComponent<CameraCmp>();
            mScene->Add(lCamera.lock()->GetCamera());
        }
        else if ( lEntity.lock()->HasComponent<RenderableCmp>() )
        {
            weak_ptr<RenderableCmp> lRenderable = lEntity.lock()->GetComponent<RenderableCmp>();
            lModel2D = lRenderable.lock()->GetModel2D();
            lModel3D = lRenderable.lock()->GetModel3D();

            if(lModel2D)
                mScene->Add(lModel2D);

            if(lModel3D)
            {
                mScene->Add(lModel3D);
                lModelFlags = &lModel3D->mFlags;
            }
        }
        else if ( lEntity.lock()->HasComponent<DirectLightCmp>() )
        {
            weak_ptr<DirectLightCmp> lDirectLight = lEntity.lock()->GetComponent<DirectLightCmp>();
            mScene->Add(lDirectLight.lock()->GetDirectLight());
        }
        else if ( lEntity.lock()->HasComponent<SpotLightCmp>() )
        {
            weak_ptr<SpotLightCmp> lSpotLight = lEntity.lock()->GetComponent<SpotLightCmp>();
            mScene->Add(lSpotLight.lock()->GetSpotLight());
        }
        else if ( lEntity.lock()->HasComponent<PointLightCmp>() )
        {
            weak_ptr<PointLightCmp> lPointLight = lEntity.lock()->GetComponent<PointLightCmp>();
            mScene->Add(lPointLight.lock()->GetPointLight());
        }

        weak_ptr<RenderableCmp> lRenderable = lEntity.lock()->GetComponent<RenderableCmp>();

        const BoundingSphere& lBoundingSphere = lRenderable.lock()->GetModel3D()->GetBoundingSphere();
        float lRadius = lBoundingSphere.GetRadius() * ADD_NEW_ENTITY_DISTANCE;
        glm::vec3 lPosition = mScene->GetActiveCamera()->GetPosition() + mScene->GetActiveCamera()->GetDirection() * lRadius;
        if ( lModelFlags && (*lModelFlags & eROOMSHAPE) )
        {
            ASSERT(lModel3D && lModel2D);
            *lModelFlags |= Model::ModelFlags::eDISABLE_TRASFORM_3D;
            if ( mScene->GetActiveCamera()->GetProjectionType() == Projection::PERSPECTIVE )
                lPosition.y = -1.0e-6f;
            else // ORTHOGRAPHIC
                lPosition.z = -1.0e-6f;
            static int sRoomNumber = 1;
            lModel2D->mText = std::string("room") + std::to_string(sRoomNumber++);
            lModel2D->mText.append("\n17.5 m2");
        }

        if ( mScene->GetActiveCamera()->GetProjectionType() == Projection::PERSPECTIVE )
        {
            if ( lModel3D ) // set position of Model3D if there is Model3D
                lModel3D->SetPosition(lPosition);
        }
        else // ORTHOGRAPHIC
        {
            if ( lModel2D ) // set position of Model2D if there is Model2D
                lModel2D->SetPosition(lPosition);
            if ( lModel3D ) // if there is Model3D...
            { // set proper orientation and scale of Model2D taken from Model3D (see ToggleCamera)
                ASSERT(lModel3D->mBuddy == lModel2D);
                // rotation about Y in 3D view -> rotation about Z in 2D view
                float a = atan2(-lModel3D->GetOrientation()[0].z, lModel3D->GetOrientation()[0].x);
                glm::mat4 lOrientation = glm::mat4(1.0);
                lOrientation[0] = glm::rotateZ(glm::vec4(1, 0, 0, 0), a);
                lOrientation[1] = glm::rotateZ(glm::vec4(0, 1, 0, 0), a);
                lModel2D->SetOrientation(lOrientation);
                // scale in 3D view = scale in 2D view
                lModel2D->SetScaleFactor(lModel3D->GetScaleFactor());
            }
        }

        if ( lModelFlags && (*lModelFlags & eROOMSHAPE) &&
             mScene->GetActiveCamera()->GetProjectionType() == Projection::PERSPECTIVE )
        {
            mScene->ToggleCamera();
            mToggleView2DBtn->setPushed(true);
            mToggleView3DBtn->setPushed(false);
        }
    }

    void UIFloorPlanner3D::SetProjectSaved(bool aSaved)
    {
        mProjectSaved = aSaved;

        if (mSaveButton == NULL)
            mSaveButton = Engine::Instance()->Display().findWidget("btnSave");

        if (mSaveButton)
        {
            if (mProjectSaved)
                mSaveButton->setEnabled(false);
            else
                mSaveButton->setEnabled(true);
        }
    }

    bool UIFloorPlanner3D::LoadScene(string aSceneName)
    {
        shared_ptr<Scene> lNewScene = GetCurrentProject()->GetSceneByName(aSceneName);

        //  The current scene is the same as selected scene
        if (mScene == lNewScene)
        {
            WARNING("The '%s' scene is loaded already!", aSceneName.c_str());
            return false;
        }

        if (lNewScene == nullptr)
        {
            CRASH("No scene found with name '%s'!", aSceneName.c_str());
            return false;
        }

        mScene = lNewScene;
        GetCurrentProject()->SetActiveScene(mScene);
        Engine::Instance()->Display().performLayout();
        Engine::Instance()->StateMachine().ExecuteAction("OnChangedSceneSelection");
        return true;
    }

    void UIFloorPlanner3D::ChangeComboScene(int aSelectedSceneIndex)
    {
        ASSERT(aSelectedSceneIndex < mSceneSelectCombo->items().size());

        string lSelectedSceneName = mSceneSelectCombo->items()[aSelectedSceneIndex];
        //Add the new floor
        if (lSelectedSceneName.compare("Add Floor") == 0)
            Engine::Instance()->StateMachine().ExecuteAction("GotoAddScene");
        //Load the scene
        else 
            LoadScene(lSelectedSceneName);

        mSceneSelectComboIndex = aSelectedSceneIndex;
        mSceneSelectCombo->setSelectedIndex(mSceneSelectComboIndex);
        Engine::Instance()->Display().performLayout();
    }

    void UIFloorPlanner3D::RemoveComboScene(int aSelectedSceneIndex)
    {
        ASSERT(aSelectedSceneIndex < mSceneSelectCombo->items().size());

        if (mSceneSelectCombo->items().size() == 2)
        {
            WARNING("Cannot remove the last scene from the project!");
            return;
        }

        string lSelectedSceneName = mSceneSelectCombo->items()[aSelectedSceneIndex];
        Engine::Instance()->StateMachine().ExecuteActionWithArgs("GotoRemoveScene", lSelectedSceneName);
    }

    void UIFloorPlanner3D::LoadSceneList(int aSelectedSceneIndex)
    {
        vector<string> lSceneNameList = GetCurrentProject()->GetSceneList();    //Scene name
        vector<int> lSceneIconsList (lSceneNameList.size(), 61795);             //Scene icons
        
        //  Add floor options
        lSceneNameList.push_back("Add Floor");
        lSceneIconsList.push_back(61978);

        //  If we have only one floor remaining, then we should not show the remove button
        //  because the last scene in the project cannot be removed.
        if (lSceneNameList.size() > 2)
        {
            vector<bool> lRemovableScenes(lSceneIconsList.size()-1, true);
            lRemovableScenes.push_back(false);
            mSceneSelectCombo->setItems(lSceneNameList, lSceneNameList, lSceneIconsList, lRemovableScenes);
        }
        else
        {
            mSceneSelectCombo->setItems(lSceneNameList, lSceneNameList, lSceneIconsList);
        }

        ChangeComboScene(aSelectedSceneIndex);
    }

    void UIFloorPlanner3D::LoadSceneList(string aSelectedSceneName)
    {
        vector<string> lSceneList = GetCurrentProject()->GetSceneList();
        vector<string>::iterator lSceneIter = std::find(lSceneList.begin(), lSceneList.end(), aSelectedSceneName);

        if (lSceneIter == lSceneList.end())
            CRASH("No scene found with name '%s'!", aSelectedSceneName.c_str());

        int lSelectedSceneIndex = static_cast<int>(std::distance(lSceneList.begin(), lSceneIter));
        LoadSceneList(lSelectedSceneIndex);
    }

    shared_ptr<Framework::Project> UIFloorPlanner3D::GetCurrentProject()
    {
        if (mCurrentProject == nullptr)
            CRASH("No project set as current.");

        return mCurrentProject;
    }

    bool UIFloorPlanner3D::LoadProject(std::string aProjectFile)
    {
        shared_ptr<Framework::Project> lProject = Engine::Instance()->ResourceManager().GetProjectByFile(aProjectFile);
        if (lProject == nullptr)
        {
            CRASH("Trying to load project that does not exist in ResourceManager");
            return false;
        }
        
        if (mCurrentProject)
            mCurrentProject->Unload();

        if (!lProject->Load())
        {
            WARNING("Project load failed for project named %s", lProject->GetName().c_str());
            return false;
        }

        mCurrentProject = lProject;
        return true;
    }

    void UIFloorPlanner3D::Deserialize(const Json::Value& aSerializer)
    {
        UICanvasGL::Deserialize(aSerializer);

        string lProjectFile = aSerializer.get("projectFile", "").asString();
        LoadProject(lProjectFile);

        //  UIComboBox implementation requires that it is created as a child of a UIWindow element.
        //  Hence we create the combo box here instead of the constructor of UIFloorPlanner3D.
        //  This means that this pointer is now a valid initialized UIWindow object and can be passed
        //  to the constructor of UIComboBox
        mSceneSelectCombo = std::make_unique<UIComboBox>(this);
        mSceneSelectCombo->setId("sceneSelectCombo");
        mSceneSelectCombo->setFixedSize({ 235, 35 });
        mSceneSelectCombo->setCaptionAlignment(Alignment::eLeft, 20);
        mSceneSelectCombo->ignoreParentPosition(true);

        //Set the callbacks
        auto lCallbackFunc = std::bind(&UIFloorPlanner3D::ChangeComboScene, this, _1);
        mSceneSelectCombo->setCallback(lCallbackFunc);
        auto lRemoveCallbackFunc = std::bind(&UIFloorPlanner3D::RemoveComboScene, this, _1);
        mSceneSelectCombo->setRemoveCallback(lRemoveCallbackFunc);

        mSceneSelectCombo->setAppearance(UIPopup::Appearance::Down | UIPopup::Appearance::Menu);
        LoadSceneList(0);
    }

    bool UIFloorPlanner3D::CustomRenderSort2D(Framework::Model2D* aModel1, Framework::Model2D* aModel2)
    {
        std::string lGroupModel1 = "";
        std::string lGroupModel2 = "";

        weak_ptr<CatalogCmp> lCatalogCmp1 = aModel1->GetOwnerEntity()->GetComponent<CatalogCmp>();
        if (!lCatalogCmp1.expired())
            lGroupModel1 = lCatalogCmp1.lock()->GetGroup();
        
        weak_ptr<CatalogCmp> lCatalogCmp2 = aModel2->GetOwnerEntity()->GetComponent<CatalogCmp>();
        if (!lCatalogCmp2.expired())
            lGroupModel2 = lCatalogCmp2.lock()->GetGroup();

        if (lGroupModel1.compare(lGroupModel2) == 0)
            return (aModel1->mBuddy->GetPosition().y > aModel2->mBuddy->GetPosition().y);
        else if (lGroupModel1.compare("RoomShapes") == 0)
            return false;
        else if (lGroupModel2.compare("RoomShapes") == 0)
            return true;
        else
            return (aModel1->mBuddy->GetPosition().y > aModel2->mBuddy->GetPosition().y);
    }
}
