/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief Interface for software/hardware renderers
 *******************************************************************************/

#pragma once

#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "graphic/scene.h"
#include "graphic/camera.h"
#include "graphic/viewport.h"
#include "graphic/fontrender.h"


#define RENDER_MODEL__RENDER_FOCUSED_FLAG  1
#define RENDER_MODEL__RENDER_SELECTED_FLAG 2
#define RENDER_MODEL__NO_MATERIAL_FLAG     4

using namespace std::placeholders;

namespace Framework
{
    class Renderer
    {
    public:
        /**
         * Singleton
         */
        static Renderer* GetInstance(void);
        static void DisposeInstance(void);

        /**
         * Destructor
         */
        virtual ~Renderer() {}
        /**
         * Initializes the renderer
         */
        virtual bool Init(void) = 0;

        /**
         * Renderer introspection
         */
        virtual const char* GetName() const = 0;
        virtual const char* GetVersion() const = 0;
        virtual const char* GetVendor() const = 0;
        virtual const char* GetShaderVersion() const = 0;

        virtual Shader* NewShader(const std::string& aName) const = 0;
        virtual bool        PrepareForRendering(Asset2D& aAsset) const = 0;
        virtual bool        PrepareForRendering(Asset3D& aAsset) const = 0;


        /**
         * Renders a whole scene
         *
         * This method uses the input scene to render the final frame. It performs
         * some geometry occlusion calculations, and determines the best parameters
         * for light rendering using the camera frustum and the models bounding boxes
         *
         * It does NOT blit the active render target. This must be done by the caller.
         * This allows the caller to perform other operation on the rendered scene.
         * To blit the scene, use aScene.GetActiveRenderTarget()->Blit().
         *
         * @see Scene
         *
         * @param scene     The scene to be rendered
         */
        void RenderScene(const Scene& aScene) const;

    private:
        void RenderScene2D(const Scene& aScene) const;
        void RenderScene3D(const Scene& aScene) const;
        
        bool RenderSort2D(Model2D* aModel1, Model2D* aModel2)
        {
            return (aModel1->mBuddy->GetPosition().y > aModel2->mBuddy->GetPosition().y);
        }
         
        std::function<bool(Model2D*, Model2D*)> mRenderSort2D = std::bind(&Renderer::RenderSort2D, this, _1, _2);

    public:
        void SetRenderSort2D(const std::function<bool(Model2D*, Model2D*)> &aRenderSort) { mRenderSort2D = aRenderSort; }

    public:

        void RenderDebug(const Scene& aScene) const;

        /**
        * Renders a model 2D from the given camera using the provided lighting shader and the
        * different types of light into the given renderTarget
        *
        * @param model         Model to be rendered
        * @param view-projection matrix
        * @param shader        Lighting shader to apply to the model
        */
        virtual void RenderModel2D(const Model2D& aModel, const glm::mat4& aViewProjectionMatrix, const Shader* aShader,
            uint32_t aFlags) const = 0;


        /**
         * Renders the wireframe of a 2D/3D model from the given camera with the given color
         *
         * @param model         Model to be rendered
         * @param view-projection matrix  Camera settings
         * @param color         Color of the wireframe
         */
        virtual void RenderModelWireframe(const Object* aModel, const glm::mat4& aViewProjectionMatrix, const glm::vec4& aColor) const = 0;

    protected:
        // This functions simply passes Model's geometry to rendering pipeline
        virtual void RenderModel3D_core(const Model3D& aModel, uint32_t aFlags) const = 0;
        // Render depthmaps of shadow casters to corresponding texture
        virtual void RenderToShadowMap(const Light* aLight, const std::list<Model3D*>& aModels3D, const Shader* aShader) const;

    public:

        /** FORWARD SHADNG RENDERING ROUTINE
         * Renders a model 3D from the given camera using the provided lighting shader and the
         * different types of light into the given renderTarget
         *
         * @param model         Model to be rendered
         * @param camera        Camera to use for the rendering
         * @param shader        Lighting shader to apply to the model
         * @param sun           Direct light to apply to the model
         * @param pointLights   Vector of point lights to use for the rendering
         * @param spotLights    Vector of spot lights to use for the rendering
         * @param ambientK      Precalculated ambient factor to use for the rendering. This is
         *                      typically calculated from the scene definition
         */
        struct LightingInfo {
            const DirectLight* mDirectLight;
            const std::vector<const PointLight*>& mPointLights;
            const std::vector<const SpotLight*>& mSpotLights;
            float mAmbientK;
        };

        virtual void RenderModel3D(const Model3D& aModel, const Camera& aCamera, const Shader* aShader,
            uint32_t aFlags,
            const LightingInfo& aLightingInfo) const = 0;

        /* DEFERRED SHADNG RENDERING ROUTINE
         Note: a proper gbuffer render target and a geometry pass shader
         must have been bound already before this call */
        virtual void RenderModel3D_GPass(const Model3D& aModel, const glm::mat4& aModelViewProjectionMatrix, const Shader* aShader,
            uint32_t aFlags,
            const LightingInfo& aLightingInfo) const = 0;

