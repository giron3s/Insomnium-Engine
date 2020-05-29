/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Interface for software/hardware renderers
 *******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "core/FPS.h"

#include "graphic/renderer.h"
#include "graphic/opengl/opengl_renderer.h"
#include "graphic/opengl/opengl_fontrenderer.h"
#include "graphic/procedural/texturedquad.h"
#include "graphic/procedural/plane.h"
#include "graphic/procedural/sphere.h"

#include "graphic/gui/canvasgl.h"

#include "engine/components/catalogcmp.h"



namespace Framework
{
    Renderer *Renderer::sRenderer = nullptr;
    FontRenderer *Renderer::sFontRenderer = nullptr;

    Renderer *Renderer::GetInstance(void)
    {
        if ( sRenderer == NULL ) {
            sRenderer = new OpenGLRenderer();
            sFontRenderer = new OpenGLFontRenderer();
            auto lFont = std::shared_ptr<TrueTypeFont>(TrueTypeFont::New());
            const auto& lFontInfo = Engine::Instance()->Config().GetFontInfo();
            if ( !lFont->Init(lFontInfo.path, lFontInfo.size) )
                CRASH("Failed to initialize default font");
            sFontRenderer->SetFont(lFont);
        }
        return sRenderer;
    }

    void Renderer::DisposeInstance(void)
    {
        delete sRenderer;
        delete sFontRenderer;
        sRenderer = nullptr;
        sFontRenderer = nullptr;
    }

    void Renderer::RenderScene(const Scene &aScene) const
    {
        if (!aScene.GetActiveCamera()) 
            return;

        // Force recalculation of projection volume planes
        aScene.GetActiveCamera()->RecalculateProjectionVolume();
        // Set default render state
        glDisable(GL_BLEND);
        glDisable(GL_STENCIL_TEST);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glFrontFace(GL_CCW);
        glEnable(GL_CULL_FACE);
        // Invoke 2D or 3D scene rendring routine
        if ( aScene.GetActiveCamera()->GetProjectionType() == Projection::ORTHOGRAPHIC )
            RenderScene2D(aScene);
        else
            RenderScene3D(aScene);
    }

