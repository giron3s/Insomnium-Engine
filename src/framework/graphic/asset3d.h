/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential

 *  Brief	    : An Asset3D object represents a 3D asset in the engine including the geometry,
 *                normals, textures and texture coordinates, and vertices indices for
 *                indexed geometry rendering. This class can be expanded to support any
 *                3D model definition that can be needed in the engine, like displacement
 *                maps. This class represents an static model and does not take into
 *                account animations or tessellation of skin models.
 *
 *                Objects of this class cannot be instantiated directly and can only
 *                be obtained through the model loaders of the Renderer class.
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
 *                The former data allows to render a model2D as a group of rendering lists, with each list
 *                defining a material and a texture, and a list of indices into the raw vertex data. This
 *                minimizes the bandwidth required from the GPU to render the model.
 *******************************************************************************/

#pragma once

#include <stdint.h>
#include <vector>
#include "graphic/material.h"
#include "graphic/texture.h"
#include "boundingbox.h"


namespace Framework
{
    class RendererResources
    {
    public:
        virtual ~RendererResources() = default;
    };

    class Asset3D;

    namespace Procedural
    {
        void AppendBentPlane(Asset3D &aAsset, float aWidth, float aHeight, float aAngleWidth, float aAngleHeight, float aAngleRadius,
                             uint32_t aNumVertsWidth, uint32_t aNumVertsHeight);
    }

    class Asset3D
    {
    public:

        /* not good, maybe it worth just defining these as member funcs here? */
        friend class Asset3DLoaders;
        friend class AssetTransform;
        friend void Procedural::AppendBentPlane(Asset3D &aAsset, float aWidth, float aHeight, float aAngleWidth, float aAngleHeight, float aAngleRadius,
                                                uint32_t aNumVertsWidth, uint32_t aNumVertsHeight);


        /**
         * Vertex data of the model
         */
#pragma pack(push)
#pragma pack(1)
        struct VertexData
        {
            glm::vec3 mVertex;  /**< Vertex in model coordinates */
            glm::vec3 mNormal;  /**< Normal of the vertex */
            glm::vec2 mUVCoord; /**< Textures coordinates of the vertex */
        };
#pragma pack(pop)


        Asset3D(const std::string& aName,
                const std::string& aResourceName)
            : mName(aName)
            , mResourceName(aResourceName)
            , mBoundingBox(glm::vec3(0), glm::vec3(0))
        {}

        virtual ~Asset3D() = default;

        /**
         * Normalizes all vertex data in the Asset3D
         *
         * This method calculates the center of mass of all vertices in the model
         * and then normalizes all vertices with respect to this CoM so the maximum length
         * from a vertex position to the CoM is 1.0f. This is typically used after loading
         * a model from disk which hasn't been imported before in the engine so the user can
         * make sure the model will be visible on screen
         */
        void Normalize();

        /**
         * The toString() method returns the value of a asset3d object.
         *
         * @return A string with the value of a asset3d object.
         */
        std::string ToString() const;
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
        inline const std::vector<uint32_t>&    GetIndexData() const      { return mVertexIndices; }
        inline const std::vector<uint32_t>&    GetIndicesOffsets() const { return mIndicesOffsets; }        
        inline const std::vector<uint32_t>&    GetIndicesCount() const   { return mIndicesCount; }
        inline const std::vector<Texture>&     GetTextures() const       { return mTextures; }
        inline const std::vector<Material>&    GetMaterials() const      { return mMaterials; }

        inline const BoundingBox&              GetBoundingBox() const { return mBoundingBox; }
        inline const glm::vec3&                GetMaxLengthVertex() const { return mMaxLengthVertex; }
        

        /**
        * Saves a Asset3D to disk with the given name
        *
        * @param name   Name of the model
        * @param model  Asset3D to be saved to disk
        *
        * @return true if the model was saved correctly or false
        *         otherwise
        */
        bool Save(const std::string &aName);

        /**
        * Loads a Asset3D from disk with the given name
        *
        * @param name   Name of the model
        * @param model  Asset3D to be loaded to disk
        *
        * @return true if the model was loaded correctly or false
        *         otherwise
        */
        bool Load(const std::string &aName);

        std::unique_ptr<RendererResources> mRendererResources;
        
        /**
        * Calculate AABB, MaxLengthVertex
        * Clean up mResourceName, mVertexData, mVertexIndices, mTextures
        */
        void RenderReady();

    protected:
        std::string              mName;
        std::string              mResourceName;
        std::vector<VertexData>  mVertexData;     /**< Data containing the vertex position, normal and UV coordinates */
        std::vector<uint32_t>    mVertexIndices;  /**< List of indices containing all rendering lists together */
        std::vector<uint32_t>    mIndicesOffsets; /**< Offset in vertexIndices of the beginning of the rendering list number 'n' */
        std::vector<uint32_t>    mIndicesCount;   /**< Number of indices belonging to the rendering list number 'n' */
        std::vector<Texture>     mTextures;       /**< List of textures used in the model */
        std::vector<Material>    mMaterials;      /**< List of materials used in the model */

        BoundingBox              mBoundingBox;
        glm::vec3                mMaxLengthVertex;
    };

}
