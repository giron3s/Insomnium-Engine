/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential

 * Brief	    : A Model3D represents a 3D asset in the engine including the geometry,
 *                normals, textures and texture coordinates, and vertices indices for
 *                indexed geometry rendering. This class can be expanded to support any
 *                3D model definition that can be needed in the engine, like displacement
 *                maps. This class represents an static model and does not take into
 *                account animations or tessellation of skin models.
 *
 *                Objects of this class cannot be instantiated directly and cannot only
 *                be obtained through model loaders.
 *
 *                The data of the 3D model is organized as follows:
 *
 *                    * List of vertices of the model including position, normal and UV coordinate.
 *                      If a vertex belongs to 2 different polygons with different textures or surface
 *                      directions, the same vertex position can appear more than once with different
 *                      normal and/or different UV coordinate
 *                    * List of indices for triangle rendering lists. The list of indices is always
 *                      a multiple of 3 and it specifies how to group 3 vertices to form a triangle,
 *                      including the face direction (counter clock-wise)
 *                    * The following vector define the rendering lists. Each entry of the folowing vectors
 *                      define 1 display list of triangles with it's corresponding material and texture. All
 *                      the following vectors must have the same length:
 *
 *                        * List of textures
 *                        * List of materials
 *                        * List of offsets into the indices list for the start of the rendering list
 *                        * List with the number of indices that belong to the rendering list starting
 *                          at the indicated offset
 *
 *                The former data allows to render a model3D as a group of rendering lists, with each list
 *                defining a material and a texture, and a list of indices into the raw vertex data. This
 *                minimizes the bandwidth required from the GPU to render the model.
 *
 *                TODO: This class is likely to be refactored when animations are introduced
 *                in the engine
 *******************************************************************************/

#pragma once

#include <stdint.h>
#include <vector>

#include "glm/glm.hpp"
#include "graphic/model.h"
#include "graphic/asset3d.h"
#include "graphic/lightingshader.h"
#include "graphic/material.h"
#include "graphic/object3d.h"
#include "graphic/texture.h"

namespace Framework
{
    class Model3D: public Model, public Object3D
    {
      public:
        /**
         * Friend classes
         */
        friend class ModelLoaders;
        friend class ModelTransform;

        /**
         * Constructors
         */
       Model3D(const char* aAssetName = nullptr,
               const char* aResourceName = nullptr,
               size_t aEntityID = Entity::GetDefaultID())
            : Model()
            , Object3D(aEntityID)
            , mRenderNormals(false)
            , mIsShadowCaster(true)
            , mIsShadowReceiver(true)
        {
            if( aAssetName && *aAssetName )
                mAsset = std::make_shared<Asset3D>( aAssetName, aResourceName );
        }

        Model3D(std::shared_ptr<const Asset3D> aAsset,
                size_t aEntityID = Entity::GetDefaultID())
            : Model()
            , Object3D(aEntityID)
            , mAsset(std::move(aAsset))
            , mRenderNormals(false)
            , mIsShadowCaster(true)
            , mIsShadowReceiver(true)
        {
        }

        /**
         * Destructor
         */
        ~Model3D() override = default;

        Model3D(const Model3D&) = delete;
        Model3D(Model3D&&) = delete;
        Model3D& operator =(const Model3D&) = delete;
        Model3D& operator =(Model3D&&) = delete;

        /* Adjust model position so that model's AABB is inside parent AABB */
        bool ConstrainPosition(const BoundingBox& aParentAABB);

        /**
         * Retrieves the model asset 3D
         *
         * @return The associated Asset3D
         */
        //Asset3D* GetAsset3D() { return mAsset.get(); }
        const Asset3D* GetAsset3D() const { return mAsset.get(); }
        operator const Asset3D*() { return mAsset.get(); }
        operator const Asset3D&() { return *mAsset; }

        /**
         * Enables/disables this model as a shadow caster
         *
         * If disabled the model is not taken into account when generating the
         * shadow map for a specific light
         *
         * @param aFlag  true (casts a shadow) or false (does not cast a shadow)
         */
        void SetShadowCaster(bool aFlag) { mIsShadowCaster = aFlag; }
        /**
         * Indicates if this model casts a shadow or not
         *
         * @see setShadowCaster
         *
         * @return true (casts a shadow) or false (does not cast a shadow)
         */
        bool IsShadowCaster(void) const { return mIsShadowCaster; }
        /**
         * Enables/disables this model as a shadow receiver
         *
         * If disabled the model will not use the available shadow maps to render
         * a shadow onto itself. Useful for example when an object is a light-emitting
         * object
         *
         * @param aFlag  true (receives shadow) or false (does not receive shadow)
         */
        void SetShadowReceiver(bool aFlag) { mIsShadowReceiver = aFlag; }
        /**
         * Indicates if this model receives shadow or not
         *
         * @see setShadowReceiver
         *
         * @return true (receives shadow) or false (does not receive shadow)
         */
        bool IsShadowReceiver(void) const { return mIsShadowReceiver; }
        /**
         * Debug information
         */
        void SetRenderNormals(bool aFlag) { mRenderNormals = aFlag; }
        bool GetRenderNormals(void) const { return mRenderNormals; }

      protected:
        /**
         * Methods from Object3D class
         */
        void CalculateBoundingVolumes() const override;

        std::shared_ptr<const Asset3D>  mAsset;
        bool                      mRenderNormals;    /**< Enables normal rendering for this model */
        bool                      mIsShadowCaster;   /**< Indicates if this model is a shadow caster */
        bool                      mIsShadowReceiver; /**< Indicates if this model is a shadow receiver */
    };

}