    void Renderer::RenderScene2D(const Scene &aScene) const
    {
        auto lRT = aScene.GetRenderTarget("NoAA"); // TODO: Fix/Change later.
        if ( !lRT )
            return;

        lRT->BindForDrawning();
        lRT->Clear();

        const GLenum lColorBuffers[2] = { GL_COLOR_ATTACHMENT0/*main*/, GL_COLOR_ATTACHMENT1/*modelId*/ };
        // Render Grid if presents
        if ( aScene.GetGrid() )
        {
            int lDistMult = (int)(aScene.GetActiveCamera()->GetPosition().z / aScene.GetGrid()->GetRescaleDistance());
            if ( lDistMult < 1 )
                lDistMult = 1;
            glDrawBuffers(1, lColorBuffers); // don't draw to modelId buffer
            glDisable(GL_DEPTH_TEST);
            RenderGrid2D( // major
                aScene.GetGrid()->GetMajorStep2D() * lDistMult,
                glm::vec4(aScene.GetGrid()->GetColor(), 1.0f),
                aScene.GetGrid()->GetMajorLineWidth(),
                *aScene.GetActiveCamera());
            RenderGrid2D( // minor
                aScene.GetGrid()->GetMinorStep2D() * lDistMult,
                glm::vec4(aScene.GetGrid()->GetColor(), 1.0f),
                aScene.GetGrid()->GetMinorLineWidth(),
                *aScene.GetActiveCamera());
           glDrawBuffers(2, lColorBuffers); // draw to all buffers
           glEnable(GL_DEPTH_TEST);
        }

        // Collect visible models
        vector<Model2D*> lVisible2DModels;
        lVisible2DModels.reserve(aScene.GetModels2D().size());
        for (auto lModel : aScene.GetModels2D())
        {
            if (lModel->IsEnabled() && aScene.GetActiveCamera()->IsObjectVisible(*lModel))
                lVisible2DModels.push_back(lModel);
        }

        std::sort(lVisible2DModels.begin(), lVisible2DModels.end(), mRenderSort2D);

        // Temporary update camera scale factor
        const glm::vec3 lCameraScaleFactor = aScene.GetActiveCamera()->GetScaleFactor();
        float s = 10.0f / aScene.GetActiveCamera()->GetPosition().z;
        const_cast<Camera*>(aScene.GetActiveCamera())->SetScaleFactor({ s,s,1.0f });

        // Render visible models
        const glm::mat4 lViewProjectionMatrix = aScene.GetActiveCamera()->GetProjectionMatrix() * aScene.GetActiveCamera()->GetViewMatrix();
        for (auto lModel : lVisible2DModels)
        {
            uint32_t lFlags = 0;
            if (std::find(aScene.mSelectedModels.begin(), aScene.mSelectedModels.end(), lModel) != aScene.mSelectedModels.end())
                lFlags = RENDER_MODEL__RENDER_SELECTED_FLAG;
            else if (aScene.mFocusedModel == lModel)
                lFlags = RENDER_MODEL__RENDER_FOCUSED_FLAG;
            RenderModel2D(*lModel, lViewProjectionMatrix, lModel->GetShader(),
                lFlags);
        }

        // Render texts
        glDisable(GL_DEPTH_TEST);
        glDrawBuffers(1, lColorBuffers); // don't draw to modelId buffer
        const float lTextScale = 100.0 / aScene.GetActiveCamera()->GetPosition().z;
        constexpr uint32_t VERTICAL_SPACING = 20;
        for (auto lModel : lVisible2DModels)
            if (!lModel->mText.empty())
            {
                glm::vec4 lPos(lModel->GetPosition(), 1);
                lPos = lViewProjectionMatrix * lPos; // model center position in ndc
                lPos.x /= lPos.w;
                lPos.y /= lPos.w;
                lPos.x = (lPos.x + 1.0f) / 2.0f;
                lPos.y = (lPos.y + 1.0f) / 2.0f;
                lPos.y = 1.0 - lPos.y;
                lPos.x *= lRT->GetSize().x;
                lPos.y *= lRT->GetSize().y; // model center in screen space
                const glm::uvec2 lTextBox = Renderer::sFontRenderer->EvaluateText(lModel->mText.c_str(), lTextScale, VERTICAL_SPACING);
                const glm::uvec3 lTextLayout(lPos.x - lTextBox.x / 2, lPos.y - lTextBox.y / 2, VERTICAL_SPACING);
                const glm::vec4& lTextColor = Engine::Instance()->Config().GetFontInfo().color;
                Renderer::sFontRenderer->RenderText(lModel->mText.c_str(), lTextScale, lTextLayout,
                    lTextColor, *lRT);
            }
        glDrawBuffers(2, lColorBuffers); // draw to all buffers
        glEnable(GL_DEPTH_TEST);

        // Render a transform gizmo if any
        if (aScene.mModelUnderTransform.mTarget &&
            !(aScene.mModelUnderTransform.mTarget->mFlags & Model::ModelFlags::eDISABLE_TRASFORM_2D))
        {
            uint32_t lGizmoType = static_cast<uint32_t>(aScene.mModelUnderTransform.mTransformType);
            lGizmoType |= _2D_FLAG;
            Model2D* lModel2D = dynamic_cast<Model2D*>(aScene.mModelUnderTransform.mTarget);
            glDisable(GL_DEPTH_TEST);
            RenderTransformGizmo(lGizmoType, lModel2D, *aScene.GetActiveCamera());
            glEnable(GL_DEPTH_TEST);
        }

        lRT->Unbind();

        // Restore camera scale factor
        const_cast<Camera*>(aScene.GetActiveCamera())->SetScaleFactor(lCameraScaleFactor);

        //Render debug
        RenderDebug(aScene);
    }


