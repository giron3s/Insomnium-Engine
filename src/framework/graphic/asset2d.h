/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential

 *  Brief	    : An Asset2D object represents a 2D asset in the engine including the geometry,
                  textures and texture coordinates, and vertices indices for
 *                indexed geometry rendering. This class can be expanded to support any
 *                2D model definition that can be needed in the engine, like displacement
 *                maps. This class represents an static model and does not take into
 *                account animations or tessellation of skin models.
 *
 *                Objects of this class cannot be instantiated directly and can only
 *                be obtained through the model loaders of the Renderer class.
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
 *                The former data allows to render a model2D as a group of rendering lists, with each list
 *                defining a material and a texture, and a list of indices into the raw vertex data. This
 *                minimizes the bandwidth required from the GPU to render the model.

 *******************************************************************************/

#pragma once

#include <stdint.h>
#include <vector>
#include "graphic/material.h"
#include "graphic/texture.h"
#include "graphic/asset3d.h"
#include "boundingbox.h"


namespace Framework
{
    class Asset2D
    {
    public:

        /* not good, maybe it worth just defining these as member funcs here? */
        friend class AssetTransform;

        /**
         * Vertex data of the model
         */
#pragma pack(push)
#pragma pack(1)
        struct VertexData
        {
            glm::vec2 mVertex;  /**< Vertex in model coordinates */
            glm::vec2 mUVCoord; /**< Textures coordinates of the vertex */
        };
#pragma pack(pop)


        Asset2D(const std::string& aName,
                const std::string& aResourceName)
            : mName(aName)
            , mResourceName(aResourceName)
        {}

        virtual ~Asset2D() = default;

        Asset2D& operator =(Asset2D&& aTempAsset)
        {
            mName = std::move(aTempAsset.mName);
            mResourceName = std::move(aTempAsset.mResourceName);
            mVertexData = std::move(aTempAsset.mVertexData);
            mTextures = std::move(aTempAsset.mTextures);
            mMaxLengthVertex = std::move(aTempAsset.mMaxLengthVertex);
            return *this;
        }

        /**
         * Getter
         *
         * All getter return a reference to one of the internal vectors containing
         * the model definition for a specific subset of the data. The data is layed
         * out consecutively on memory so the caller can obtain the pointer to the
         * first element and load the buffer into the GPU
         */
        inline const std::string&              GetName() const { return mName; }
        inline const std::string&              GetResourceName() const { return mResourceName; }

        inline const std::vector<VertexData>&  GetVertexData() const     { return mVertexData; }
        inline const std::vector<Texture>&     GetTextures() const   { return mTextures; }

        inline const glm::vec2&                GetMaxLengthVertex() const { return mMaxLengthVertex; }
                
        std::unique_ptr<RendererResources> mRendererResources;

        /**
        * Calculate AABB, MaxLengthVertex
        * Clean up mResourceName, mVertexData, mVertexIndices, mTextures
        */
        void RenderReady();

    protected:
        std::string              mName;
        std::string              mResourceName;
        std::vector<VertexData>  mVertexData;    /*< Data containing the vertex position, and UV coordinates */
        std::vector<Texture>     mTextures;      /*< List of textures used in the model. The first texture is a shape texture. */

        glm::vec2                mMaxLengthVertex;
    };

}
