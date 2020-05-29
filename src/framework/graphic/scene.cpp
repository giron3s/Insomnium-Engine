/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "engine/components/entity.h"
#include "engine/components/cameracmp.h"
#include "engine/components/motioncmp.h"
#include "engine/components/directlightcmp.h"
#include "engine/components/pointlightcmp.h"
#include "engine/components/spotlightcmp.h"
#include "engine/components/renderablecmp.h"
#include "engine/components/transformcmp.h"

#include "graphic/scene.h"
#include "graphic/rendertarget.h"
#include "graphic/noaarendertarget.h"

#include "graphic/gui/widget.h"

#include "json/json.h"
#include "core/serialization/jsoncpputils.h"

#include "glm/gtx/rotate_vector.hpp"

namespace Framework
{
    
    bool Scene::Add(Procedural::Grid* aGrid)
    {
        if( aGrid == nullptr )
            CRASH("ERROR Not the Grid is NULL");

        if ( mGrid )
            CRASH("ERROR Only one Grid per scene is allowed");

        mGrid = aGrid;

        Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
        return true;
    }

    bool Scene::Add(Model2D *aElem)
    {
        if(dynamic_cast<Procedural::Grid*>(aElem))
        {
            CRASH("Model2D model is the procedural grid! Use the Scene::Add(Procedural::Grid* aGrid)!");
            return false;
        }

        if (GetModel2D(aElem->GetName()) != NULL)
        {
            CRASH("Model2D named '%s' already exists and cannot be added again!", aElem->GetName().c_str());
            return false;
        }

        mModels2D.push_back(aElem);

        Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
        return true;
    }

    bool Scene::Add(Model3D *aElem)
    {
        if (GetModel3D(aElem->GetName()) != NULL)
        {
            WARNING("Model3D named '%s' already exists and cannot be added again!", aElem->GetName().c_str());
            aElem->SetName(aElem->GetName());
        }
        mModels3D.push_back(aElem);

        Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
        return true;
    }

    bool Scene::Add(PointLight *aElem)
    {
        if (GetPointLight(aElem->GetName()) != NULL)
        {
            CRASH("Pointlight named '%s' already exists and cannot be added again!", aElem->GetName().c_str());
            return false;
        }
        mPointLights.push_back(aElem);

        Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
        return true;
    }

    bool Scene::Add(SpotLight *aElem)
    {
        if (GetSpotLight(aElem->GetName()) != NULL)
        {
            CRASH("Spotlight named '%s' already exists and cannot be added again!", aElem->GetName().c_str());
            return false;
        }
        mSpotLights.push_back(aElem);

        Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
        return true;
    }

    bool Scene::Add(DirectLight *aElem)
    {
        //if (GetDirectLight() != NULL)
        //{
        //    CRASH("DirectLight named '%s' already exists and cannot be added again!", aElem->GetName().c_str());
        //    return false;
        //}
        mDirectLight = aElem;

        Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
        return true;
    }


    bool Scene::Add(Camera* aCamera)
    {
        if ( GetCamera(aCamera->GetName()) != NULL )
        {
           CRASH("Camera named '%s' already exists and cannot be added again!", aCamera->GetName().c_str());
           return false;
        }
        mCameras.push_back(aCamera);

        if ( mActiveCamera == NULL )
        {
            mActiveCamera = aCamera;
        }
        else
        { // disable updates for other (non-active) camera
            auto lCameraEntity = Engine::Instance()->EntityManager().GetEntityByID(aCamera->GetEntityID());
            ASSERT(lCameraEntity.get()); // debug consistency check
            auto lCameraMotion = lCameraEntity->GetComponent<MotionCmp>();
            lCameraMotion.lock()->SetUpdatable(false);
        }

        Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
        return true;
    }

    bool Scene::Add(RenderTarget* aRenderTarget)
    {
        if ( GetRenderTarget(aRenderTarget->GetName()) != NULL )
        {
            CRASH("RenderTarget named '%s' already exists and cannot be added again!", aRenderTarget->GetName().c_str());
            return false;
        }

        mRenderTargets.push_back(aRenderTarget);


        Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
        return true;
    }


    Model3D* Scene::GetModel3D(const string &aName) const
    {
        for ( auto lModel : mModels3D )
            if ( lModel->GetName() == aName )
                return lModel;
        return nullptr;
    }