    void Renderer::RenderToShadowMap(const Light* aLight, const std::list<Model3D*>& aModels3D, const Shader* aShader) const
    {
        //aShader->Attach();
        auto lRT_DepthMap = aLight->GetShadowMap();
        ASSERT(lRT_DepthMap);
        ASSERT(lRT_DepthMap->GetTextureId(GL_DEPTH_ATTACHMENT));
        lRT_DepthMap->BindForDrawning();
        lRT_DepthMap->Clear();

        glCullFace(GL_FRONT);
        for ( auto lModel : aModels3D ) /* go through ALL models because even
            if a model is not visible, its shadow may still be visible. */
            if ( lModel->IsEnabled() && lModel->IsShadowCaster() )
            {
                const auto lMVP = aLight->GetViewProjectionMatrix() * lModel->GetModelMatrix();
                aShader->SetUniformMat4("u_MVPMatrix", &lMVP);
                RenderModel3D_core(*lModel, RENDER_MODEL__NO_MATERIAL_FLAG); // to depth map of directional light
            }
        glCullFace(GL_BACK);

        lRT_DepthMap->Unbind();
        //aShader->Detach();
    }


    void Renderer::SetupShaderShadowParams(const Light* aLight, const Shader* aShader) const
    {
        //aShader->Attach();
        glm::mat4 lLightViewProjMatrix_scaled_biased(0.0f);
        if ( aLight->GetShadowMap() ) // if there are shadows from this light source
        {
            static const auto sScaleBiasMatrix = glm::mat4(
                0.5f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.5f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.5f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f);
            lLightViewProjMatrix_scaled_biased = sScaleBiasMatrix * aLight->GetViewProjectionMatrix();
            aShader->SetUniformTexture2D("u_shadowMap", 3);
            auto lTexId = aLight->GetShadowMap()->GetTextureId(GL_DEPTH_ATTACHMENT);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, lTexId);
            ASSERT(glIsTexture(lTexId));
            //__(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            //__(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            //__(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
            //__(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE));
        }
        // else no shadows from this light source, keep zero LightViewProjMatrix_scaled_biased
        aShader->SetUniformMat4("u_LVPMatrix_sb", &lLightViewProjMatrix_scaled_biased);
        //aShader->Detach();
    }


    void Renderer::RenderScene3D(const Scene &aScene) const
    {
        // Collect visible models
        float lAvgRadius = 0.0f;
        vector<Model3D*> lVisible3DModels;
        lVisible3DModels.reserve(aScene.GetModels3D().size());
        for (auto lModel : aScene.GetModels3D())
        {
            if ( lModel->IsEnabled() && aScene.GetActiveCamera()->IsObjectVisible(*lModel) )
            {
                lVisible3DModels.push_back(lModel);
                lAvgRadius += lModel->GetBoundingSphere().GetRadius() / glm::length(lModel->GetScaleFactor());
            }
        }
        lAvgRadius /= aScene.GetModels3D().size();

        const DirectLight* lDirectLight = aScene.GetDirectLight();
        if ( lDirectLight && !lDirectLight->IsEnabled() )
            lDirectLight = nullptr;
        // Determine the lights visibility
        vector<const PointLight*> lVisiblePointLights;
        vector<const SpotLight*> lVisibleSpotLights;
        for (auto lPointLight : aScene.GetPointLights())
        {
            if ( lPointLight->IsEnabled() && aScene.GetActiveCamera()->IsObjectVisible(*lPointLight) )
                lVisiblePointLights.push_back(lPointLight);
        }

        for (auto lSpotLight : aScene.GetSpotLights())
        {
            if ( lSpotLight->IsEnabled() && aScene.GetActiveCamera()->IsObjectVisible(*lSpotLight) )
                lVisibleSpotLights.push_back(lSpotLight);
        }

        // Rendering shadows to depth maps
        const Shader* lDepthShader = Engine::Instance()->ResourceManager().FindShader("Depth");
        ASSERT(lDepthShader);
        lDepthShader->Attach();

        if ( lDirectLight &&
             lDirectLight->GetShadowMap() ) // if there are shadows from this light source
        { /* z position in light volume must be chosen so that all
             shadow caster objects are entirely 'visible' from that point */
            constexpr float AREA_SIDE_X = 500.0f + 200.0f;
            constexpr float AREA_SIDE_Y = 500.0f + 50.0f;
            constexpr float MAX_ELEVATION = 1500.0f;
            lDirectLight->mProjectionVolume = glm::vec3(AREA_SIDE_X, AREA_SIDE_Y, MAX_ELEVATION);
            RenderToShadowMap(lDirectLight, aScene.GetModels3D(), lDepthShader);
        }

        // Render spot light shadows
        for ( auto lLight : lVisibleSpotLights )
        {
            if( lLight->GetShadowMap() ) // if there are shadows from this light source
                RenderToShadowMap(lLight, aScene.GetModels3D(), lDepthShader);            
        }

        // Render point light shadows
        for ( auto lLight : lVisiblePointLights )
        { 
            if ( lLight->GetShadowMap() ) // if there are shadows from this light source
                RenderToShadowMap(lLight, aScene.GetModels3D(), lDepthShader);
        }

        lDepthShader->Detach();

        // Done rendering shadow maps
        auto lRT_GBuffer = aScene.GetRenderTarget("GBuffer");
        if ( !lRT_GBuffer )
            CRASH("Scene '%s' does not have a GBuffer render target", aScene.GetName().c_str());
        // Geometry Pass - Render visible models
        // Note: Geometry pass is the only pass that updates depth buffer
        lRT_GBuffer->BindForDrawning();
        lRT_GBuffer->Clear();

        auto lShader = Engine::Instance()->ResourceManager().FindShader("DeferredShading_GeometryPass");
        if ( !lShader )
            CRASH("Deferred lighting shader/Geomerty pass was not found in Resource Manager");
        const glm::mat4 lViewProjectionMatrix = aScene.GetActiveCamera()->GetProjectionMatrix() * aScene.GetActiveCamera()->GetViewMatrix();

        const LightingInfo lLightingInfo{
            aScene.GetDirectLight(),
            lVisiblePointLights,
            lVisibleSpotLights,
            0.05f };

        for ( auto lModel : lVisible3DModels )
        {
            const glm::mat4 lModelViewProjectionMatrix = lViewProjectionMatrix * lModel->GetModelMatrix();

            uint32_t lFlags = 0;
            if ( std::find(aScene.mSelectedModels.begin(), aScene.mSelectedModels.end(), lModel) != aScene.mSelectedModels.end() )
                lFlags = RENDER_MODEL__RENDER_SELECTED_FLAG;
            else if ( aScene.mFocusedModel == lModel )
                lFlags = RENDER_MODEL__RENDER_FOCUSED_FLAG;
            RenderModel3D_GPass(*lModel, lModelViewProjectionMatrix, lShader,
                lFlags, lLightingInfo);

            // Render overlay wireframe if requested
            if ( GetWireframeMode() == Renderer::RENDER_WIREFRAME_OVERLAY )
                RenderModel3DWireframe_GPass(*lModel, lModelViewProjectionMatrix, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

            // Render required debug info:
            // Render normals information
            if ( lModel->GetRenderNormals() || this->GetRenderNormals() )
                RenderModelNormals(*lModel, lModelViewProjectionMatrix, lAvgRadius * 0.02f);
            // Render bounding volumes information
            RenderBoundingVolumes(*lModel, *aScene.GetActiveCamera(),
                lModel->GetRenderBoundingSphere() || this->GetRenderBoundingSphere(),
                lModel->GetRenderAABB() || this->GetRenderAABB(), lModel->GetRenderOOBB() || this->GetRenderOOBB());
        }

        // Render Grid if presents
        if (aScene.GetGrid())
        {
            glEnable(GL_DEPTH_TEST);
            RenderGrid3D(// Major
                aScene.GetGrid()->GetMajorStep3D(),
                aScene.GetGrid()->GetSize3D(),
                glm::vec4(aScene.GetGrid()->GetColor(), 1.0f),
                aScene.GetGrid()->GetMajorLineWidth()+1,
                *aScene.GetActiveCamera());
            RenderGrid3D(// Minor
                aScene.GetGrid()->GetMinorStep3D(),
                aScene.GetGrid()->GetSize3D(),
                glm::vec4(aScene.GetGrid()->GetColor(), 1.0f),
                aScene.GetGrid()->GetMinorLineWidth()+1,
                *aScene.GetActiveCamera());

            glEnable(GL_DEPTH_TEST);
        }

        // Render a transform gizmo if necessary
        if ( aScene.mModelUnderTransform.mTarget &&
            !(aScene.mModelUnderTransform.mTarget->mFlags & Model::ModelFlags::eDISABLE_TRASFORM_3D) )
        {
            uint32_t lGizmoType = static_cast<uint32_t>(aScene.mModelUnderTransform.mTransformType);
            Model3D* lModel3D = dynamic_cast<Model3D*>(aScene.mModelUnderTransform.mTarget);
            GLenum lColorBuffers[4] = { GL_COLOR_ATTACHMENT0/*diffuse*/, GL_COLOR_ATTACHMENT1/*modelId*/, GL_NONE/*position*/, GL_COLOR_ATTACHMENT3/*normal*/ };
            //glDrawBuffers(4, lColorBuffers); // don't draw to position buffer
            glDisable(GL_DEPTH_TEST);
            RenderTransformGizmo(lGizmoType, lModel3D, *aScene.GetActiveCamera());
            glEnable(GL_DEPTH_TEST);
            lColorBuffers[2] = GL_COLOR_ATTACHMENT2;
            //glDrawBuffers(4, lColorBuffers); // draw to all buffers
        }

        { // Render debug info for spot and point lights:
            vector<const Light*> lLightsMarkers;
            for ( auto lLight : lVisibleSpotLights )
            {
                RenderBoundingVolumes(*lLight, *aScene.GetActiveCamera(),
                    lLight->GetRenderBoundingSphere() || this->GetRenderBoundingSphere(),
                    lLight->GetRenderAABB() || this->GetRenderAABB(),
                    lLight->GetRenderOOBB() || this->GetRenderOOBB());
                if ( lLight->GetRenderMarker() || this->GetRenderLightsMarkers() )
                    lLightsMarkers.push_back(lLight);
            }
            for ( auto lLight : lVisiblePointLights )
            {
                RenderBoundingVolumes(*lLight, *aScene.GetActiveCamera(),
                    lLight->GetRenderBoundingSphere() || this->GetRenderBoundingSphere(),
                    lLight->GetRenderAABB() || this->GetRenderAABB(),
                    lLight->GetRenderOOBB() || this->GetRenderOOBB());
                if ( lLight->GetRenderMarker() || this->GetRenderLightsMarkers() )
                    lLightsMarkers.push_back(lLight);
            }
            if( !lLightsMarkers.empty() )
                RenderLights(lLightsMarkers, *aScene.GetActiveCamera());
        }


        // begin Lighting Passes...
        /* Note: color attachmanets are:
        0 - diffuse
        1 - color picking (modelID)
        2 - position
        3 - normal + lighting flag in w:
          0 = fragment is not subject to lighting,
          1 = fragment is subject to lighting but not a shadow receiver,
          2 = fragment is subject to lighting and a shadow receiver */

        auto lRT_PostProcessing = aScene.GetRenderTarget("NoAA");
        if ( !lRT_PostProcessing )
            CRASH("Scene '%s' does not have a NoAA render target", aScene.GetName().c_str());
        
        lRT_PostProcessing->BindForDrawning(); // now draw to a different RT
        lRT_PostProcessing->Clear();

        // copy modelId buffer from GBuffer RT to NoAA RT
        {
            GLenum lColorBuffers[2] = { GL_NONE/*main*/, GL_COLOR_ATTACHMENT1/*modelId*/ };
            /*glDrawBuffers(2, lColorBuffers);
            lRT_GBuffer->Blit(0,0, lRT_GBuffer->GetSize().x, lRT_GBuffer->GetSize().y, 1, false);
            lColorBuffers[0] = GL_COLOR_ATTACHMENT0;
            glDrawBuffers(2, lColorBuffers);*/
        }

        // When we get here the depth buffer is already populated and the stencil pass
        // depends on it, but it does not write to it.
        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        GLenum lColorBuffers[2] = { GL_COLOR_ATTACHMENT0/*main*/, GL_COLOR_ATTACHMENT1/*modelId*/ };
        glDrawBuffers(1, lColorBuffers); // don't draw to modelId buffer

        const glm::mat4 ViewProjectionMatrix = aScene.GetActiveCamera()->GetProjectionMatrix() * aScene.GetActiveCamera()->GetViewMatrix();
        glm::mat4 MVP(1.0f); // identity
        glm::vec2 lScreenSize(lRT_GBuffer->GetSize().x, lRT_GBuffer->GetSize().y); // TODO...

        // Ambient - always
        lShader = Engine::Instance()->ResourceManager().FindShader("DeferredShading_AmbientPass");
        if ( !lShader )
            CRASH("Deferred lighting shader/Ambient pass was not found in Resource Manager");
        lShader->Attach();
        lShader->SetUniformTexture2D("u_diffuseMap", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, lRT_GBuffer->GetTextureId(GL_COLOR_ATTACHMENT0));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        lShader->SetUniformTexture2D("u_normalMap", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, lRT_GBuffer->GetTextureId(GL_COLOR_ATTACHMENT3));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        lShader->SetUniformVec2("u_screenSize", lScreenSize);
        glBindVertexArray(((OpenGLRenderer*)this)->GetTempVAO());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        //glBindVertexArray(GL_NONE);
        ASSERT(glGetError() == GL_NO_ERROR);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
        lShader->Detach();

        // Directional light - if needed
        const Light* lLight = aScene.GetDirectLight();
        if ( lLight && lLight->IsEnabled() )
        {
            lShader = Engine::Instance()->ResourceManager().FindShader("DeferredShading_DirectionalLightPass");
            if ( !lShader )
                CRASH("Deferred lighting shader/Directional light pass was not found in Resource Manager");
            lShader->Attach();
            lShader->SetUniformTexture2D("u_diffuseMap", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, lRT_GBuffer->GetTextureId(GL_COLOR_ATTACHMENT0));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            lShader->SetUniformTexture2D("u_positionMap", 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, lRT_GBuffer->GetTextureId(GL_COLOR_ATTACHMENT2));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            lShader->SetUniformTexture2D("u_normalMap", 2);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, lRT_GBuffer->GetTextureId(GL_COLOR_ATTACHMENT3));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            lShader->SetUniformVec2("u_screenSize", lScreenSize);
            glBindVertexArray(((OpenGLRenderer*)this)->GetTempVAO());
            glm::vec3 pos = glm::normalize(lLight->GetPosition());
            ASSERT_vec3_is_normalized(pos);
            lShader->SetUniformVec3("u_LightPosition", pos); // = any_vertex-to-light vector
            lShader->SetUniformVec4("u_LightColor", glm::vec4(lLight->GetDiffuse(), 1.0f));
            SetupShaderShadowParams(lLight, lShader);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            //glBindVertexArray(GL_NONE);
            ASSERT(glGetError() == GL_NO_ERROR);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, GL_NONE);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, GL_NONE);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, GL_NONE);
            lShader->Detach();
        }

        // Spot lights - if needed
        if ( !lVisibleSpotLights.empty() )
        {
            lShader = Engine::Instance()->ResourceManager().FindShader("DeferredShading_SpotLightPass");
            if ( !lShader )
                CRASH("Deferred lighting shader/Spot light pass was not found in Resource Manager");
            lShader->Attach();
            lShader->SetUniformTexture2D("u_diffuseMap", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, lRT_GBuffer->GetTextureId(GL_COLOR_ATTACHMENT0));
            lShader->SetUniformTexture2D("u_positionMap", 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, lRT_GBuffer->GetTextureId(GL_COLOR_ATTACHMENT2));
            lShader->SetUniformTexture2D("u_normalMap", 2);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, lRT_GBuffer->GetTextureId(GL_COLOR_ATTACHMENT3));
            lShader->SetUniformVec2("u_screenSize", lScreenSize);
            for ( auto lLight : lVisibleSpotLights )
            {
                MVP = ViewProjectionMatrix * lLight->GetModelMatrix();
                //lShader->SetUniformMat4("u_MVPMatrix", &MVP); // position and radius for sphere defining a spot light
                lShader->SetUniformVec3("u_LightPosition", lLight->GetPosition());
                lShader->SetUniformVec4("u_LightColor", glm::vec4(lLight->GetDiffuse(), 1.0f));
                lShader->SetUniformFloat("u_LightRadius", lLight->GetCutoff());
                ASSERT_vec3_is_normalized(lLight->GetDirection());
                lShader->SetUniformVec3("u_LightDirection", lLight->GetDirection());
                lShader->SetUniformFloat("u_LightConeAngle", lLight->GetConeAngle());
                //glDrawElements(GL_TRIANGLES, sUnitSphereIndexCount, GL_UNSIGNED_INT, 0); 
                SetupShaderShadowParams(lLight, lShader);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
        }

        // Point lights - if needed
        if ( !lVisiblePointLights.empty() )
        {
            // ---- TODO: move is somewhere else ------
            /*static GLuint sUnitSphereVA = GL_NONE;
            static GLuint sUnitSphereIndexCount = 0;
            if ( sUnitSphereVA == GL_NONE )
            {
                Procedural::Sphere lSphereObject(1.0f);
                const auto& lVertexData = lSphereObject.GetVertexData();
                vector<glm::vec3> lVertices(lVertexData.size());
                for ( size_t i = 0; i < lVertexData.size(); ++i )
                    lVertices[i] = lVertexData[i].mVertex; // get vertex positions only

                GLuint lDataBuf = GL_NONE;
                glGenBuffers(1, &lDataBuf);
                glBindBuffer(GL_ARRAY_BUFFER, lDataBuf);
                glBufferData(GL_ARRAY_BUFFER, lVertices.size() * sizeof(lVertices[0]), lVertices.data(), GL_STATIC_DRAW);
                GLuint lIndexBuf = GL_NONE;
                glGenBuffers(1, &lIndexBuf);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lIndexBuf);
                ASSERT(lSphereObject.GetIndicesCount().size() == 1);
                sUnitSphereIndexCount = lSphereObject.GetIndicesCount()[0];
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sUnitSphereIndexCount * sizeof(uint32_t), lSphereObject.GetIndexData().data(), GL_STATIC_DRAW);

                glGenVertexArrays(1, &sUnitSphereVA);
                glBindVertexArray(sUnitSphereVA);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

                glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
                glBindVertexArray(GL_NONE);
            }
            // ----------------------------*/
            lShader = Engine::Instance()->ResourceManager().FindShader("DeferredShading_PointLightPass");
            if ( !lShader )
                CRASH("Deferred lighting shader/Point light pass was not found in Resource Manager");
            lShader->Attach();
            lShader->SetUniformTexture2D("u_diffuseMap", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, lRT_GBuffer->GetTextureId(GL_COLOR_ATTACHMENT0));
            lShader->SetUniformTexture2D("u_positionMap", 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, lRT_GBuffer->GetTextureId(GL_COLOR_ATTACHMENT2));
            lShader->SetUniformTexture2D("u_normalMap", 2);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, lRT_GBuffer->GetTextureId(GL_COLOR_ATTACHMENT3));
            lShader->SetUniformVec2("u_screenSize", lScreenSize);
            //glBindVertexArray(sUnitSphereVA);
            for ( auto lLight : lVisiblePointLights )
            {
                MVP = ViewProjectionMatrix * lLight->GetModelMatrix();
                //lShader->SetUniformMat4("u_MVPMatrix", &MVP); // position and radius for sphere defining a point light
                lShader->SetUniformVec3("u_LightPosition", lLight->GetPosition());
                lShader->SetUniformVec4("u_LightColor", glm::vec4(lLight->GetDiffuse(), 1.0f));
                lShader->SetUniformFloat("u_LightRadius", lLight->GetCutoff());
                //glDrawElements(GL_TRIANGLES, sUnitSphereIndexCount, GL_UNSIGNED_INT, 0); 
                SetupShaderShadowParams(lLight, lShader);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
            //glBindVertexArray(GL_NONE);
            ASSERT(glGetError() == GL_NO_ERROR);
            lShader->Detach();
        }
    }

    void Renderer::RenderDebug(const Scene &aScene) const
    {
        if (!mRenderDebugInfo)
            return;

        __(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        const glm::uvec3 lFPSLayout(900, 15, 0);
        const glm::uvec3 lRTLayout(700, 15, 0);
        const glm::vec4& lTextColor = Engine::Instance()->Config().GetFontInfo().color;
        
        string lAttachmentName = "";
        std::vector<RenderTarget*> lRenderTargets = aScene.GetRenderTargets();

        if (mRenderTargetIdx >= lRenderTargets.size())
        {
            if (mRenderTargetIdx == lRenderTargets.size()) lAttachmentName = "ShadowMap-DirectLight";
            if (mRenderTargetIdx == lRenderTargets.size() + 1) lAttachmentName = "ShadowMap-PointLight-" + std::to_string(mRenderShadowMapIdx);
            if (mRenderTargetIdx == lRenderTargets.size() + 2) lAttachmentName = "ShadowMap-SpotLight-" + std::to_string(mRenderShadowMapIdx);
            
            Renderer::sFontRenderer->RenderText((lAttachmentName + mRenderLightShadowMap->GetName()).c_str(), 0.3f, lRTLayout, lTextColor, *lRenderTargets.at(0));
        }
        else
        {
            int lAttachmentIdx = GL_COLOR_ATTACHMENT0 + mRenderAttachmentIdx;
            if (lRenderTargets.at(mRenderTargetIdx)->GetName().compare("GBuffer") == 0)
            {
                if (lAttachmentIdx == GL_COLOR_ATTACHMENT0) lAttachmentName = " - Diffuse";
                else if (lAttachmentIdx == GL_COLOR_ATTACHMENT1) lAttachmentName = " - Object ID";
                else if (lAttachmentIdx == GL_COLOR_ATTACHMENT2) lAttachmentName = " - Normals";
            }

            Renderer::sFontRenderer->RenderText(FPS::GetInstance()->GetFPSInfo().c_str(), 0.3f, lFPSLayout, lTextColor, *lRenderTargets.at(mRenderTargetIdx));
            Renderer::sFontRenderer->RenderText((lRenderTargets.at(mRenderTargetIdx)->GetName() + lAttachmentName).c_str(), 0.3f, lRTLayout, lTextColor, *lRenderTargets.at(mRenderTargetIdx));
        }
    }
}