    protected:
        virtual void RenderModel3DWireframe_GPass(const Model3D& aModel, const glm::mat4& aModelViewProjectionMatrix, const glm::vec4& aColor) const = 0;
        virtual void SetupShaderShadowParams(const Light* aLight, const Shader* aShader) const;
    public:

        /**
         * Renders a textured quad into the given renderTarget
         * with the given color
         *
         * @param model         Model to be rendered
         * @param imageOffset   Offset of the textured quad
         * @param imageScale    Scale  of the textured quad
         */
        virtual void RenderTexturedQuad(const Model2D& aModel, const glm::vec2& aImageOffset, const glm::vec2& aImageScale) const = 0;

        /**
         * Renders a quad textured with a shadow map, for debug purposes.
         *
         * @param imageOffset   Offset of the textured quad
         * @param imageScale    Scale  of the textured quad
         * @param cam           current/active camera for drawing
         * @param shadowMap     shadow map to be debugged
         * @param renderTarget  render target to draw into, or nullptr if directly to screen
         */
        virtual void RenderShadowMapQuad(const glm::vec2& aQuadOffset, const glm::vec2& aQuadScale, const Camera* aCam, RenderTarget* aShadowMap, RenderTarget* aRenderTarget) const = 0;


        /**
         * Renders a transform gizmo for the given 2D/3D model
         */
        virtual void RenderTransformGizmo(uint32_t aGizmoType, const Object* aModel, const Camera& aCamera) const = 0;

        /**
         * Renders a grid in XY plane (for z == 0)
         */
        virtual void RenderGrid2D(float aStep, const glm::vec4& aColor, float aLineWidth, const Camera& aCamera) const = 0;
        virtual void RenderGrid3D(float aStep, float aSize, const glm::vec4& aColor, float aLineWidth, const Camera& aCamera) const = 0;

        /**
         * Adjusts the renderer's display size
         */
         //virtual void Resize(uint16_t aWidth, uint16_t aHeight) = 0;


         /**
          * Flushes all the renderers pending commands
          */
        virtual void Flush() = 0;


        /**
         * Clears the main framebuffer
         */
         //virtual void Clear() = 0;


         /**---------------------------------------------------------------
          *
          *               DEBUG RENDERING METHODS
          *
          *---------------------------------------------------------------*/

          /**
           * Renders the position of the light as a billboard with the ambient component
           * of the light. Used for debugging purposes
           *
           * Mainly used from renderLights as a helper function
           *
           * @param light         Light to be rendered as a billboard
           * @param camera        Camera to use for the rendering
           * @param renderTarget  Render target for rendering the billboard
           * @param lightNumber   Number of light to offset the billboard rendering, this helps
           *                      that the billboards are not rendered at the same depth
           *
           * @return true or false
           */
        virtual void RenderLight(const Light& aLight, const Camera& aCamera, uint32_t aLightNumber) const = 0;


        /**
         * Renders the position of the light in the input vector as a billboard with the ambient component
         * of the light. Used for debugging purposes
         *
         * @param lights        Vector of lights to be rendered as a billboard
         * @param camera        Camera to use for the rendering
         * @param renderTarget  Render target for rendering the billboard
         *
         * @return true or false
         */
        virtual void RenderLights(std::vector<const Light*>& aLights, const Camera& aCamera) const = 0;


        /**
         * Renders the bounding box with the given color
         *
         * Helper method for renderModelBoundingBoxes
         *
         * @param box           Bounding box to be rendered
         * @param modelMatrix   Model matrix used to bring the bounding box to world coordinates. Typically
         *                      the matrix comes from the same Model3D than the BoundingBox
         * @param color         Color to use for the bounding box rendering
         * @param camera        Camera to use for the bounding box rendering
         * @param renderTarget  Render target for rendering the bounding box
         *
         * @return true or false
         */
        virtual void RenderBoundingBox(const BoundingBox& aBox, const glm::mat4& aModelViewProjectionMatrix, const glm::vec4& aColor) const = 0;


        /**
         *
         * Renders the bounding sphere with the given color
         *
         * Helper method for renderModelBoundingBoxes
         *
         * @param sphere        Bounding sphere to be rendered
         * @param center        World coordinates position for the center of the sphere. Typically the
         *                      position of the Model3D that contained the bounding sphere
         * @param color         Color to use for the bounding box
         * @param camera        Camera to use for the bounding box rendering
         * @param renderTarget  Render target for rendering the bounding box
         *
         * @return true or false
         */
        virtual void RenderBoundingSphere(const BoundingSphere& aSphere, const glm::vec3& aCenter, const glm::vec4& aColor, const Camera& aCamera) const = 0;


