/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
   Brief       : Represents a scene with all the elements needed to render the scene. This acts
*                as a container for models, lights and any other element that may be needed to
*                render the scene. Each added element will have an associated name that can be
*                used to access the element to modify it, or to remove it from the scene
*
*******************************************************************************/

#pragma once
#include <vector>
#include <map>
#include <list>
#include <string>

#include "glm/glm.hpp"
#include "graphic/camera.h"
#include "graphic/directlight.h"
#include "graphic/model.h"
#include "graphic/model2d.h"
#include "graphic/model3d.h"
#include "graphic/procedural/grid.h"
#include "graphic/pointlight.h"
#include "graphic/rendertarget.h"
#include "graphic/spotlight.h"
#include "json/json.h"

namespace Framework
{

    class Scene : public SerializableObject
    {
      public:
        /**
         * Constructor
         */
        Scene(const std::string& aName)
            : mName(aName)
            , mActiveCamera(nullptr)
            , mIsLoaded(false)
        {}

        inline const std::string& GetName() const { return mName; }

        /**
         * Methods to add different elements to the scene by name
         *
         * The elements are added to the scene and can be later retrieved
         * using the 'get' method.
         *
         * The name must be unique across all elements of the same type. The name
         * can be duplicated between elements of different types, although it is not
         * recommended
         *
         * When adding the first camera or render target, that element will be set as
         * the active camera or render target for the scene until it is changed using
         * setActiveCamera or setActiveRenderTarget
         *
         * @param elem  Element to be added to the scene
         *
         * @return true if the element was added or false if
         *              the name already exists
         */
        bool Add(Procedural::Grid* aGrid);
        bool Add(Model2D *aElem);
        bool Add(Model3D *aElem);
        bool Add(PointLight *aElem);
        bool Add(SpotLight *aElem);
        bool Add(DirectLight *aElem);
        bool Add(Camera *aElem);
        bool Add(RenderTarget *aElem);


        /**
         * Methods to retrieve an element from the scene by name
         *
         * @param name  Name of the element in the scene
         *
         * @return The requested element
         */
        Model2D*      GetModel2D(const std::string &aName) const;
        Model3D*      GetModel3D(const std::string &aName) const;
        DirectLight*  GetDirectLight() const { return mDirectLight; }
        PointLight*   GetPointLight(const std::string &aName) const;
        SpotLight*    GetSpotLight(const std::string &aName) const;
        Camera*       GetCamera(const std::string &aName) const;
        RenderTarget* GetRenderTarget(const std::string &aName) const;


        /**
         * Retrieves the internal maps of elements
         *
         * @return The map of elements including the names and the elements
         */
        const std::list<Model2D*>&        GetModels2D() const { return mModels2D; }
        const std::list<Model3D*>&        GetModels3D() const { return mModels3D; }
        const std::vector<PointLight*>&   GetPointLights() const { return mPointLights; }
        const std::vector<SpotLight*>&    GetSpotLights() const { return mSpotLights; }
        const std::vector<Camera*>&       GetCameras() const { return mCameras; }
        const std::vector<RenderTarget*>& GetRenderTargets() const { return mRenderTargets; }


        /**
         * Sets the active camera to be used for rendering
         *
         * TODO: Add viewport support to support multiple renderings.
         *       When viewports are supported a new function to associate
         *       a camera with a viewport will be needed
         *
         * @param name  Name of the camera to be set as active
         *
         * @return true if the camera was set as active or false if the
         *         name does not exist
         */
        bool SetActiveCamera(const std::string &name);

        /**
         * Retrieves the active camera for the scene
         *
         * @return The active camera
         * */
        Camera* GetActiveCamera() const { return mActiveCamera; }

        /**
         * Sets the active render target where the rendered image will be
         * written to
         *
         * TODO: Add viewport support to support multiple renderings.
         *       When viewports are supported a new function to associate
         *       a render target with a viewport will be needed
         *
         * @param name  Name of the render target to be set as active
         *
         * @return true if the render target was set as active or false if the
         *         name does not exist
         */
        //bool SetActiveRenderTarget(const std::string &name); // TODO: maybe get rig of it!?

        /**
         * Retrieves the active render target
         *
         * @return The active render target
         * */
        //RenderTarget* GetActiveRenderTarget() const { return mActiveRenderTarget; } // TODO: maybe get rig of it!?

        /**
         * Retrieves the grid of the scene
         *
         * @return The grid of the scene
         */
        Procedural::Grid* GetGrid() const { return mGrid; }
        
        /**
         * Unload scene (delete all models, reset cameras to default)
         **/
        void Unload();

        /**
         * Returns whether the resources for this scene are loaded or not
         **/
        bool IsLoaded() { return mIsLoaded; }

        /**
         * Delete the selected models from the scene
         */
        void DeleteSelectedModels();
        
        /**
         * Get the model from the (X,Y) screen coordinates
         */
        Model::ReservedId GetModelAtPoint(int aX, int aY, __out Model*& aModel) const;
        
        /**
         * Toogle the camera between OTRHOGRAPHICS <-> PERSPECTIVE view
         */
        void ToggleCamera();

        virtual void Serialize(Json::Value& aSerializer) const override;
        virtual void Deserialize(const Json::Value& aSerializer) override;

        mutable Model*            mFocusedModel = nullptr;
        mutable std::list<Model*> mSelectedModels;


        struct TransformInfo
        {
            enum class TransformType : uint32_t
            {
                None      = 0,
                Translate = 1,
                Rotate    = 2,
                Scale     = 3
            };
            TransformType mTransformType = TransformType::None;
            Model::ReservedId mAxis      = Model::ReservedId::ClearColor;
            Model* mTarget = nullptr;
        };
        mutable TransformInfo mModelUnderTransform;

        BoundingBox mBoundingBox = BoundingBox(glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX), glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX));

      private:
        std::string                          mName;

        bool                                 mIsLoaded;

        std::list<Model2D*>                  mModels2D;             /**< Contains all models in the scene 2D view*/
        std::list<Model3D*>                  mModels3D;             /**< Contains all models in the scene 3D view*/
        DirectLight*                         mDirectLight;          /**< Directional light in the scene */
        std::vector<PointLight*>             mPointLights;          /**< Contains all point lights in the scene */
        std::vector<SpotLight*>              mSpotLights;           /**< Contains all spot lights in the scene */
        std::vector<Camera*>                 mCameras;              /**< Contains all cameras in the scene */
        std::vector<RenderTarget*>           mRenderTargets;        /**< Contains all render targets lights */

        Procedural::Grid*                    mGrid = nullptr;

        Camera*                              mActiveCamera;         /**< The current active camera */
    };

}

