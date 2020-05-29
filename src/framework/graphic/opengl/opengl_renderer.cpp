/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential

 * Brief        : OpenGL based 3D renderer

********************************************************************************/

#include "precompiled.h"
#include "graphic/shader.h"
#include "graphic/opengl/opengl.h"
#include "graphic/opengl/opengl_resources.h"
#include "graphic/opengl/opengl_renderer.h"
#include "engine.h"

namespace Framework
{

    bool OpenGLRenderer::Init()
    {
        ASSERT(mTempVBO == GL_NONE);
        INFO(LogLevel::eLEVEL2, "Renderer: %s", GetName());
        INFO(LogLevel::eLEVEL2, "Version: %s", GetVersion());
        INFO(LogLevel::eLEVEL2, "Vendor: %s", GetVendor());
        INFO(LogLevel::eLEVEL2, "Shading language version: %s", GetShaderVersion());

        string lError;
        //__(glClearColor(0.0, 0.0, 0.0, 1.0));
        /* There seems to be a bug with sRGB default framebuffer, as
         * the buffer is always linear, but if we enable this the
         * drivers are doing the linear->sRGB conversion anyway!
         * So don't enable it for now, we will have to compensate
         * in the shaders */
         // __(glEnable(GL_FRAMEBUFFER_SRGB));

        /* Generate a fake texture to fullfill GLSL 3.3 requirement
         * on some cards that need all samplers to be bound to a valid
         * texture, even if they are not Used */
        __(glGenTextures(1, &mDummyTexture));

        /* Also generate a texture for no-shadow receivers that is
         * completely white */
        __(glGenTextures(1, &mNoShadowTexture));
        __(glBindTexture(GL_TEXTURE_2D, mNoShadowTexture));
        {
            unsigned char lNoShadow[3] = { 255, 255, 255 };
            __(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
            __(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, lNoShadow));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        }
        __(glBindTexture(GL_TEXTURE_2D, GL_NONE));

        /* TODO: Once we Use our own format, this should not be needed */
        __(glBindTexture(GL_TEXTURE_2D, mDummyTexture));
        {
            __(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        }
        __(glBindTexture(GL_TEXTURE_2D, GL_NONE));

        /* Create the shader to render light debug billboard */
        if ( mRenderLightShader.Load("utils/render_light", lError) != true )
            CRASH("ERROR loading utils/render_light shader: %s\n", lError.c_str());

        /* Create the shader to render bounding volume */
        if ( mRenderBoundingVolume.Load("utils/bounding_volume", lError) != true )
            CRASH("ERROR loading shader utils/bounding_volume: %s\n", lError.c_str());

        /* Create the shader to render normals information */
        if ( mRenderNormals.Load("utils/render_normals", lError) != true )
            CRASH("ERROR loading utils/render_normals shader: %s\n", lError.c_str());

        /* Create the wireframe shader */
        if ( !mWireframeShader.Load("utils/render_solidcolor", lError) )
            CRASH("Failed to load render_solidcolor shader: %s", lError.c_str());

        /* Create the shader to render shadowmaps */
        if ( !PrepareForRendering(mShadowMapQuad) )
            CRASH("Failed to prepare model %s\n", mShadowMapQuad.GetName().c_str());
        //TODO!!!mShadowMapQuad->Disable(); // ???

        if ( !mRenderShadowMapQuad.Load("utils/render_shadowmap_quad", lError) )
            CRASH("Failed to load render_shadowmap_quad shader: %s", lError.c_str());

        __(glGenVertexArrays(1, &mTempVAO));
        __(glBindVertexArray(mTempVAO));
        __(glGenBuffers(1, &mTempVBO));
        __(glBindBuffer(GL_ARRAY_BUFFER, mTempVBO));
        __(glBindBuffer(GL_ARRAY_BUFFER, GL_NONE));
        __(glBindVertexArray(GL_NONE));

        return glGetError() == GL_NO_ERROR;
    }

    OpenGLRenderer::~OpenGLRenderer()
    {
        if ( mDummyTexture != GL_NONE )
            glDeleteTextures(1, &mDummyTexture);
        if ( mNoShadowTexture != GL_NONE )
            glDeleteTextures(1, &mNoShadowTexture);
        if ( mTempVBO != GL_NONE )
            __(glDeleteBuffers(1, &mTempVBO));
        if ( mTempVAO != GL_NONE )
            __(glDeleteVertexArrays(1, &mTempVAO));
    }

    const char* OpenGLRenderer::GetName() const { return (const char *)glGetString(GL_RENDERER); }
    const char* OpenGLRenderer::GetVersion() const { return (const char *)glGetString(GL_VERSION); }
    const char* OpenGLRenderer::GetVendor() const { return (const char *)glGetString(GL_VENDOR); }
    const char* OpenGLRenderer::GetShaderVersion() const { return (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION); }

    Shader* OpenGLRenderer::NewShader(const std::string& aName) const
    { return new OpenGLShader(aName); }

    bool OpenGLRenderer::PrepareForRendering(Asset2D& aAsset) const
    {
        if ( aAsset.mRendererResources )
            return true;
        aAsset.mRendererResources = std::make_unique<OpenGLResources>(aAsset);
        if ( static_cast<const OpenGLResources*>(aAsset.mRendererResources.get())->GetVertexArrayID() != GL_NONE )
        {
            aAsset.RenderReady();
            return true;
        }
        aAsset.mRendererResources.reset();
        return false;
    }

    bool OpenGLRenderer::PrepareForRendering(Asset3D& aAsset) const
    {
        if ( aAsset.mRendererResources )
            return true;
        aAsset.mRendererResources = std::make_unique<OpenGLResources>(aAsset);
        if ( static_cast<const OpenGLResources*>(aAsset.mRendererResources.get())->GetVertexArrayID() != GL_NONE )
        {
            aAsset.RenderReady();
            return true;
        }
        aAsset.mRendererResources.reset();
        return false;
    }

    void OpenGLRenderer::RenderTexturedQuad(const Model2D &aModel, const glm::vec2& aImageOffset, const glm::vec2& aImageScale/*, RenderTarget &aRenderTarget*/) const
    {   // what is that function for ?
        ASSERT(0);
    }


    void OpenGLRenderer::RenderShadowMapQuad(const glm::vec2& aQuadOffset, const glm::vec2& aQuadScale, const Camera* aCamera, RenderTarget* aShadowMap, RenderTarget* aRenderTarget) const
    {
        __(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE));

        mRenderShadowMapQuad.Attach();
        mRenderShadowMapQuad.SetUniformVec2("u_scale", aQuadScale);
        mRenderShadowMapQuad.SetUniformVec2("u_offset", aQuadOffset);
        mRenderShadowMapQuad.SetUniformFloat("u_v1_near_plane", aCamera->GetZNear());
        mRenderShadowMapQuad.SetUniformFloat("u_v1_far_plane", aCamera->GetZFar());
        mRenderShadowMapQuad.SetUniformTexture2D("u_s2dsm_shadowmap", 0);

        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        const OpenGLResources* lGLRes = static_cast<const OpenGLResources*>(mShadowMapQuad.mRendererResources.get());
        __(glBindVertexArray(lGLRes->GetVertexArrayID()));
        {
            __(glActiveTexture(GL_TEXTURE0));
            __(glBindTexture(GL_TEXTURE_2D, aShadowMap->GetTextureId(GL_DEPTH_ATTACHMENT)));
            __(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
        }
        __(glBindVertexArray(GL_NONE));

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        mRenderShadowMapQuad.Detach();
    }


    void OpenGLRenderer::RenderModel2D(const Model2D &aModel, const glm::mat4& aViewProjectionMatrix, const Shader *aShader,
        uint32_t aFlags) const
    {
        if ( aFlags & (RENDER_MODEL__RENDER_FOCUSED_FLAG | RENDER_MODEL__RENDER_SELECTED_FLAG) )
        {
            // Set the clear value for the stencil buffer, then clear it, enable stencil test
            __(glClearStencil(0));
            __(glClear(GL_STENCIL_BUFFER_BIT));
            __(glEnable(GL_STENCIL_TEST));
            // Set the stencil buffer to write a 1 in every time a pixel is written to the screen
            __(glStencilFunc(GL_ALWAYS, 1, 0xFFFF));
            __(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
            // Proceed to render the model normally...
        }

        // Set shader program parameters
        const OpenGLResources* lGLRes = static_cast<const OpenGLResources*>(aModel.GetAsset2D()->mRendererResources.get());
        aShader->Attach();
        const glm::mat4 lModelViewProjectionMatrix = aViewProjectionMatrix * aModel.GetModelMatrix();
        aShader->SetUniformMat4("u_MVPMatrix", &lModelViewProjectionMatrix);
        aShader->SetUniformTexture2D("u_shapeMap", 0);
        aShader->SetUniformTexture2D("u_overlayMap", 1);
        aShader->SetUniformBool("u_useOverlayMap", lGLRes->GetTexturesIDs().size() > 1);
        uint32_t lColor = aModel.GetId();
        uint8_t* lColorBytes = (uint8_t*)&lColor;
        glm::vec3 lModelColor(lColorBytes[0] / 255.0f, lColorBytes[1] / 255.0f, lColorBytes[2] / 255.0f);
        ASSERT(lColorBytes[3] == 0xFF);
        aShader->SetUniformVec3("u_modelId", lModelColor);

        // Set render state and Draw the model
        if ( GetWireframeMode() == Renderer::RENDER_WIREFRAME_ONLY )
        {
            __(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
            __(glDisable(GL_CULL_FACE));
            __(glEnable(GL_LINE_SMOOTH));
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        __(glBindVertexArray(lGLRes->GetVertexArrayID()));
        {
            ASSERT(1 <= lGLRes->GetTexturesIDs().size() && lGLRes->GetTexturesIDs().size() <= 2);
            __(glActiveTexture(GL_TEXTURE0));
            __(glBindTexture(GL_TEXTURE_2D, lGLRes->GetTexturesIDs()[0]));
            ASSERT(glIsTexture(lGLRes->GetTexturesIDs()[0]));
            if ( lGLRes->GetTexturesIDs().size() > 1 )
            { // an overlay texture is available
                __(glActiveTexture(GL_TEXTURE1));
                __(glBindTexture(GL_TEXTURE_2D, lGLRes->GetTexturesIDs()[1]));
                ASSERT(glIsTexture(lGLRes->GetTexturesIDs()[1]));
            }
            __(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
        }
        __(glBindVertexArray(GL_NONE));
        glDisable(GL_BLEND);

        aShader->Detach();

        if ( aFlags & (RENDER_MODEL__RENDER_FOCUSED_FLAG | RENDER_MODEL__RENDER_SELECTED_FLAG) )
        { // Second pass to render contour (shall be done with a different shader)
            // Set the stencil buffer to only allow writing to the screen when the value of the stencil buffer is not 1
            __(glStencilFunc(GL_NOTEQUAL, 1, 0xFFFF));
            __(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
            // Draw the object with thick lines
            uint32_t lIsSelected = (aFlags & RENDER_MODEL__RENDER_SELECTED_FLAG);
            float lLineWidth = lIsSelected ? Engine::Instance()->Config().GetSelectionLineWidth() : Engine::Instance()->Config().GetFocusLineWidth();
            const glm::vec4& lLineColor = lIsSelected ? Engine::Instance()->Config().GetSelectionLineColor() : Engine::Instance()->Config().GetFocusLineColor();
            __(glLineWidth(lLineWidth)); // Deprecated OpenGL API !
            GLenum lColorBuffers[2] = { GL_COLOR_ATTACHMENT0/*main*/, GL_COLOR_ATTACHMENT1/*modelId*/ };
            glDrawBuffers(1, lColorBuffers); // don't draw to modelId buffer
            RenderModelWireframe(&aModel, lModelViewProjectionMatrix, lLineColor);
            glDrawBuffers(2, lColorBuffers); // draw to all buffers
            __(glLineWidth(1.0f)); // Deprecated OpenGL API !
            // disable stencil test after we done
            __(glDisable(GL_STENCIL_TEST));
        }

        // Reset render state to default
        if ( GetWireframeMode() == Renderer::RENDER_WIREFRAME_ONLY )
        {
            __(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
            __(glEnable(GL_CULL_FACE));
            __(glDisable(GL_LINE_SMOOTH));
        }
    }


    void OpenGLRenderer::RenderModelWireframe(const Object* aModel, const glm::mat4& aModelViewProjectionMatrix, const glm::vec4 &aColor) const
    {
        // Set shader program parameters
        mWireframeShader.Attach();
        mWireframeShader.SetUniformMat4("u_MVPMatrix", &aModelViewProjectionMatrix);
        mWireframeShader.SetUniformVec4("u_color", aColor);

        // Set render state and draw 2D or 3D model
        __(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        __(glDisable(GL_CULL_FACE));
        __(glEnable(GL_LINE_SMOOTH));

        const Model2D* lModel2D = dynamic_cast<const Model2D*>(aModel);
        if ( lModel2D )
        {
            const OpenGLResources* lGLRes = static_cast<const OpenGLResources*>(lModel2D->GetAsset2D()->mRendererResources.get());
            __(glBindVertexArray(lGLRes->GetVertexArrayID()));
            __(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
        }
        else
        {
            const Model3D* lModel3D = dynamic_cast<const Model3D*>(aModel);
            ASSERT(lModel3D);
            RenderModel3D_core(*lModel3D, RENDER_MODEL__NO_MATERIAL_FLAG);
        }
        __(glBindVertexArray(GL_NONE));

        mWireframeShader.Detach();

        // Reset render state to default
        __(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        __(glEnable(GL_CULL_FACE));
        __(glDisable(GL_LINE_SMOOTH));
    }


    void OpenGLRenderer::RenderModel3D_core(const Model3D& aModel, uint32_t aFlags) const
    {
        const OpenGLResources* lGLRes = static_cast<const OpenGLResources*>(aModel.GetAsset3D()->mRendererResources.get());
        __(glBindVertexArray(lGLRes->GetVertexArrayID()));
        auto& lOffsets = aModel.GetAsset3D()->GetIndicesOffsets();
        auto& lCounts = aModel.GetAsset3D()->GetIndicesCount();
        auto& lTexturesIDs = lGLRes->GetTexturesIDs();
        size_t n = lOffsets.size();
        ASSERT( lCounts.size() == n &&
                (lTexturesIDs.empty() || lTexturesIDs.size() == n) );
        for ( size_t i = 0; i < n; ++i )
        {
            if ( !(lTexturesIDs.empty() || (aFlags & RENDER_MODEL__NO_MATERIAL_FLAG)) )
            {
                __(glBindTexture(GL_TEXTURE_2D, lTexturesIDs[i]));
                ASSERT(glIsTexture(lTexturesIDs[i]));
            }
            __(glDrawElements(GL_TRIANGLES, lCounts[i], GL_UNSIGNED_INT, (void*)(lOffsets[i] * sizeof(GLuint))));
        }
        __(glBindVertexArray(GL_NONE));
    }


    void OpenGLRenderer::RenderModel3D(const Model3D &aModel, const Camera &aCamera, const Shader *aShader,
        uint32_t aFlags, const LightingInfo& aLightingInfo) const
    {
        const LightingShader* lLightingShader = dynamic_cast<const LightingShader*>(aShader);
        ASSERT(lLightingShader);
        if ( !lLightingShader )
            return;

        if ( aFlags & (RENDER_MODEL__RENDER_FOCUSED_FLAG | RENDER_MODEL__RENDER_SELECTED_FLAG) )
        {
            // Set the clear value for the stencil buffer, then clear it, enable stencil test
            __(glClearStencil(0));
            __(glClear(GL_STENCIL_BUFFER_BIT));
            __(glEnable(GL_STENCIL_TEST));
            // Set the stencil buffer to write a 1 in every time a pixel is written to the screen
            __(glStencilFunc(GL_ALWAYS, 1, 0xFFFF));
            __(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
            // Proceed to render the model normally...
        }

        lLightingShader->Attach();
        const glm::mat4 lModelViewProjectionMatrix = aCamera.GetProjectionMatrix() * aCamera.GetViewMatrix() * aModel.GetModelMatrix();
        lLightingShader->SetUniformMat4("u_MVPMatrix", &lModelViewProjectionMatrix);
        GLuint lTextureUnit = 0;
        GLuint lDummyTextureUnit = 0;
        lLightingShader->SetUniformTexture2D("u_diffuseMap", lTextureUnit++);
        uint32_t lColor = aModel.GetId();
        uint8_t* lColorBytes = (uint8_t*)&lColor;
        glm::vec3 lModelColor(lColorBytes[0] / 255.0f, lColorBytes[1] / 255.0f, lColorBytes[2] / 255.0f);
        ASSERT(lColorBytes[3] == 0xFF);
        lLightingShader->SetUniformVec3("u_modelId", lModelColor);
        lLightingShader->SetUniformMat4("u_viewMatrix", &aCamera.GetViewMatrix());
        lLightingShader->SetUniformMat4("u_modelMatrix", &aModel.GetModelMatrix());
        const glm::mat3 lNormalMatrix = glm::transpose(glm::inverse(glm::mat3(aModel.GetModelMatrix())));
        lLightingShader->SetUniformMat3("u_normalMatrix", &lNormalMatrix);
        lLightingShader->SetUniformFloat("u_ambientK", aLightingInfo.mAmbientK);

        lDummyTextureUnit = lTextureUnit++;
        __(glActiveTexture(GL_TEXTURE0 + lDummyTextureUnit));
        __(glBindTexture(GL_TEXTURE_2D, mDummyTexture));

        static const glm::mat4 sBiasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0);
        // Set the aSun light
        if ( aLightingInfo.mDirectLight )
        {
            glm::mat4 lShadowMVP; // TODO if we still want to support this function... = aLightingInfo.mDirectLight->GetProjectionMatrix() * aLightingInfo.mDirectLight->GetViewMatrix() * aModel.GetModelMatrix();
            lShadowMVP = sBiasMatrix * lShadowMVP;

            lLightingShader->SetDirectLight(*aLightingInfo.mDirectLight);
            lLightingShader->SetUniformInt("u_numDirectLights", 1);

            /* TODO: This has to be set in a matrix array */
            lLightingShader->SetUniformMat4("u_shadowMVPDirectLight", &lShadowMVP);
            lLightingShader->SetUniformTexture2D("u_shadowMapDirectLight", lTextureUnit);

            __(glActiveTexture(GL_TEXTURE0 + lTextureUnit));
            if ( aModel.IsShadowReceiver() )
            {
                __(glBindTexture(GL_TEXTURE_2D, aLightingInfo.mDirectLight->GetShadowMap()->GetTextureId(GL_DEPTH_ATTACHMENT)));
            }
            else
            {
                __(glBindTexture(GL_TEXTURE_2D, mNoShadowTexture));
            }

            lTextureUnit++;
        }
        else
        {
            lLightingShader->SetUniformInt("u_numDirectLights", 0);
            lLightingShader->SetUniformTexture2D("u_shadowMapDirectLight", lDummyTextureUnit);
        }

        glm::mat4* lShadowMVPArray = (glm::mat4*)alloca(std::max(aLightingInfo.mPointLights.size(), aLightingInfo.mSpotLights.size()) * sizeof(glm::mat4));
        GLuint* lTexturesArray = (GLuint*)alloca(lLightingShader->GetMaxLights() * sizeof(GLuint));

        // Point lights
        for ( uint32_t numLight = 0; numLight < aLightingInfo.mPointLights.size(); ++numLight )
        {
            const auto lPointLight = aLightingInfo.mPointLights[numLight];
            lLightingShader->SetPointLight(numLight, *lPointLight);

            // Calculate adjusted shadow map matrix
            glm::mat4 lShadowMVP; // TODO if we still want to support this function... = lPointLight->GetProjectionMatrix() * lPointLight->GetViewMatrix() * aModel.GetModelMatrix();

            lShadowMVPArray[numLight] = sBiasMatrix * lShadowMVP;
            lTexturesArray[numLight] = lTextureUnit;

            __(glActiveTexture(GL_TEXTURE0 + lTextureUnit));
            if ( aModel.IsShadowReceiver() )
            {
                __(glBindTexture(GL_TEXTURE_2D, lPointLight->GetShadowMap()->GetTextureId(GL_DEPTH_ATTACHMENT)));
            }
            else
            {
                __(glBindTexture(GL_TEXTURE_2D, mNoShadowTexture));
            }

            lTextureUnit++;
        }
        for ( size_t numLight = aLightingInfo.mPointLights.size(); numLight < lLightingShader->GetMaxLights(); ++numLight )
            lTexturesArray[numLight] = lDummyTextureUnit;

        lLightingShader->SetUniformMat4("u_shadowMVPPointLight[0]", lShadowMVPArray, (uint32_t)aLightingInfo.mPointLights.size());
        lLightingShader->SetUniformTexture2DArray("u_shadowMapPointLight[0]", lTexturesArray, lLightingShader->GetMaxLights());
        lLightingShader->SetUniformInt("u_numPointLights", (int)aLightingInfo.mPointLights.size());

        // Spotlights
        for ( uint32_t numLight = 0; numLight < aLightingInfo.mSpotLights.size(); ++numLight )
        {
            const auto lSpotLight = aLightingInfo.mSpotLights[numLight];
            lLightingShader->SetSpotLight(numLight, *lSpotLight);

            // Calculate adjusted shadow map matrix
            glm::mat4 lShadowMVP; // TODO if we still want to support this function... = lSpotLight->GetProjectionMatrix() * lSpotLight->GetViewMatrix() * aModel.GetModelMatrix();

            lShadowMVPArray[numLight] = sBiasMatrix * lShadowMVP;
            lTexturesArray[numLight] = lTextureUnit;

            __(glActiveTexture(GL_TEXTURE0 + lTextureUnit));
            if ( aModel.IsShadowReceiver() )
            {
                __(glBindTexture(GL_TEXTURE_2D, lSpotLight->GetShadowMap()->GetTextureId(GL_DEPTH_ATTACHMENT)));
            }
            else
            {
                __(glBindTexture(GL_TEXTURE_2D, mNoShadowTexture));
            }

            lTextureUnit++;
        }
        for ( size_t numLight = aLightingInfo.mSpotLights.size(); numLight < lLightingShader->GetMaxLights(); ++numLight )
            lTexturesArray[numLight] = lDummyTextureUnit;

        lLightingShader->SetUniformMat4("u_shadowMVPSpotLight[0]", lShadowMVPArray, (uint32_t)aLightingInfo.mSpotLights.size());
        lLightingShader->SetUniformTexture2DArray("u_shadowMapSpotLight[0]", lTexturesArray, lLightingShader->GetMaxLights());
        lLightingShader->SetUniformInt("u_numSpotLights", (int)aLightingInfo.mSpotLights.size());

        /* Set the shader custom parameters */
        //aShader->SetCustomParams();

        // Set render state and draw the model
        if ( GetWireframeMode() == Renderer::RENDER_WIREFRAME_ONLY )
        {
            __(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
            __(glDisable(GL_CULL_FACE));
            __(glEnable(GL_LINE_SMOOTH));
        }

        //---------- TODO: replace it with RenderModel3D_core --------
        const OpenGLResources* lGLRes = static_cast<const OpenGLResources*>(aModel.GetAsset3D()->mRendererResources.get());
        __(glBindVertexArray(lGLRes->GetVertexArrayID()));
        {
            __(glActiveTexture(GL_TEXTURE0));

            auto& lOffsets = aModel.GetAsset3D()->GetIndicesOffsets();
            auto& lCounts = aModel.GetAsset3D()->GetIndicesCount();
            auto& lTexturesIDs = lGLRes->GetTexturesIDs();
            auto& lMaterials = aModel.GetAsset3D()->GetMaterials();

            for ( size_t i = 0; i < lMaterials.size(); ++i )
            {
                __(glBindTexture(GL_TEXTURE_2D, lTexturesIDs[i]));
                ASSERT(glIsTexture(lTexturesIDs[i]));
                lLightingShader->SetMaterial(lMaterials[i]);
                __(glDrawElements(GL_TRIANGLES, lCounts[i], GL_UNSIGNED_INT, (void *)(lOffsets[i] * sizeof(GLuint))));
            }
        }
        __(glBindVertexArray(GL_NONE));
        //-----------------------------------

        aShader->Detach();

        if ( aFlags & (RENDER_MODEL__RENDER_FOCUSED_FLAG | RENDER_MODEL__RENDER_SELECTED_FLAG) )
        { // Second pass to render contour (shall be done with a different shader)
            // Set the stencil buffer to only allow writing to the screen when the value of the stencil buffer is not 1
            __(glStencilFunc(GL_NOTEQUAL, 1, 0xFFFF));
            __(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
            // Draw the object with thick lines
            uint32_t lIsSelected = (aFlags & RENDER_MODEL__RENDER_SELECTED_FLAG);
            float lLineWidth = lIsSelected ? Engine::Instance()->Config().GetSelectionLineWidth() : Engine::Instance()->Config().GetFocusLineWidth();
            const glm::vec4& lLineColor = lIsSelected ? Engine::Instance()->Config().GetSelectionLineColor() : Engine::Instance()->Config().GetFocusLineColor();
            __(glLineWidth(lLineWidth)); // Deprecated OpenGL API !
            GLenum lColorBuffers[2] = { GL_COLOR_ATTACHMENT0/*main*/, GL_COLOR_ATTACHMENT1/*modelId*/ };
            glDrawBuffers(1, lColorBuffers); // don't draw to modelId buffer
            RenderModelWireframe(&aModel, lModelViewProjectionMatrix, lLineColor);
            glDrawBuffers(2, lColorBuffers); // draw to all buffers
            __(glLineWidth(1.0f)); // Deprecated OpenGL API !
            // disable stencil test after we done
            __(glDisable(GL_STENCIL_TEST));
        }

        // Reset render state to default
        if ( GetWireframeMode() == Renderer::RENDER_WIREFRAME_ONLY )
        {
            __(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
            __(glEnable(GL_CULL_FACE));
            __(glDisable(GL_LINE_SMOOTH));
        }
    }


    void OpenGLRenderer::RenderModel3D_GPass(const Model3D &aModel, const glm::mat4& aModelViewProjectionMatrix, const Shader *aShader,
        uint32_t aFlags, const LightingInfo& aLightingInfo) const
    {
        if ( aFlags & (RENDER_MODEL__RENDER_FOCUSED_FLAG | RENDER_MODEL__RENDER_SELECTED_FLAG) )
        {
            // Set the clear value for the stencil buffer, then clear it, enable stencil test
            __(glClearStencil(0));
            __(glClear(GL_STENCIL_BUFFER_BIT));
            __(glEnable(GL_STENCIL_TEST));
            // Set the stencil buffer to write a 1 in every time a pixel is written to the screen
            __(glStencilFunc(GL_ALWAYS, 1, 0xFF));
            __(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
            // Proceed to render the model normally...
        }

        aShader->Attach();
        aShader->SetUniformMat4("u_MVPMatrix", &aModelViewProjectionMatrix);
        aShader->SetUniformMat4("u_ModelMatrix", &aModel.GetModelMatrix());
        
        aShader->SetUniformTexture2D("u_diffuseMap", 0);
        uint32_t lColor = aModel.GetId();
        uint8_t* lColorBytes = (uint8_t*)&lColor;
        glm::vec3 lModelColor(lColorBytes[0] / 255.0f, lColorBytes[1] / 255.0f, lColorBytes[2] / 255.0f);
        ASSERT(lColorBytes[3] == 0xFF);
        aShader->SetUniformVec3("u_modelId", lModelColor);
        aShader->SetUniformFloat("u_lightingFlag", aModel.IsShadowReceiver() ? 2.0f : 1.0f);

        // Draw the model
        glActiveTexture(GL_TEXTURE0);
        RenderModel3D_core(aModel, 0);

        if ( aFlags & (RENDER_MODEL__RENDER_FOCUSED_FLAG | RENDER_MODEL__RENDER_SELECTED_FLAG) )
        { // Second pass to render contour (shall be done with a different shader)
            // Set the stencil buffer to only allow writing to the screen when the value of the stencil buffer is not 1
            __(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
            __(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
            // Draw the object with thick lines
            uint32_t lIsSelected = (aFlags & RENDER_MODEL__RENDER_SELECTED_FLAG);
            float lLineWidth = lIsSelected ? Engine::Instance()->Config().GetSelectionLineWidth() : Engine::Instance()->Config().GetFocusLineWidth();
            const glm::vec4& lLineColor = lIsSelected ? Engine::Instance()->Config().GetSelectionLineColor() : Engine::Instance()->Config().GetFocusLineColor();
            __(glLineWidth(lLineWidth)); // Deprecated OpenGL API !
            GLenum lColorBuffers[4] = { GL_COLOR_ATTACHMENT0/*diffuse*/, GL_NONE/*modelId*/, GL_NONE/*position*/, GL_COLOR_ATTACHMENT3/*normal*/ };
            //glDrawBuffers(4, lColorBuffers); // don't draw to modelId and position buffers, but draw to normal buffer
            RenderModel3DWireframe_GPass(aModel, aModelViewProjectionMatrix, lLineColor);
            lColorBuffers[1] = GL_COLOR_ATTACHMENT1;
            lColorBuffers[2] = GL_COLOR_ATTACHMENT2;
            //glDrawBuffers(4, lColorBuffers); // draw to all buffers
            __(glLineWidth(1.0f)); // Deprecated OpenGL API !
            // disable stencil test after we done
            __(glDisable(GL_STENCIL_TEST));
        }

        aShader->Detach(); // assume will be done in calling code
    }


    void OpenGLRenderer::RenderModel3DWireframe_GPass(const Model3D &aModel, const glm::mat4 &aModelViewProjectionMatrix, const glm::vec4 &aColor) const
    {
        mWireframeShader.Attach();
        mWireframeShader.SetUniformMat4("u_MVPMatrix", &aModelViewProjectionMatrix);
        mWireframeShader.SetUniformVec4("u_color", aColor);

        // Set render state and draw the model
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
        glEnable(GL_LINE_SMOOTH);

        RenderModel3D_core(aModel, RENDER_MODEL__NO_MATERIAL_FLAG);

        mWireframeShader.Detach();

        // Reset render state to default
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
        glDisable(GL_LINE_SMOOTH);
    }


    void OpenGLRenderer::RenderTransformGizmo(uint32_t aGizmoType, const Object *aModel, const Camera &aCamera) const
    {
        const Shader* lGizmoDrawingShader = Engine::Instance()->ResourceManager().FindShader("Gizmo drawing");
        if ( !lGizmoDrawingShader )
            return;
        lGizmoDrawingShader->Attach();
        glm::mat4 lMatrix = aModel->GetModelMatrix();
        lGizmoDrawingShader->SetUniformMat4("ModelMatrix", &lMatrix);
        lMatrix = aCamera.GetProjectionMatrix() * aCamera.GetViewMatrix();
        lGizmoDrawingShader->SetUniformMat4("ViewProjectionMatrix", &lMatrix);
        const float scale = aCamera.GetProjectionType() == Projection::PERSPECTIVE ? 0.3f : 0.15f; // ad hoc values
        float lScaledDistance = glm::distance(aCamera.GetPosition(), aModel->GetPosition()) * scale;
        lGizmoDrawingShader->SetUniformFloat("ScaledDistance", lScaledDistance);
        lGizmoDrawingShader->SetUniformInt("GizmoType", (int)aGizmoType);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glFrontFace(GL_CCW);
        //glEnable(GL_CULL_FACE);
        __(glBindVertexArray(mTempVAO));
        lGizmoDrawingShader->SetUniformInt("InvocationId", 0);
        __(glDrawArrays(GL_POINTS, 0, 1)); // a dummy call for x axis
        lGizmoDrawingShader->SetUniformInt("InvocationId", 1);
        __(glDrawArrays(GL_POINTS, 0, 1)); // a dummy call for y axis
        lGizmoDrawingShader->SetUniformInt("InvocationId", 2);
        __(glDrawArrays(GL_POINTS, 0, 1)); // a dummy call for z axis
        __(glBindVertexArray(GL_NONE));
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        lGizmoDrawingShader->Detach();
    }


    void OpenGLRenderer::RenderGrid2D(float aStep, const glm::vec4 &aColor, float aLineWidth, const Camera &aCamera) const
    {
        const Shader* lGridDrawingShader = Engine::Instance()->ResourceManager().FindShader("Grid drawing 2D");
        if ( !lGridDrawingShader || aCamera.GetProjectionType() != Projection::ORTHOGRAPHIC )
            return;

        lGridDrawingShader->Attach();
        ASSERT(aCamera.GetPosition().z >= 1.0e-6f);
        glm::vec4 lOrigin(aCamera.GetPosition().x / aCamera.GetPosition().z,
            aCamera.GetPosition().y / aCamera.GetPosition().z,
            0, 1); // scaled origin position
        lOrigin = lOrigin * (aCamera.GetViewMatrix() * aCamera.GetProjectionMatrix()); // origin position in NDC
        lOrigin.x *= 10.0f; // Why - see intermediate scale factor update in RenderScene2D
        lOrigin.y *= 10.0f;
        lGridDrawingShader->SetUniformVec2("OriginPosition", glm::vec2(-lOrigin.x, -lOrigin.y));
        glm::vec2 lScaledStep;
        lScaledStep.x = aStep / aCamera.GetPosition().z;
        lScaledStep.y = lScaledStep.x * aCamera.GetAspect();
        lGridDrawingShader->SetUniformVec2("ScaledStep", lScaledStep);
        lGridDrawingShader->SetUniformVec4("Color", aColor);

        __(glBindVertexArray(mTempVAO));
        __(glLineWidth(aLineWidth)); // Deprecated OpenGL API !
        lGridDrawingShader->SetUniformInt("InvocationId", 0);
        __(glDrawArrays(GL_POINTS, 0, 1)); // a dummy call for x lines
        lGridDrawingShader->SetUniformInt("InvocationId", 1);
        __(glDrawArrays(GL_POINTS, 0, 1)); // a dummy call for y lines
        __(glLineWidth(1.0f)); // Deprecated OpenGL API !
        __(glBindVertexArray(GL_NONE));

        lGridDrawingShader->Detach();
    }

    void OpenGLRenderer::RenderGrid3D(float aStep, float aSize, const glm::vec4& aColor, float aLineWidth, const Camera& aCamera) const
    {
        const Shader* lGridDrawingShader = Engine::Instance()->ResourceManager().FindShader("Grid drawing 3D");
        if (!lGridDrawingShader || aCamera.GetProjectionType() != Projection::PERSPECTIVE)
            return;

        lGridDrawingShader->Attach();
       
        lGridDrawingShader->SetUniformFloat("ScaledStep", aStep);
        lGridDrawingShader->SetUniformFloat("GridSize", aSize);
        lGridDrawingShader->SetUniformVec4("Color", aColor);

        glm::mat4 lMatrix = aCamera.GetProjectionMatrix() * aCamera.GetViewMatrix();
        lGridDrawingShader->SetUniformMat4("ViewProjectionMatrix", &lMatrix);

        __(glBindVertexArray(mTempVAO));
        __(glLineWidth(aLineWidth)); // Deprecated OpenGL API !
        lGridDrawingShader->SetUniformInt("InvocationId", 0);
        __(glDrawArrays(GL_POINTS, 0, 1)); // a dummy call for x lines
        lGridDrawingShader->SetUniformInt("InvocationId", 1);
        __(glDrawArrays(GL_POINTS, 0, 1)); // a dummy call for y lines
        __(glLineWidth(1.0f)); // Deprecated OpenGL API !
        __(glBindVertexArray(GL_NONE));

        lGridDrawingShader->Detach();
    }

    void OpenGLRenderer::RenderLight(const Light &aLight, const Camera &aCamera, uint32_t aLightNumber) const
    {
        mRenderLightShader.Attach();
        const glm::mat4 lModelViewMatrix = aCamera.GetViewMatrix() * aLight.GetModelMatrix();
        mRenderLightShader.SetUniformMat4("u_MVMatrix", &lModelViewMatrix);
        mRenderLightShader.SetUniformMat4("u_PMatrix", &aCamera.GetProjectionMatrix());
        const glm::vec3 lAmbient = (aLight.GetAmbient() + aLight.GetDiffuse() + aLight.GetSpecular()) / 3.0f;
        mRenderLightShader.SetUniformVec3("u_lightColor", lAmbient);
        mRenderLightShader.SetUniformInt("u_lightNumber", (int)aLightNumber);

        __(glBindVertexArray(mTempVAO));
        __(glBindBuffer(GL_ARRAY_BUFFER, mTempVBO));
        // TODO: optimize: just use uniform for light position, don't mangle with VBO
        __(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &aLight.GetPosition()[0], GL_STATIC_DRAW));

        __(glEnable(GL_PROGRAM_POINT_SIZE));
        __(glDrawArrays(GL_POINTS, 0, 1));
        __(glDisable(GL_PROGRAM_POINT_SIZE));

        __(glBindVertexArray(GL_NONE));

        mRenderLightShader.Detach();
    }

    void OpenGLRenderer::RenderLights(vector<const Light*> &aLights, const Camera &aCamera) const
    {
        /* Sort the lights by its inverse proximity to the camera */
        auto light_compare = [&aCamera](const Light* aLight1, const Light* aLight2) {
            return glm::length(aCamera.GetPosition() - aLight1->GetPosition()) > glm::length(aCamera.GetPosition() - aLight2->GetPosition());
        };
        sort(aLights.begin(), aLights.end(), light_compare);

        uint32_t i = 0;
        for ( const auto aLight : aLights )
            RenderLight(*aLight, aCamera, i++);
        ++i;
    }

    void OpenGLRenderer::RenderBoundingBox(const BoundingBox &aBox, const glm::mat4 &aModelViewProjectionMatrix, const glm::vec4 &aColor) const
    {
        mRenderBoundingVolume.Attach();
        mRenderBoundingVolume.SetUniformInt("u_type", static_cast<int>(eBoundingVolume::BOX));
        mRenderBoundingVolume.SetUniformMat4("u_MVPMatrix", &aModelViewProjectionMatrix);
        mRenderBoundingVolume.SetUniformVec3("u_vertex0", aBox.GetMin());
        mRenderBoundingVolume.SetUniformVec3("u_vertex1", aBox.GetMax());
        mRenderBoundingVolume.SetUniformVec4("u_color", aColor);
        __(glBindVertexArray(mTempVAO));
        __(glDrawArrays(GL_POINTS, 0, 1)); // a dummy call
        __(glBindVertexArray(GL_NONE));
        mRenderBoundingVolume.Detach();
    }

    void OpenGLRenderer::RenderBoundingSphere(const BoundingSphere &aSphere, const glm::vec3 &aCenter, const glm::vec4 &aColor, const Camera &aCamera) const
    {
        mRenderBoundingVolume.Attach();
        mRenderBoundingVolume.SetUniformInt("u_type", static_cast<int>(eBoundingVolume::SPHERE));
        const glm::mat4 lModelViewProjectionMatrix = aCamera.GetProjectionMatrix() * aCamera.GetViewMatrix();
        mRenderBoundingVolume.SetUniformMat4("u_MVPMatrix", &lModelViewProjectionMatrix);
        mRenderBoundingVolume.SetUniformMat4("u_ProjectionMatrix", &aCamera.GetProjectionMatrix());
        mRenderBoundingVolume.SetUniformVec3("u_vertex0", aCenter);
        glm::vec3 lRadius(aSphere.GetRadius(), 0.0f, 0.0f);
        mRenderBoundingVolume.SetUniformVec3("u_vertex1", lRadius);
        mRenderBoundingVolume.SetUniformVec4("u_color", aColor);
        __(glBindVertexArray(mTempVAO));
        __(glDrawArrays(GL_POINTS, 0, 1)); // a dummy call
        __(glBindVertexArray(GL_NONE));
        mRenderBoundingVolume.Detach();
    }


    void OpenGLRenderer::RenderBoundingVolumes(const Object3D &aObject, const Camera &aCamera, bool aShowSphere, bool aShowAABB,
        bool aShowOOBB) const
    {
        if ( aShowSphere )
            RenderBoundingSphere(aObject.GetBoundingSphere(), aObject.GetPosition(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), aCamera);
        if ( aShowAABB ) {
            const glm::mat4 lModelViewProjectionMatrix = aCamera.GetProjectionMatrix() * aCamera.GetViewMatrix(); // identity model matrix
            RenderBoundingBox(aObject.GetAABB(), lModelViewProjectionMatrix, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        }
        if ( aShowOOBB ) {
            const glm::mat4 lModelViewProjectionMatrix = aCamera.GetProjectionMatrix() * aCamera.GetViewMatrix() * aObject.GetModelMatrix();
            RenderBoundingBox(aObject.GetOOBB(), lModelViewProjectionMatrix, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        }
    }


    void OpenGLRenderer::RenderModelNormals(const Model3D &aModel, const glm::mat4 &aModelViewProjectionMatrix, float aNormalSize) const
    {
        mRenderNormals.Attach();
        mRenderNormals.SetUniformMat4("u_MVPMatrix", &aModelViewProjectionMatrix);
        mRenderNormals.SetUniformFloat("u_normalSize", aNormalSize);

        RenderModel3D_core(aModel, RENDER_MODEL__NO_MATERIAL_FLAG);

        mRenderNormals.Detach();
    }

    /*bool OpenGLRenderer::Resize(uint16_t aWidth, uint16_t aHeight)
    {
        mWidth = aWidth;
        mHeight = aHeight;
        return true;
    }*/

    void OpenGLRenderer::Flush()
    {
        glFinish();
    }

    /*void OpenGLRenderer::Clear()
    {
        __(glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE));
        __(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        __(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }*/
}