        /**
         * Renders an object 3D bounding volumes (AABB, OOBB and Bounding Sphere)
         *
         * @param object        Object containing the bounding boxes to be rendered
         * @param camera        Camera to use for the rendering
         * @param renderTarget  Render target for rendering the bounding volumes
         * @param showSphere    Indicates whether to render the bounding sphere or not
         * @param showAABB      Indicates whether to render the AABB or not
         * @param showOOBB      Indicates whether to render the OOBB or not
         *
         * @return true or false
         */
        virtual void RenderBoundingVolumes(const Object3D& aObject, const Camera& aCamera, bool aShowSphere = true,
            bool aShowAABB = true, bool aShowOOBB = true) const = 0;

        /**
         * Renders a model 3D normals
         *
         * @param model         Model containing the normals to be rendered
         * @param camera        Camera to use for the rendering
         * @param renderTarget  Render target for rendering the normals
         * @param normalSize    Length of the normal in world units
         *
         * @return true or false
         */
        virtual void RenderModelNormals(const Model3D& aModel, const glm::mat4& aModelViewProjectionMatrix, float aNormalSize) const = 0;

        /**
         * Global settings for the renderer
         */
        enum WireframeMode {
            RENDER_WIREFRAME_OFF,     /**< Disables wireframe rendering */
            RENDER_WIREFRAME_OVERLAY, /**< Enables wireframe overlay rendering, first normal model is rendered, then wireframe */
            RENDER_WIREFRAME_ONLY     /**< Enable full wireframe rendering, normal model is NOT renderered */
        };

        void SetWireframeMode(WireframeMode mode) { mWireframeMode = mode; }
        WireframeMode GetWireframeMode() const { return mWireframeMode; }
        void SetRenderNormals(bool aFlag) { mRenderNormals = aFlag; }
        bool GetRenderNormals() const { return mRenderNormals; }
        void SetRenderBoundingVolumes(bool aFlag)
        {
            mRenderBoundingSphere = aFlag;
            mRenderAABB = aFlag;
            mRenderOOBB = aFlag;
        }
        void SetRenderBoundingSphere(bool aFlag) { mRenderBoundingSphere = aFlag; }
        void SetRenderAABB(bool aFlag) { mRenderAABB = aFlag; }
        void SetRenderOOBB(bool aFlag) { mRenderOOBB = aFlag; }
        bool GetRenderBoundingSphere() const { return mRenderBoundingSphere; }
        bool GetRenderAABB() const { return mRenderAABB; }
        bool GetRenderOOBB() const { return mRenderOOBB; }
        void SetRenderLightsMarkers(bool aFlag) { mRenderLightsMarkers = aFlag; }
        bool GetRenderLightsMarkers() const { return mRenderLightsMarkers; }
        void SetRenderDebugInfo (bool aFlag) { mRenderDebugInfo = aFlag; }
        bool GetRenderDebugInfo () const { return mRenderDebugInfo; }

        int GetRenderTargetIdx() { return mRenderTargetIdx; }
        void SetRenderTargetIdx(int aIdx) { mRenderTargetIdx = aIdx; }

        int GetRenderAttachmentIdx() { return mRenderAttachmentIdx; }
        void SetRenderAttachmentIdx(int aIdx) { mRenderAttachmentIdx = aIdx; }

        Light* GetRenderLightShadowMap() { return mRenderLightShadowMap; }
        void SetRenderLightShadowMap(Light* aLight) { mRenderLightShadowMap = aLight; } //  Can be set to nullptr so no check
    
        int GetRenderShadowMapIdx() { return mRenderShadowMapIdx; }
        void SetRenderShadowMapIdx(int aIdx) { mRenderShadowMapIdx = aIdx; }

     protected:
        /**
         * Constructor
         */
        Renderer()
            : mWireframeMode(RENDER_WIREFRAME_OFF)
            , mRenderNormals(false)
            , mRenderBoundingSphere(false)
            , mRenderAABB(false)
            , mRenderOOBB(false)
            , mRenderLightsMarkers(false)
            , mRenderDebugInfo (true)
            , mRenderTargetIdx(0)
            , mRenderAttachmentIdx(0)
            , mRenderLightShadowMap(nullptr)
        {

        }

    protected:
        static Renderer* sRenderer;             /**< Singleton instance */
        static FontRenderer* sFontRenderer;         /**< Singleton instance */
        WireframeMode          mWireframeMode;        /**< Sets the wireframe mode rendering. @see WireframeMode */
        bool                   mRenderNormals;        /**< Global aFlag to enable model normals rendering */
        bool                   mRenderBoundingSphere; /**< Global aFlag to enable model bounding sphere rendering */
        bool                   mRenderAABB;           /**< Global aFlag to enable model AABB rendering */
        bool                   mRenderOOBB;           /**< Global aFlag to enable model OOBB rendering */
        bool                   mRenderLightsMarkers;  /**< Global aFlag to enable lights markers rendering */
        bool                   mRenderDebugInfo;

        int                    mRenderTargetIdx;       ///  Current render target being displayed
        int                    mRenderAttachmentIdx;   ///  Current color attachment being displayed
        Light*                 mRenderLightShadowMap;  ///  Current light whose shadow map is being displayed
        int                    mRenderShadowMapIdx;    ///  Current index of the light whose shadow map is being displayed
    };

    constexpr uint32_t _2D_FLAG = 16;
}
