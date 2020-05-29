/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential

 * Brief	    : A Model2D represents a 2D asset in the engine including the geometry,
 *                textures and texture coordinates, and vertices indices for
 *                indexed geometry rendering. This class can be expanded to support any
 *                2D model definition that can be needed in the engine, like displacement
 *                maps. This class represents an static model and does not take into
 *                account animations or tessellation of skin models.
 *
 *                Objects of this class cannot be instantiated directly and cannot only
 *                be obtained through model loaders.
 *
 *                The data of the 2D model is organized as follows:
 *
 *                    * List of vertices of the model including position and UV coordinate.
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
 *                The former data allows to render a Model2D as a group of rendering lists, with each list
 *                defining a material and a texture, and a list of indices into the raw vertex data. This
 *                minimizes the bandwidth required from the GPU to render the model.
 *
 *******************************************************************************/

#pragma once

#include <stdint.h>
#include "glm/glm.hpp"
#include "graphic/model.h"
#include "graphic/asset2d.h"
#include "graphic/material.h"
#include "graphic/object2d.h"
#include "graphic/texture.h"

#include "engine/components/entity.h"

namespace Framework
{
    class Model2D : public Model, public Object2D
    {
      public:
        /**
         * Friend classes
         */
        friend class ModelLoaders;
        friend class ModelTransform;

        std::string mText;

        /**
         * Constructors
         */
       Model2D(const char* aAssetName = nullptr,
               const char* aResourceName = nullptr,
               size_t aEntityID = Entity::GetDefaultID())
            : Model()
            , Object2D(aEntityID)
        {
            if( aAssetName && *aAssetName )
                mAsset = std::make_shared<Asset2D>( aAssetName, aResourceName );

        }

        Model2D(std::shared_ptr<const Asset2D> aAsset,
                size_t aEntityID = Entity::GetDefaultID())
            : Model()
            , Object2D(aEntityID)
            , mAsset(std::move(aAsset))
        {

        }

        /**
         * Destructor
         */
        ~Model2D() override = default;

        Model2D(const Model2D&) = delete;
        Model2D(Model2D&&) = delete;
        Model2D& operator =(const Model2D&) = delete;
        Model2D& operator =(Model2D&&) = delete;

        /**
         * Retrieves the model asset 2D
         *
         * @return The associated Asset2D
         */
        //Asset2D* GetAsset2D() { return mAsset.get(); }
        const Asset2D* GetAsset2D() const { return mAsset.get(); }
        operator const Asset2D*() { return mAsset.get(); }
        operator const Asset2D&() { return *mAsset; }

      protected:
        std::shared_ptr<const Asset2D>  mAsset;
    };

}
