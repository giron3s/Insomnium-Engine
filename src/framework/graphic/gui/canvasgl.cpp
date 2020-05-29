/*
nanogui/glcanvas.cpp -- Canvas widget for rendering full-fledged
OpenGL content within its designated area. Very useful for
displaying and manipulating 3D objects or scenes. Subclass it and
overload `drawGL` for rendering.

NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
The widget drawing code is based on the NanoVG demo application
by Mikko Mononen.

All rights reserved. Use of this source code is governed by a
BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"

#include "json/json.h"
#include "engine.h"
#include "engine/project.h"
#include "graphic/scene.h"
#include "graphic/viewport.h"
#include "graphic/renderer.h"
#include "graphic/opengl/opengl.h"
#include "graphic/gui/window.h"
#include "graphic/gui/screen.h"
#include "graphic/gui/canvasgl.h"
#include "graphic/gui/theme.h"
#include "engine/components/motioncmp.h"
#include "engine/entitymanager.h"


namespace Framework
{

    namespace GUI
    {

        UICanvasGL::UICanvasGL()
            : UIWidget(nullptr)
            , mDrawBorder(true)
            , mRTNeedsResize(false)
            , mRenderAABB(false)
            , mRenderOOBB(false)
            , mRenderBoundingSphere(false)
        {
            mSize = glm::ivec2(250, 250);
        }

        UICanvasGL::UICanvasGL(UIWidget *parent)
            : UIWidget(parent)
            , mDrawBorder(true)
            , mRTNeedsResize(false)
            , mRenderAABB(false)
            , mRenderOOBB(false)
            , mRenderBoundingSphere(false)
        {
            mSize = glm::ivec2(250, 250);
        }

        UICanvasGL::~UICanvasGL()
        {

        }

        void UICanvasGL::setSize(const glm::ivec2 &size)
        {
            UIWidget::setSize(size);
            // we need to resize the active render target now
            mRTNeedsResize = true;
        }

        void UICanvasGL::setScene(std::shared_ptr<Scene> aScene)
        {
            if (!aScene->IsLoaded())
                CRASH("Scene being set for rendering is not loaded");

            mScene = aScene;
        }

        /// Handle a focus change event (default implementation: record the focus status, but do nothing)
        bool UICanvasGL::focusEvent(bool focused)
        {
            mFocused = focused;
            SetCameraMovementEnabled(mFocused);

            return false;
        }

        Model::ReservedId UICanvasGL::GetModelAtPoint(int aX, int aY, __out Model*& aModel)
        {
            if ( !(0 <= aX && aX < mSize.x && 0 <= aY && aY < mSize.y) )
            {
                aModel = nullptr;
                return Model::ReservedId::ClearColor;
            }
            return mScene->GetModelAtPoint(aX, aY, aModel);
        }

        bool UICanvasGL::keyboardEvent(int key, int scancode, int action, int modifiers)
        {
            //Handle only the PRESS event
            if ( action == GLFW_RELEASE )
                return true;

            if ( !mScene )
                return true;

            switch ( key )
            {
                case GLFW_KEY_F1:
                {

                    mRenderAABB = !mRenderAABB;
                    for ( auto lModel : mScene->GetModels3D() )
                        lModel->SetRenderAABB(mRenderAABB);
                    break;
                }

                case GLFW_KEY_F2:
                {
                    mRenderOOBB = !mRenderOOBB;
                    for ( auto lModel : mScene->GetModels3D() )
                        lModel->SetRenderOOBB(mRenderOOBB);
                    break;
                }

                case GLFW_KEY_F3:
                {
                    mRenderBoundingSphere = !mRenderBoundingSphere;
                    for ( auto lModel : mScene->GetModels3D() )
                        lModel->SetRenderBoundingSphere(mRenderBoundingSphere);
                    break;
                }

                case GLFW_KEY_F4:
                {
                    Renderer* lRenderer = Engine::Instance()->Display().GetRenderer();
                    lRenderer->SetRenderNormals(!lRenderer->GetRenderNormals());
                    break;
                }

                case GLFW_KEY_F5:
                {
                    Renderer* lRenderer = Engine::Instance()->Display().GetRenderer();
                    lRenderer->SetRenderLightsMarkers(!lRenderer->GetRenderLightsMarkers());
                    break;
                }

                case GLFW_KEY_F6:
                {
                    Renderer* lRenderer = Engine::Instance()->Display().GetRenderer();
                    lRenderer->SetRenderAABB(!lRenderer->GetRenderAABB());
                    break;
                }

                case GLFW_KEY_F7:
                {
                    Renderer* lRenderer = Engine::Instance()->Display().GetRenderer();
                    if (lRenderer->GetRenderTargetIdx() < mScene->GetRenderTargets().size())
                    {        
                        if (lRenderer->GetRenderAttachmentIdx() == 0)
                        {
                            if (lRenderer->GetRenderTargetIdx() > 0)
                                lRenderer->SetRenderTargetIdx(lRenderer->GetRenderTargetIdx() - 1);
                            else
                            {
                                if (mScene->GetSpotLights().size() > 0)
                                {
                                    lRenderer->SetRenderTargetIdx(mScene->GetRenderTargets().size() + 2);
                                    lRenderer->SetRenderShadowMapIdx(mScene->GetSpotLights().size() - 1);
                                    lRenderer->SetRenderLightShadowMap(mScene->GetSpotLights().at(lRenderer->GetRenderShadowMapIdx()));
                                }
                                else if (mScene->GetPointLights().size() > 0)
                                {
                                    lRenderer->SetRenderTargetIdx(mScene->GetRenderTargets().size() + 1);
                                    lRenderer->SetRenderShadowMapIdx(mScene->GetPointLights().size() - 1);
                                    lRenderer->SetRenderLightShadowMap(mScene->GetPointLights().at(lRenderer->GetRenderShadowMapIdx()));
                                }
                                else
                                {
                                    lRenderer->SetRenderTargetIdx(mScene->GetRenderTargets().size());
                                    lRenderer->SetRenderLightShadowMap(mScene->GetDirectLight());
                                }
                            }
                        }
                        else
                        {
                            lRenderer->SetRenderAttachmentIdx(lRenderer->GetRenderAttachmentIdx() - 1);
                        }
                    }
                    else if (lRenderer->GetRenderTargetIdx() == mScene->GetRenderTargets().size())
                    {
                        //  Direct light is currently being rendered. So simply shift to the last render target
                        lRenderer->SetRenderTargetIdx(lRenderer->GetRenderTargetIdx() - 1);
                        lRenderer->SetRenderAttachmentIdx(mScene->GetRenderTargets().at(lRenderer->GetRenderTargetIdx())->GetNumColorBuffers() - 2);
                        lRenderer->SetRenderLightShadowMap(nullptr);
                    }
                    else if (lRenderer->GetRenderTargetIdx() == mScene->GetRenderTargets().size() + 1)
                    {
                        //  We are rendering point lights currently. If we are on the first point light then
                        //  shift back to direct light
                        if (lRenderer->GetRenderLightShadowMap() == mScene->GetPointLights().at(0))
                        {
                            lRenderer->SetRenderTargetIdx(mScene->GetRenderTargets().size());
                            lRenderer->SetRenderLightShadowMap(mScene->GetDirectLight());
                        }
                        else
                        {
                            lRenderer->SetRenderShadowMapIdx(lRenderer->GetRenderShadowMapIdx() - 1);
                            lRenderer->SetRenderLightShadowMap(mScene->GetPointLights().at(lRenderer->GetRenderShadowMapIdx()));
                        }
                    }
                    else if (lRenderer->GetRenderTargetIdx() == mScene->GetRenderTargets().size() + 2)
                    {
                        //  We are rendering spot lights currently. If we are on the first spot light then
                        //  shift back to last point light
                        if (lRenderer->GetRenderLightShadowMap() == mScene->GetSpotLights().at(0))
                        {
                            if (mScene->GetPointLights().size() > 0)
                            {
                                lRenderer->SetRenderTargetIdx(mScene->GetRenderTargets().size() + 1);
                                lRenderer->SetRenderShadowMapIdx(mScene->GetPointLights().size() - 1);
                                lRenderer->SetRenderLightShadowMap(mScene->GetPointLights().at(lRenderer->GetRenderShadowMapIdx()));
                            }
                            else
                            {
                                //  If there are no point lights in the scene then shift to direct light
                                lRenderer->SetRenderTargetIdx(mScene->GetRenderTargets().size());
                                lRenderer->SetRenderLightShadowMap(mScene->GetDirectLight());
                            }
                        }
                        else
                        {
                            lRenderer->SetRenderShadowMapIdx(lRenderer->GetRenderShadowMapIdx() - 1);
                            lRenderer->SetRenderLightShadowMap(mScene->GetSpotLights().at(lRenderer->GetRenderShadowMapIdx()));
                        }
                    }

                    break;
                }

                case GLFW_KEY_F8:
                {
                    Renderer* lRenderer = Engine::Instance()->Display().GetRenderer();

                    if (lRenderer->GetRenderTargetIdx() == mScene->GetRenderTargets().size())
                    {
                        lRenderer->SetRenderTargetIdx(mScene->GetRenderTargets().size() + 1); // Shift to point lights
                        lRenderer->SetRenderShadowMapIdx(-1);
                    }

                    if (lRenderer->GetRenderTargetIdx() == mScene->GetRenderTargets().size() - 1 &&
                        lRenderer->GetRenderAttachmentIdx() == mScene->GetRenderTargets().at(lRenderer->GetRenderTargetIdx())->GetNumColorBuffers() - 2)
                    {
                        //  We are on the last render target so shift to direct light shadow map
                        lRenderer->SetRenderTargetIdx(mScene->GetRenderTargets().size());
                        lRenderer->SetRenderLightShadowMap(mScene->GetDirectLight());
                    }

                    if (lRenderer->GetRenderTargetIdx() < mScene->GetRenderTargets().size())
                    {
                        if (lRenderer->GetRenderAttachmentIdx() < mScene->GetRenderTargets().at(lRenderer->GetRenderTargetIdx())->GetNumColorBuffers() - 2)
                            lRenderer->SetRenderAttachmentIdx(lRenderer->GetRenderAttachmentIdx() + 1);
                        else if (lRenderer->GetRenderTargetIdx() < mScene->GetRenderTargets().size() - 1)
                            lRenderer->SetRenderTargetIdx(lRenderer->GetRenderTargetIdx() + 1);
                    }

                    if (lRenderer->GetRenderTargetIdx() == mScene->GetRenderTargets().size() + 1)
                    {
                        lRenderer->SetRenderShadowMapIdx(lRenderer->GetRenderShadowMapIdx() + 1);
                        if (lRenderer->GetRenderShadowMapIdx() >= mScene->GetPointLights().size())
                        {
                            lRenderer->SetRenderTargetIdx(mScene->GetRenderTargets().size() + 2); // Shift to spot lights
                            lRenderer->SetRenderShadowMapIdx(-1);
                        }
                        else
                            lRenderer->SetRenderLightShadowMap(mScene->GetPointLights().at(lRenderer->GetRenderShadowMapIdx()));
                    }

                    if (lRenderer->GetRenderTargetIdx() == mScene->GetRenderTargets().size() + 2)
                    {
                        lRenderer->SetRenderShadowMapIdx(lRenderer->GetRenderShadowMapIdx() + 1);
                        if (lRenderer->GetRenderShadowMapIdx() >= mScene->GetSpotLights().size())
                        {
                            lRenderer->SetRenderTargetIdx(0); // Shift back to first render target
                            lRenderer->SetRenderAttachmentIdx(0);
                            lRenderer->SetRenderShadowMapIdx(-1);
                        }
                        else
                            lRenderer->SetRenderLightShadowMap(mScene->GetSpotLights().at(lRenderer->GetRenderShadowMapIdx()));
                    }

                    break;
                }

                case GLFW_KEY_R:
                {
                    if ( mScene->mSelectedModels.size() == 1 )
                    {
                        mScene->mModelUnderTransform.mTarget = *mScene->mSelectedModels.begin();
                        mScene->mModelUnderTransform.mTransformType = Scene::TransformInfo::TransformType::Rotate;
                    }
                    break;
                }
                case GLFW_KEY_T:
                {
                    if ( mScene->mSelectedModels.size() == 1 )
                    {
                        mScene->mModelUnderTransform.mTarget = *mScene->mSelectedModels.begin();
                        mScene->mModelUnderTransform.mTransformType = Scene::TransformInfo::TransformType::Translate;
                    }
                    break;
                }
                case GLFW_KEY_S:
                {
                    if ( mScene->mSelectedModels.size() == 1 )
                    {
                        mScene->mModelUnderTransform.mTarget = *mScene->mSelectedModels.begin();
                        mScene->mModelUnderTransform.mTransformType = Scene::TransformInfo::TransformType::Scale;
                    }
                    break;
                }
                case GLFW_KEY_C:
                {
                    if ( mScene )
                        mScene->ToggleCamera();
                    break;
                }
                case GLFW_KEY_DELETE:
                {
                    mScene->DeleteSelectedModels();
                    break;
                }
                case GLFW_KEY_P:
                {
                    if (mScene->mSelectedModels.size() == 1)
                        Engine::Instance()->StateMachine().ExecuteActionWithArgs("GotoObjectEditor", (*mScene->mSelectedModels.begin())->GetOwnerEntity()->GetEntityID());
                }
                default:
                mScene->mModelUnderTransform.mTransformType = Scene::TransformInfo::TransformType::None;
            }
            return true;
        }


        static glm::vec3 sLastRayDirection(0.0f, 0.0f, 0.0f);

        bool UICanvasGL::mouseButtonEvent(const glm::ivec2 &aMousePointer, int aButton, bool aDown, int aModifiers)
        {
            if ( aButton != GLFW_MOUSE_BUTTON_2 )
                return true;

            if ( !mScene )
                return true;

            // No model selected for transformation
            if ( !mScene )
                return true;

            if ( !mScene->mModelUnderTransform.mTarget )
            {
                // model selected for transformation = focused model, focused model = none
                if ( aDown && mScene->mFocusedModel )
                {
                    auto lIt = std::find(mScene->mSelectedModels.begin(), mScene->mSelectedModels.end(), mScene->mFocusedModel);
                    if ( lIt == mScene->mSelectedModels.end() )
                        mScene->mSelectedModels.push_back(mScene->mFocusedModel);
                    else
                        mScene->mSelectedModels.erase(lIt);
                    //mScene->mModelUnderTransform.mTarget = mScene->mFocusedModel;
                    mScene->mFocusedModel = nullptr;
                    Engine::Instance()->StateMachine().ExecuteAction("OnChangedSceneSelection");
                }
                // No focused model - ignore event
                else
                    return true;
            }

            // Right button released - deselect transformation axis, reset LastRayDirection
            if ( !aDown )
            {
                mScene->mModelUnderTransform.mAxis = Model::ReservedId::ModelMask;
                sLastRayDirection.x = sLastRayDirection.y = sLastRayDirection.z = 0.0f;
                // But keep mScene->mModelUnderTransform.mTarget as is!
                return true;
            }

            int lPx = aMousePointer.x - mPos.x;
            int lPy = mSize.y - aMousePointer.y;
            //ASSERT(0 <= lPx && lPx < mSize.x  &&  0 <= lPy && lPy < mSize.y);

            Model* lModel = nullptr;
            auto lSelectedId = Model3D::ReservedId::ClearColor;
            if ( 0 <= lPx && lPx < mSize.x && 0 <= lPy && lPy < mSize.y )
                lSelectedId = mScene->GetModelAtPoint(lPx, lPy, lModel);
            switch ( lSelectedId )
            {
                // step 1: select a model that shall be under transformation
                case Model::ReservedId::ModelMask:
                {
                    if ( lModel != mScene->mModelUnderTransform.mTarget )
                    { // drop current transform, focus another model
                        mScene->mModelUnderTransform.mTarget = nullptr;
                        mScene->mFocusedModel = lModel;
                        return true;
                    }
                    break;

                }

                // step 2: if there is a model under transformation - select transformation axis
                case Model3D::ReservedId::GizmoAxisX:
                case Model3D::ReservedId::GizmoAxisY:
                case Model3D::ReservedId::GizmoAxisZ:
                case Model3D::ReservedId::GizmoAxisAll:
                {
                    if ( mScene->mModelUnderTransform.mTarget )
                        mScene->mModelUnderTransform.mAxis = lSelectedId;
                    break;
                }

                default:
                mScene->mModelUnderTransform.mTarget = nullptr;
            }

            return true;
        }


        bool UICanvasGL::mouseMotionEvent(const glm::ivec2 &p, const glm::ivec2 &rel, int button, int modifiers)
        {
            if ( !mScene || !mFocused )
                return true;

            //INFO(LogLevel::eLEVEL2, "Mouse Position in UICanvasGL. p(x,y) = %d,%d   rel(x,y) = %d, %d", p.x, p.y, rel.x, rel.y);

            int lPx = p.x - mPos.x;
            int lPy = mSize.y - p.y;
            //ASSERT(0 <= lPx && lPx < mSize.x  &&  0 <= lPy && lPy < mSize.y);

            if ( !mScene->mModelUnderTransform.mTarget )
            { // no model is selected for transformation, update model focus and return
                if ( 0 <= lPx && lPx < mSize.x && 0 <= lPy && lPy < mSize.y )
                    mScene->GetModelAtPoint(lPx, lPy, mScene->mFocusedModel);
                else
                    mScene->mFocusedModel = nullptr;
                return true;
            }
            // else a model is selected for transformation

            if ( mScene->mModelUnderTransform.mAxis != Model3D::ReservedId::GizmoAxisX &&
                mScene->mModelUnderTransform.mAxis != Model3D::ReservedId::GizmoAxisY &&
                mScene->mModelUnderTransform.mAxis != Model3D::ReservedId::GizmoAxisZ &&
                mScene->mModelUnderTransform.mAxis != Model3D::ReservedId::GizmoAxisAll )
            { // mouse movement is not related to model transformation
                sLastRayDirection.x = sLastRayDirection.y = sLastRayDirection.z = 0.0f;
                return true;
            }

            //lPy = -lPy;
            glm::vec4 lVec(
                ((double)lPx / mSize.x - 0.5) * 2.0,
                ((double)lPy / mSize.y - 0.5) * 2.0,
                0.0, 1.0); // lVec = mouse position in NDC
            //ASSERT(-1.0f <= lVec.x && lVec.x <= 1.0f  &&  -1.0f <= lVec.y && lVec.y <= 1.0f);
            if ( !(-1.0f <= lVec.x && lVec.x <= 1.0f  &&  -1.0f <= lVec.y && lVec.y <= 1.0f) )
            { // drag outside viewport
                mScene->mFocusedModel = nullptr;
                return true;
            }
            lVec = glm::inverse(mScene->GetActiveCamera()->GetProjectionMatrix()) * lVec; // lVec = mouse position in Eye space
            lVec.z = -1.0f; lVec.w = 0.0f; // lVec = ray direction in Eye space: forward, and not a point
            lVec = glm::inverse(mScene->GetActiveCamera()->GetViewMatrix()) * lVec; // lVec = ray direction in World space
            glm::vec3 lRayDirection(lVec.x, lVec.y, lVec.z);
            lRayDirection = glm::normalize(lRayDirection);
            const glm::vec3& lRayOrigin = mScene->GetActiveCamera()->GetPosition();

            if ( sLastRayDirection.x || sLastRayDirection.y || sLastRayDirection.z )
            {
                switch ( mScene->mModelUnderTransform.mTransformType )
                {

                    //TRANSLATE TRANSFORMATION
                    case Scene::TransformInfo::TransformType::Translate:
                    {
                        glm::vec3 lDelta = lRayDirection - sLastRayDirection;
                        switch ( mScene->mModelUnderTransform.mAxis )
                        {
                            case Model::ReservedId::GizmoAxisX:
                            {
                                lDelta.y = lDelta.z = 0.0f;
                                if ( mScene->GetActiveCamera()->GetProjectionType() == Camera::Projection::ORTHOGRAPHIC )
                                    lDelta.x = rel.x;
                            }
                            break;

                            case Model::ReservedId::GizmoAxisY:
                            {
                                lDelta.x = lDelta.z = 0.0f;
                                if ( mScene->GetActiveCamera()->GetProjectionType() == Camera::Projection::ORTHOGRAPHIC )
                                    lDelta.y = -rel.y;
                            }
                            break;

                            case Model::ReservedId::GizmoAxisZ:
                            {
                                lDelta.x = lDelta.y = 0.0f;
                                break;
                            }

                            default:
                            WARNING("Unknown Axis!");
                        }
                        //INFO(LogLevel::eLEVEL2, "lDelta = %f, %f, %f", lDelta.x, lDelta.y, lDelta.z);

                        if ( mScene->GetActiveCamera()->GetProjectionType() == Camera::Projection::ORTHOGRAPHIC )
                        { // we are in 2D view
                            Model2D* lModel2DTarget = static_cast<Model2D*>(mScene->mModelUnderTransform.mTarget);
                            lModel2DTarget->SetPosition(lModel2DTarget->GetPosition() + lDelta);
                            Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
                        }
                        else
                        { // we are in 3D view
                            Model3D* lModel3DTarget = static_cast<Model3D*>(mScene->mModelUnderTransform.mTarget);
                            lModel3DTarget->SetPosition(lModel3DTarget->GetPosition() + lDelta * 1000.f);
                            lModel3DTarget->ConstrainPosition(mScene->mBoundingBox);
                            Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
                        }
                        break;
                    }

                    //SCALE TRANSFORMATION
                    case Scene::TransformInfo::TransformType::Scale:
                    {
                        glm::vec3 lDelta = lRayDirection - sLastRayDirection;
                        switch ( mScene->mModelUnderTransform.mAxis )
                        {
                            case Model::ReservedId::GizmoAxisX:
                            {
                                lDelta.y = lDelta.z = 0.0f;
                                if ( mScene->GetActiveCamera()->GetProjectionType() == Camera::Projection::ORTHOGRAPHIC )
                                    lDelta.x = rel.x;
                            }
                            break;

                            case Model::ReservedId::GizmoAxisY:
                            {
                                lDelta.x = lDelta.z = 0.0f;
                                if ( mScene->GetActiveCamera()->GetProjectionType() == Camera::Projection::ORTHOGRAPHIC )
                                    lDelta.y = rel.y;
                            }
                            break;

                            case Model::ReservedId::GizmoAxisZ:
                            {
                                lDelta.x = lDelta.y = 0.0f;
                                break;
                            }

                            case Model::ReservedId::GizmoAxisAll:
                            {
                                if ( rel.x > 0 )
                                    lDelta.x = lDelta.y = lDelta.z = 0.01f;
                                else if ( rel.x < 0 )
                                    lDelta.x = lDelta.y = lDelta.z = -0.01f;
                                else
                                    lDelta.x = lDelta.y = lDelta.z = 0.0f;
                                break;
                            }

                            default:
                            WARNING("Unknown Axis!");
                        }

                        if ( mScene->GetActiveCamera()->GetProjectionType() == Camera::Projection::ORTHOGRAPHIC )
                        { // we are in 2D view
                            Model2D* lModel2DTarget = static_cast<Model2D*>(mScene->mModelUnderTransform.mTarget);
                            lModel2DTarget->SetScaleFactor(lModel2DTarget->GetScaleFactor() + lDelta);
                            Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
                        }
                        else
                        { // we are in 3D view
                            Model3D* lModel3DTarget = static_cast<Model3D*>(mScene->mModelUnderTransform.mTarget);
                            lModel3DTarget->SetScaleFactor(lModel3DTarget->GetScaleFactor() + lDelta * 1000.f);
                            lModel3DTarget->ConstrainPosition(mScene->mBoundingBox);
                            Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
                        }
                        break;
                    }

                    //ROTATE TRANSFORMATION
                    case Scene::TransformInfo::TransformType::Rotate:
                    {
                        Model2D* lModel2DTarget = nullptr;
                        Model3D* lModel3DTarget = nullptr;

                        glm::mat4 lModelOrientation;
                        glm::vec3 lRotationOrigin;

                        if ( mScene->GetActiveCamera()->GetProjectionType() == Camera::Projection::ORTHOGRAPHIC )
                        { // we are in 2D view
                            lModel2DTarget = static_cast<Model2D*>(mScene->mModelUnderTransform.mTarget);
                            lModelOrientation = lModel2DTarget->GetOrientation();
                            lRotationOrigin = lModel2DTarget->GetPosition();
                        }
                        else
                        { // we are in 3D view
                            lModel3DTarget = static_cast<Model3D*>(mScene->mModelUnderTransform.mTarget);
                            lModelOrientation = lModel3DTarget->GetOrientation();
                            lRotationOrigin = lModel3DTarget->GetPosition();
                        }
                        int i = mScene->GetActiveCamera()->GetProjectionType() == Camera::Projection::ORTHOGRAPHIC ? 2 : 1;
                        glm::vec3 lRotationAxis(lModelOrientation[i].x, lModelOrientation[i].y, lModelOrientation[i].z);

                        ASSERT_vec3_is_normalized(lRotationAxis);

                        glm::vec3 lPoint1, lPoint2;

                        //INFO(LogLevel::eLEVEL2, "lRayOrigin = %f, %f, %f", lRayOrigin.x, lRayOrigin.y, lRayOrigin.z);
                        //INFO(LogLevel::eLEVEL2, "lRayDirection = %f, %f, %f", lRayDirection.x, lRayDirection.y, lRayDirection.z);

                        auto c1 = LineVsPlane(lRayOrigin, lRayDirection,
                            lRotationOrigin, lRotationAxis,
                            lPoint1);

                        auto c2 = LineVsPlane(lRayOrigin, sLastRayDirection,
                            lRotationOrigin, lRotationAxis,
                            lPoint2);


                        //INFO(LogLevel::eLEVEL2, "lPoint1 = %f, %f, %f", lPoint1.x, lPoint1.y, lPoint1.z);
                        //INFO(LogLevel::eLEVEL2, "lPoint2 = %f, %f, %f", lPoint2.x, lPoint2.y, lPoint2.z);

                        float a = 0.0f;
                        if ( c1 == LineVsPlaneClassification::Intersect && c1 == c2 )
                        {

                            lPoint1 -= lRotationOrigin;
                            lPoint2 -= lRotationOrigin;
                            float l1 = glm::dot(lPoint1, lPoint1),
                                l2 = glm::dot(lPoint2, lPoint2);
                            if ( l1 > 1.0e-6f && l2 > 1.0e-6f )
                            {
                                lPoint1 *= (1.0f / sqrtf(l1));
                                lPoint2 *= (1.0f / sqrtf(l2));
                                if ( glm::dot(lRotationAxis, lRayOrigin - lRotationOrigin) < 0.0 )
                                    lRotationAxis *= -1.0f;
                                a = -SignedAngle(lPoint1, lPoint2, lRotationAxis);
                            }
                        }

                        //a *= 2.0f;

                        if ( abs(a) > 1.0e-6f )
                        {
                            glm::mat4 RotationMatrix = glm::rotate(a, lRotationAxis);
                            if (lModel2DTarget)
                            {
                                lModel2DTarget->Rotate(RotationMatrix);
                                Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
                            }
                            else
                            {
                                lModel3DTarget->Rotate(RotationMatrix);
                                lModel3DTarget->ConstrainPosition(mScene->mBoundingBox);
                                Engine::Instance()->StateMachine().ExecuteAction("SetProjectUnsaved");
                            }
                        }

                        break;
                    }

                }
            }
            sLastRayDirection = lRayDirection;
            return true;
        }

        void UICanvasGL::SetCameraMovementEnabled(bool aEnabled)
        { // TODO: maybe move this to Scene class???
            if ( !mScene )
                return;
            const Camera* lCamera = mScene->GetActiveCamera();
            size_t lEntityID = lCamera->GetEntityID();
            assert(lEntityID != Entity::GetDefaultID());
            std::shared_ptr<Entity> lEntity = Engine::Instance()->EntityManager().GetEntityByID(lEntityID);
            ASSERT(lEntity);
            std::shared_ptr<MotionCmp> lMotionCmp = lEntity->GetComponent<MotionCmp>().lock();
            ASSERT(lMotionCmp);

            lMotionCmp->SetUpdatable(aEnabled);
        }

        void UICanvasGL::drawWidgetBorder(NVGcontext *ctx) const
        {
            nvgBeginPath(ctx);
            nvgStrokeWidth(ctx, 1.0f);
            nvgRoundedRect(ctx, mPos.x - 0.5f, mPos.y - 0.5f,
                mSize.x + 1, mSize.y + 1, mTheme->mWindowCornerRadius);
            nvgStrokeColor(ctx, mTheme->mBorderLight);
            nvgRoundedRect(ctx, mPos.x - 1.0f, mPos.y - 1.0f,
                mSize.x + 2, mSize.y + 2, mTheme->mWindowCornerRadius);
            nvgStrokeColor(ctx, mTheme->mBorderDark);
            nvgStroke(ctx);
        }

        void UICanvasGL::draw(NVGcontext *ctx)
        {
            UIWidget::draw(ctx);
            nvgEndFrame(ctx);

            if ( mDrawBorder )
                drawWidgetBorder(ctx);

            const UIScreen* lScreen = this->screen();
            assert(lScreen);

            float lPixelRatio = lScreen->pixelRatio();
            glm::vec2 lScreenSize = static_cast<glm::vec2>(lScreen->size());
            glm::ivec2 lPositionInScreen = absolutePosition();

            glm::ivec2 lSize = static_cast<glm::ivec2>(static_cast<glm::vec2>(mSize) * lPixelRatio);
            glm::ivec2 lImagePosition = static_cast<glm::vec2>(glm::vec2(lPositionInScreen[0],
                lScreenSize[1] - lPositionInScreen[1] - (float)mSize[1]) * lPixelRatio);

            drawGL(Viewport(lImagePosition.x, lImagePosition.y, lSize.x, lSize.y));

            // restore the viewport dimensions // what for???
            __(glViewport(0, 0, lScreenSize.x, lScreenSize.y));
        }

        void UICanvasGL::drawGL(const Viewport& aViewPort)
        {
            if ( !mScene )
                return;

            Renderer* lRenderer = Engine::Instance()->Display().GetRenderer();
            if ( mRTNeedsResize )
            {
                for(auto& lRT : mScene->GetRenderTargets()) // resize All render targets
                    lRT->SetSize(mSize.x, mSize.y);

                for ( const auto& lCamera : mScene->GetCameras() )
                { // update 'aspect' param for all cameras
                    lCamera->SetAspect((float)mSize.x/mSize.y);
                    lCamera->RecalculateProjectionVolume();
                }
                mRTNeedsResize = false;
            }

            // restrict drawing to the viewport rectangle
            __(glViewport(aViewPort.getX(), aViewPort.getY(), aViewPort.getWidth(), aViewPort.getHeight()));
            __(glEnable(GL_SCISSOR_TEST));
            __(glScissor(aViewPort.getX(), aViewPort.getY(), aViewPort.getWidth(), aViewPort.getHeight()));


            //lRenderer->Clear();

            // disable the scissor test now, as it messes up the framebuffer blitting
            // when rendering the scene, we don't want to enable it anyway, as the rendering is done into an FBO, not the default framebuffer,
            // so it can't overlap with the windowing system
            __(glDisable(GL_SCISSOR_TEST));

            // render the scene
            lRenderer->RenderScene(*mScene);
            
            // now blit the scene
            if (lRenderer->GetRenderTargetIdx() >= mScene->GetRenderTargets().size())
            {
                __(glViewport(aViewPort.getX(), aViewPort.getY(), aViewPort.getWidth(), aViewPort.getHeight()));
                lRenderer->RenderShadowMapQuad(glm::vec2(0.0f), glm::vec2(4.0f), mScene->GetActiveCamera(), lRenderer->GetRenderLightShadowMap()->GetShadowMap(), nullptr);
            }
            else
            {
                RenderTarget* lRT = mScene->GetRenderTargets().at(lRenderer->GetRenderTargetIdx());
                lRT->Blit(aViewPort.getX(), aViewPort.getY(), aViewPort.getWidth(), aViewPort.getHeight(), lRenderer->GetRenderAttachmentIdx());
            }

            lRenderer->Flush();

            // render the debug info
            lRenderer->RenderDebug(*mScene);
        }

        void UICanvasGL::Serialize(Json::Value& aSerializer) const
        {
            //    Widget::save(s);
            //    s.set("backgroundColor", mBackgroundColor);
            //    s.set("drawBorder", mDrawBorder);
        }

        void UICanvasGL::Deserialize(const Json::Value& aSerializer)
        {
            UIWidget::Deserialize(aSerializer);
            mDrawBorder = aSerializer.get("drawBorder", mDrawBorder).asBool();
        }
    }
}