    Model2D* Scene::GetModel2D(const string &aName) const
    {
        for ( auto lModel : mModels2D )
            if ( lModel->GetName() == aName )
                return lModel;
        return nullptr;
    }


    PointLight* Scene::GetPointLight(const string &aName) const
    {
        for ( auto lPointLight : mPointLights )
            if ( lPointLight->GetName() == aName )
                return lPointLight;
        return nullptr;
    }

    SpotLight* Scene::GetSpotLight(const string &aName) const
    {
        for ( auto lSpotLight : mSpotLights )
            if ( lSpotLight->GetName() == aName )
                return lSpotLight;
        return nullptr;
    }

    Camera* Scene::GetCamera(const string &aName) const
    {
        for ( auto lCamera : mCameras )
            if ( lCamera->GetName() == aName )
                return lCamera;
        return nullptr;
    }

    RenderTarget* Scene::GetRenderTarget(const string &aName) const
    {
        for ( auto lRenderTarget : mRenderTargets )
            if ( lRenderTarget->GetName() == aName )
                return lRenderTarget;
        return nullptr;
    }

    bool Scene::SetActiveCamera(const string &aName)
    {
        if ( mActiveCamera )
        {
            auto lCameraEntity = Engine::Instance()->EntityManager().GetEntityByID(mActiveCamera->GetEntityID());
            ASSERT(lCameraEntity.get()); // debug consistency check
            auto lCameraMotion = lCameraEntity->GetComponent<MotionCmp>();
            lCameraMotion.lock()->SetUpdatable(false);
        }
        Camera *lCamera = GetCamera(aName);
        if ( lCamera == NULL )
        {
            return false;
        }
        mActiveCamera = lCamera;
        auto lCameraEntity = Engine::Instance()->EntityManager().GetEntityByID(mActiveCamera->GetEntityID());
        ASSERT(lCameraEntity.get()); // debug consistency check
        auto lCameraMotion = lCameraEntity->GetComponent<MotionCmp>();
        lCameraMotion.lock()->SetUpdatable(true);
        return true;
    }

    /*bool Scene::SetActiveRenderTarget(const string &aName)
    {
        RenderTarget* lTarget = GetRenderTarget(aName);
        if ( lTarget == NULL ) {
            return false;
        }
        mActiveRenderTarget = lTarget;
        return true;
    }*/


    void Scene::Serialize(Json::Value& aSerializer) const
    {
        aSerializer["sceneName"] = mName;
        aSerializer["rendertarget"] = "NoAA"; // ?

        Json::Value& v = aSerializer["entities"];
        v = Json::Value(Json::ValueType::arrayValue);
        const EntityManager& lEntityManager = Engine::Instance()->EntityManager();
        unsigned int i = 0;
        for ( const auto lCamera : mCameras )
        {
            auto lEntity = lEntityManager.GetEntityByID(lCamera->GetEntityID());
            lEntity->Serialize(v[i++]);
        }
        if ( mGrid )
        {
            auto lEntity = lEntityManager.GetEntityByID(mGrid->GetEntityID());
            lEntity->Serialize(v[i++]);
        }
        for ( const auto lModel : mModels3D )
        {
            auto lEntity = lEntityManager.GetEntityByID(lModel->GetEntityID());
            lEntity->Serialize(v[i++]);
        }
        for ( const auto lModel : mModels2D )
        {
            if ( !lModel->mBuddy )
            {
                auto lEntity = lEntityManager.GetEntityByID(lModel->GetEntityID());
                lEntity->Serialize(v[i++]);
            }
        }
        for ( const auto lLight : mPointLights )
        {
            auto lEntity = lEntityManager.GetEntityByID(lLight->GetEntityID());
            lEntity->Serialize(v[i++]);
        }
        for ( const auto lLight : mSpotLights )
        {
            auto lEntity = lEntityManager.GetEntityByID(lLight->GetEntityID());
            lEntity->Serialize(v[i++]);
        }
        if ( mDirectLight )
        {
            auto lEntity = lEntityManager.GetEntityByID(mDirectLight->GetEntityID());
            lEntity->Serialize(v[i++]);
        }
    }

