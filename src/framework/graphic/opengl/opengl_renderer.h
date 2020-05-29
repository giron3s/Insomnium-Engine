/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
   Brief       : OpenGL based 3D renderer
*******************************************************************************/

#pragma once

#include <vector>
#include "graphic/renderer.h"
#include "graphic/opengl/opengl_shader.h"
#include "graphic/procedural/texturedquad.h"

namespace Framework
{
    class OpenGLRenderer : public Renderer
    {
      public:
        OpenGLRenderer()
            : mShadowMapQuad(1,1)
        {}

        ~OpenGLRenderer() override;
        /**
         * Renderer methods
         */
        bool        Init(void) override;
        const char* GetName() const override;
        const char* GetVersion() const override;
        const char* GetVendor() const override;
        const char* GetShaderVersion() const override;

        Shader*     NewShader(const std::string& aName) const override;
        bool        PrepareForRendering(Asset2D& aAsset) const override;
        bool        PrepareForRendering(Asset3D& aAsset) const override;

        void        RenderShadowMapQuad(const glm::vec2& aQuadOffset, const glm::vec2& aQuadScale, const Camera* aCamera, RenderTarget* aShadowMap, RenderTarget* aRenderTarget) const override;

        void        RenderTexturedQuad(const Model2D &aModel, const glm::vec2& aImageOffset, const glm::vec2& aImageScale) const override;

        void        RenderModel2D(const Model2D &aModel, const glm::mat4& aViewProjectionMatrix, const Shader *aShader,
                                  uint32_t aFlags) const override;

        void        RenderModelWireframe(const Object* aModel, const glm::mat4& aViewProjectionMatrix, const glm::vec4 &aColor) const override;

        void        RenderModel3D_core(const Model3D& aModel, uint32_t aFlags) const override;

        void        RenderModel3D(const Model3D &aModel, const Camera &aCamera, const Shader *aShader,
                                  uint32_t aFlags, const LightingInfo& aLightingInfo) const override;

        void        RenderModel3D_GPass(const Model3D &aModel, const glm::mat4& aModelViewProjectionMatrix, const Shader *aShader,
                                        uint32_t aFlags, const LightingInfo& aLightingInfo) const override;

    protected:
        void RenderModel3DWireframe_GPass(const Model3D &aModel, const glm::mat4& aModelViewProjectionMatrix, const glm::vec4 &aColor) const override;
    public:

        void        RenderTransformGizmo(uint32_t aGizmoType, const Object *aModel, const Camera &aCamera) const override;
        void        RenderGrid2D(float aStep, const glm::vec4 &aColor, float aLineWidth, const Camera &aCamera) const override;
        void        RenderGrid3D(float aStep, float aSize, const glm::vec4& aColor, float aLineWidth, const Camera& aCamera) const override;
        void        RenderLight(const Light &aLight, const Camera &aCamera, uint32_t aLightNumber) const override;
        void        RenderLights(std::vector<const Light*> &aLights, const Camera &aCamera) const override;
        void        RenderBoundingBox(const BoundingBox &aBox, const glm::mat4 &aModelViewProjectionMatrix, const glm::vec4 &aColor) const override;
        void        RenderBoundingSphere(const BoundingSphere &aSphere, const glm::vec3 &aCenter, const glm::vec4 &aColor, const Camera &aCamera) const override;
        void        RenderBoundingVolumes(const Object3D &aObject, const Camera &aCamera, bool aShowSphere = true, bool aShowAABB = true, bool aShowOOBB = true) const override;
        void        RenderModelNormals(const Model3D &aModel, const glm::mat4 &aModelViewProjectionMatrix, float aNormalSize) const override;
        //bool        Resize(uint16_t aWidth, uint16_t aHeight);
        void        Flush();
        //void        Clear();

        inline GLuint GetTempVAO() const { return mTempVAO; }

      private:
        /**
         * Dimensions of the display
         */
        //uint16_t mWidth = 0, mHeight = 0;

        /**
         * Dummy texture for some GLSL 3.30 workaround
         */
        GLuint mDummyTexture = GL_NONE;

        /**
         * Texture used when an object does not receive shadow
         */
        GLuint mNoShadowTexture = GL_NONE;

        /**
         * Shader to render light billboards
         */
        OpenGLShader mRenderLightShader = OpenGLShader("render light");

        /**
         * Shader to render bounding volumes
         */
        OpenGLShader mRenderBoundingVolume = OpenGLShader("render bounding volume");
        enum class eBoundingVolume { BOX = 1, SPHERE = 2};

        /**
         * Shader to render model normals
         */
        OpenGLShader mRenderNormals = OpenGLShader("render normals");

        /**
         * Shader to render a solid color, used for wireframe rendering
         */
        OpenGLShader mWireframeShader = OpenGLShader("render solid color");

        /**
        * Shader to render shadowmaps
        */
        Procedural::TexturedQuad mShadowMapQuad;
        OpenGLShader mRenderShadowMapQuad = OpenGLShader("render shadowmap quad");

        GLuint mTempVAO = GL_NONE, mTempVBO = GL_NONE;
    };
}