    void Scene::Deserialize(const Json::Value& aSerializer)
    {
        if(aSerializer.isMember("sceneName"))
            mName = aSerializer["sceneName"].asString();

        if( !aSerializer.isMember("rendertarget")
            || !aSerializer.isMember("rendertarget_clearcolor"))
        {
            CRASH("Not defined one 'rendertarget' attribute at scene file!");
        }

        //Deserialize Render Target
        const string   lRTType   = aSerializer["rendertarget"].asString();
        glm::u32vec2 lRTSize(256,256);
        if(aSerializer.isMember("rendertarget_size") )
            DeserializeVec<glm::u32vec2>(aSerializer["rendertarget_size"], lRTSize);
        glm::vec4 lRTClearColor;
        DeserializeVec<glm::vec4>(aSerializer["rendertarget_clearcolor"], lRTClearColor);
        
        // reset scene bounding box
        mBoundingBox.SetMin(glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX));
        mBoundingBox.SetMax(glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX));

#define COLOR_CLEAR_VALUE {lRTClearColor.r, lRTClearColor.g, lRTClearColor.b, lRTClearColor.a}
#define COLOR_PICKING_ATTACHMENT GL_COLOR_ATTACHMENT1
        if ( lRTType.compare("NoAA") == 0 )
        {
            /* Add NoAA RT for forward shading in 2D view.
               This RT requires a depth-stencil buffer and an additional
               color buffer for color buffer for color picking. */
            /* NOTE: the same RT shall be used as a target for lighting pass
               in deferred shading in 3D view. */
            auto lRT_NoAA = NoAARenderTarget::New();
            lRT_NoAA->SetName("NoAA");
            RenderTarget::Attachments lAttachments;
            lAttachments[GL_COLOR_ATTACHMENT0] = {GL_TEXTURE_2D, GL_RGBA8, COLOR_CLEAR_VALUE}; // primary color buffer
            lAttachments[COLOR_PICKING_ATTACHMENT] = {GL_TEXTURE_2D, GL_RGB8, DEFAULT_COLOR_CLEAR_VALUE}; // color picking buffer for on-screen model selection
            lAttachments[GL_DEPTH_STENCIL_ATTACHMENT] = {GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, DEFAULT_DEPTH_STENCIL_CLEAR_VALUE};
            bool res = lRT_NoAA->Init(lRTSize.x, lRTSize.y, &lAttachments);
            ASSERT(res);
            Add(lRT_NoAA);
        }
        else
        {
            CRASH("Unknown Render Target Type(%s)", lRTType.c_str());
        }

        // Add Deferred Shading Render Target
        {
            auto lRT_GBuffer = RenderTarget::New();
            lRT_GBuffer->SetName("GBuffer");
            RenderTarget::Attachments lAttachemnts;
            lAttachemnts[GL_COLOR_ATTACHMENT0] = {GL_TEXTURE_2D, GL_RGBA8, COLOR_CLEAR_VALUE}; // diffuse
            lAttachemnts[COLOR_PICKING_ATTACHMENT] = {GL_TEXTURE_2D, GL_RGB8, DEFAULT_COLOR_CLEAR_VALUE}; // color picking buffer for on-screen model selection
            lAttachemnts[GL_COLOR_ATTACHMENT2] = {GL_TEXTURE_2D, GL_RGB32F, DEFAULT_COLOR_CLEAR_VALUE}; // position
            lAttachemnts[GL_COLOR_ATTACHMENT3] = {GL_TEXTURE_2D, GL_RGBA32F, {0.0f, 0.0f, 0.0f, 0.0f}}; /* normal + lighting flag in w:
                0 = fragment is not subject to lighting,
                1 = fragment is subject to lighting but not a shadow receiver,
                2 = fragment is subject to lighting and a shadow receiver */
            lAttachemnts[GL_DEPTH_STENCIL_ATTACHMENT] = {GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, DEFAULT_DEPTH_STENCIL_CLEAR_VALUE};
            bool res = lRT_GBuffer->Init(lRTSize.x, lRTSize.y, &lAttachemnts);
            ASSERT(res);
            ASSERT((uint32_t)Model::ReservedId::ClearColor == 0xFF000000ui32); // === DEFAULT_CLEAR_COLOR_VALUE
            Add(lRT_GBuffer);
        }

        if (aSerializer.isMember("constraints3d") )
        {
            const auto& lConstraints3d = aSerializer["constraints3d"];
            glm::vec3& lBB_min = const_cast<glm::vec3&>(mBoundingBox.GetMin());
            if ( lConstraints3d.isMember("min_x") )
                lBB_min.x = lConstraints3d["min_x"].asFloat();
            if ( lConstraints3d.isMember("min_y") )
                lBB_min.y = lConstraints3d["min_y"].asFloat();
            if ( lConstraints3d.isMember("min_z") )
                lBB_min.z = lConstraints3d["min_z"].asFloat();
            glm::vec3& lBB_max = const_cast<glm::vec3&>(mBoundingBox.GetMax());
            if ( lConstraints3d.isMember("max_x") )
                lBB_max.x = lConstraints3d["max_x"].asFloat();
            if ( lConstraints3d.isMember("max_y") )
                lBB_max.y = lConstraints3d["max_y"].asFloat();
            if ( lConstraints3d.isMember("max_z") )
                lBB_max.z = lConstraints3d["max_z"].asFloat();
            if ( lBB_min.x > lBB_max.x || lBB_min.y > lBB_max.y || lBB_min.z > lBB_max.z )
                CRASH("constraints3d member is invalid");
        }

        // Deserialize entities
        const Json::Value& lEntitiesRes = aSerializer["entities"];
        ASSERT(lEntitiesRes.isArray());
        for ( size_t i = 0, lSize = lEntitiesRes.size(); i < lSize; ++i )
        {
            const Json::Value lEntityRes = lEntitiesRes[static_cast<int>(i)];
            auto lEntity = Engine::Instance()->EntityManager().CreateEntityFromData(lEntityRes).lock();

            if ( lEntity->HasComponent<CameraCmp>() )
            {
                auto lCamera = lEntity->GetComponent<CameraCmp>().lock();
                Add(lCamera->GetCamera());
            }
            else if ( lEntity->HasComponent<RenderableCmp>() )
            {
                auto lRenderable = lEntity->GetComponent<RenderableCmp>().lock();
                auto lModel2D = lRenderable->GetModel2D();
                auto lModel3D = lRenderable->GetModel3D();

                if (lModel2D)
                {
                    Procedural::Grid* lGrid = dynamic_cast<Procedural::Grid*>(lModel2D);
                    if(lGrid)
                        Add(lGrid); // Add 2D grid
                    else 
                        Add(lModel2D); // Add any 2D model
                }

                if ( lModel3D )
                {
                    bool constrained = lModel3D->ConstrainPosition(mBoundingBox);
                    if ( constrained && lEntity->HasComponent<TransformCmp>() )
                    { // Read back the constrained transform values
                        auto lTransformCmp = lEntity->GetComponent<TransformCmp>().lock();
                        lTransformCmp->SetPosition(lModel3D->GetPosition());
                        //lTransformCmp->SetOrientation(glm::quat_cast(lModel3D->GetOrientation()));
                        //lTransformCmp->SetScale(lModel3D->GetScaleFactor());
                    }
                    Add(lModel3D);
                }
            }
            else if ( lEntity->HasComponent<DirectLightCmp>() )
            {
                auto lDirectLight = lEntity->GetComponent<DirectLightCmp>().lock();
                Add(lDirectLight->GetDirectLight());
            }
            else if ( lEntity->HasComponent<SpotLightCmp>() )
            {
                auto lSpotLight = lEntity->GetComponent<SpotLightCmp>().lock();
                Add(lSpotLight->GetSpotLight());
            }
            else if ( lEntity->HasComponent<PointLightCmp>() )
            {
                auto lPointLight = lEntity->GetComponent<PointLightCmp>().lock();
                Add(lPointLight->GetPointLight());
            }
        }

        mIsLoaded = true;
    }

    void Scene::Unload()
    {
        for (auto lRT : mRenderTargets)
            delete lRT;
        mRenderTargets.clear();

        mModels3D.clear();
        mModels2D.clear();
        mFocusedModel = nullptr;
        mSelectedModels.clear();
        mModelUnderTransform = TransformInfo();
        mDirectLight = nullptr;
        mPointLights.clear();
        mSpotLights.clear();
        mCameras.clear();

        mIsLoaded = false;
    }


    Model::ReservedId Scene::GetModelAtPoint(int aX, int aY, __out Model*& aModel) const
    {
        const char* lRTName = mActiveCamera->GetProjectionType() == Projection::PERSPECTIVE ? "GBuffer" : "NoAA";
        RenderTarget* lRT = GetRenderTarget(lRTName);
        if (!lRT)
            CRASH("Scene '%s' does not have a %s render target", GetName().c_str(), lRTName);


        INFO(LogLevel::eLEVEL1, "GetModel at [%d , %d]", aX, aY);
        aModel = nullptr;
        constexpr uint32_t COLOR_PICKING_BUFFER_INDEX = (COLOR_PICKING_ATTACHMENT - GL_COLOR_ATTACHMENT0);
        uint32_t lPixel = lRT->GetColor(aX, aY, COLOR_PICKING_BUFFER_INDEX);

        // first check against reserved ids:
        switch ( static_cast<Model::ReservedId>(lPixel) )
        {
            case Model::ReservedId::ClearColor:
            case Model::ReservedId::GizmoAxisX:
            case Model::ReservedId::GizmoAxisY:
            case Model::ReservedId::GizmoAxisZ:
            case Model::ReservedId::GizmoAxisAll:
            return static_cast<Model3D::ReservedId>(lPixel);
        }

        // Projection is ORTHOGRAPHIC - check against 2D model ids
        if ( mActiveCamera->GetProjectionType() == Camera::Projection::ORTHOGRAPHIC )
        {
            for (list<Model2D*>::const_iterator lModelIter = mModels2D.begin(); lModelIter != mModels2D.end(); ++lModelIter )
            {
                Model2D* lModel2D = *lModelIter;
                uint32_t lModelId = lModel2D->GetId();
                const uint8_t* lModelColor = (uint8_t*)&lModelId;
                const uint8_t* lPickedColor = (uint8_t*)&lPixel;
                uint8_t d = (lModelColor[0] - lPickedColor[0]) +
                    (lModelColor[1] - lPickedColor[1]) +
                    (lModelColor[2] - lPickedColor[2]) +
                    (lModelColor[3] - lPickedColor[3]);
                if ( d < 4 ) // TODO: try if lModelId == lPixel
                {
                    aModel = lModel2D;
                    return Model::ReservedId::ModelMask;
                }
            }
        }
        // Projection is PERSPECTIVE - check against 3D model ids
        else
        {
            for (list<Model3D*>::const_iterator lModelIter = mModels3D.begin(); lModelIter != mModels3D.end(); ++lModelIter )
            {
                Model3D* lModel3D = *lModelIter;
                uint32_t lModelId = lModel3D->GetId();
                const uint8_t* lModelColor = (uint8_t*)&lModelId;
                const uint8_t* lPickedColor = (uint8_t*)&lPixel;
                uint8_t d = (lModelColor[0] - lPickedColor[0]) +
                    (lModelColor[1] - lPickedColor[1]) +
                    (lModelColor[2] - lPickedColor[2]) +
                    (lModelColor[3] - lPickedColor[3]);

                if ( d < 4 ) // TODO: try if lModelId == lPixel
                {
                    aModel = lModel3D;
                    return Model::ReservedId::ModelMask;
                }
            }
        }

        ASSERT(0); // we should not reach this point!
        return Model::ReservedId::ClearColor;
    }

    void Scene::DeleteSelectedModels()
    {
        // Projection is PERSPECTIVE - remove selected 3d models and their 2d buddies from the scene
        if ( mActiveCamera->GetProjectionType() == Camera::Projection::PERSPECTIVE )
        {
            for ( auto lSelectedModel3D : mSelectedModels )
            {
                for ( auto i = mModels3D.begin(); i != mModels3D.end(); ++i )
                {
                    if ( *i == lSelectedModel3D )
                    {
                        mModels3D.erase(i);
                        Model3D* lModel3D = dynamic_cast<Model3D*>(lSelectedModel3D);

                        // 3d model has a 2d buddy -> Remove it
                        if ( lModel3D->mBuddy )
                        {
                            for ( auto j = mModels2D.begin(); j != mModels2D.end(); ++j )
                            {
                                if ( *j == lModel3D->mBuddy )
                                {
                                    mModels2D.erase(j);
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
        // Projection is ORTHOGRAPHIC - remove selected 2d models and their 3d couterparts from the scene
        else
        {
            for ( auto lSelectedModel2D : mSelectedModels )
            {
                for ( auto i = mModels2D.begin(); i != mModels2D.end(); ++i )
                {
                    if ( *i == lSelectedModel2D )
                    {
                        mModels2D.erase(i);
                        Model2D* lModel2D = dynamic_cast<Model2D*>(lSelectedModel2D);

                        // 2d model has a 3d buddy -> Remove it
                        if ( lModel2D->mBuddy )
                        {
                            for ( auto j = mModels3D.begin(); j != mModels3D.end(); ++j )
                            {
                                if ( *j == lModel2D->mBuddy )
                                {
                                    mModels3D.erase(j);
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }

        // Remove selected models and their 2d/3d couterparts from EntityManager
        EntityManager& lEntityManager = Engine::Instance()->EntityManager();
        for ( auto lModel : mSelectedModels )
        {
            /* Note:  A model is always a part (a component) of RenderableCmp,
            so it is 'hosted' by a RenderableCmp. Model's EntityID == host RenderableCmp EntityID.
            Therefore, removing a model will actually remove the whole RenderableCmp
            inluding all it components */

            Model2D* lModel2D = dynamic_cast<Model2D*>(lModel);
            Model3D* lModel3D = dynamic_cast<Model3D*>(lModel);

            if ( lModel2D )
                lEntityManager.DestroyEntity(lModel2D->GetEntityID());
            if ( lModel3D )
                lEntityManager.DestroyEntity(lModel3D->GetEntityID());
        }

        mSelectedModels.clear();
        mFocusedModel = nullptr;
        mModelUnderTransform.mTarget = nullptr;
        mModelUnderTransform.mTransformType = TransformInfo::TransformType::None;
        mModelUnderTransform.mAxis = Model::ReservedId::ModelMask;

        Engine::Instance()->StateMachine().ExecuteAction("OnChangedSceneSelection");
        Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
    }


    void Scene::ToggleCamera()
    {
        glm::vec3 lPosition;
        glm::mat4 lOrientation = glm::mat4(1.0);
        if ( mActiveCamera->GetProjectionType() == Camera::Projection::PERSPECTIVE )
        {
            for ( auto lModel : mModels3D )
            {
                if ( lModel->mBuddy )
                {
                    lPosition = lModel->GetPosition();
                    // (x,y,z) in 3D view -> (x,-z,0) in 2D view:
                    lPosition.y = -lPosition.z;
                    lPosition.z = 0.0;
                    lModel->mBuddy->SetPosition(lPosition);
                    // rotation about Y in 3D view -> rotation about Z in 2D view
                    float a = atan2(-lModel->GetOrientation()[0].z, lModel->GetOrientation()[0].x);
                    lOrientation[0] = glm::rotateZ(glm::vec4(1, 0, 0, 0), a);
                    lOrientation[1] = glm::rotateZ(glm::vec4(0, 1, 0, 0), a);
                    lModel->mBuddy->SetOrientation(lOrientation);
                    // scale in 3D view = scale in 2D view
                    lModel->mBuddy->SetScaleFactor(lModel->GetScaleFactor());
                }
            }
            SetActiveCamera("camera_orthographic");
        }
        else
        {
            for ( auto lModel : mModels2D )
            {
                if ( lModel->mBuddy )
                {
                    // (x,y,0) in 2D view -> (x,y',-y) in 3D view
                    // where y' is the value of '3d-buddy' in 3D view
                    lPosition = lModel->GetPosition();
                    lPosition.z = -lPosition.y;
                    lPosition.y = lModel->mBuddy->GetPosition().y;
                    lModel->mBuddy->SetPosition(lPosition);
                    // rotation about Z in 2D view -> rotation about Y in 3D view
                    float a = atan2(lModel->GetOrientation()[0].y, lModel->GetOrientation()[0].x);
                    lOrientation[0] = glm::rotateY(glm::vec4(1, 0, 0, 0), a);
                    lOrientation[2] = glm::rotateY(glm::vec4(0, 0, 1, 0), a);
                    lModel->mBuddy->SetOrientation(lOrientation);
                    // scale in 2D view = scale in 3D view
                    lModel->mBuddy->SetScaleFactor(lModel->GetScaleFactor());
                }
            }
            SetActiveCamera("camera_perspective");
        }
        mModelUnderTransform.mAxis = Model3D::ReservedId::ModelMask;
        mModelUnderTransform.mTarget = nullptr;
        mSelectedModels.clear(); // TODO: maybe later transfer selection from one view to another

        Engine::Instance()->StateMachine().ExecuteAction("OnChangedSceneSelection");
        Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
    }
}
