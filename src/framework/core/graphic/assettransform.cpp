 /*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential

 *  Brief       : Transforms the vertex data of a Asset3D
 *******************************************************************************/
#include "precompiled.h"
#include "core/graphic/assettransform.h"
#include <glm/gtx/quaternion.hpp>

namespace Framework
{
    void AssetTransform::Rotate(Asset3D &aAsset, const glm::vec3 aEulerAngles)
    {
        glm::mat4 rotation = glm::toMat4(glm::quat(aEulerAngles));

        for (vector<Asset3D::VertexData>::iterator it = aAsset.mVertexData.begin(); it != aAsset.mVertexData.end(); ++it)
        {
            it->mVertex = glm::vec3(rotation * glm::vec4(it->mVertex, 1.0f));
            it->mNormal = glm::mat3(rotation) * it->mNormal;
        }
    }

    void AssetTransform::Translate(Asset2D &aAsset, const glm::vec2 aOffsets)
    {
        for (vector<Asset2D::VertexData>::iterator it = aAsset.mVertexData.begin(); it != aAsset.mVertexData.end(); ++it)
        {
            it->mVertex += aOffsets;
        }
    }

    void AssetTransform::Translate(Asset3D &aAsset, const glm::vec3 aOffsets)
    {
        for (vector<Asset3D::VertexData>::iterator it = aAsset.mVertexData.begin(); it != aAsset.mVertexData.end(); ++it) 
        {
            it->mVertex += aOffsets;
        }
    }

    void AssetTransform::AppendGeometryOnly(Asset3D &to, const Asset3D &from)
    {
        uint32_t lOrigDataSize = static_cast<uint32_t>(to.mVertexData.size());

        /* The vertex data, materials, textures and indices count can be appended directly,
        * as they are independant of the mVertexData size */
        to.mVertexData.insert(to.mVertexData.end(), from.GetVertexData().begin(), from.GetVertexData().end());

        /* Copy the vertices indices. We need to add to the indices the original size of the
        * mVertexData array in the 'to' asset */
        for (vector<uint32_t>::const_iterator it = from.mVertexIndices.begin(); it != from.mVertexIndices.end(); ++it) {
            to.mVertexIndices.push_back(*it + lOrigDataSize);
        }
    }

    void AssetTransform::SetUniqueMaterial(Asset3D &aAsset, const Material &aMaterial, Texture&& aTexture)
    {
        /* Clear all previous data */
        aAsset.mMaterials.clear();
        aAsset.mTextures.clear();
        aAsset.mIndicesOffsets.clear();
        aAsset.mIndicesCount.clear();

        aAsset.mMaterials.push_back(aMaterial);
        aAsset.mTextures.push_back(std::move(aTexture));

        /* Only one set of indices */
        aAsset.mIndicesOffsets.push_back(0);
        aAsset.mIndicesCount.push_back( static_cast<uint32_t>(aAsset.mVertexIndices.size()) );
    }

    void AssetTransform::SetUniqueMaterialFromColor(Asset2D &aAsset, const glm::vec3 &aColor)
    {
        Texture lTexture;
        lTexture.mName.assign("UniformColor");
        lTexture.mWidth = lTexture.mHeight = 1;
        lTexture.mBytesPerPixel = 3;
        lTexture.mPixels.resize(3, 0);
        lTexture.mPixels[0] = (uint8_t)(aColor.r * 255);
        lTexture.mPixels[1] = (uint8_t)(aColor.g * 255);
        lTexture.mPixels[2] = (uint8_t)(aColor.b * 255);
        if ( aAsset.mTextures.empty() )
            aAsset.mTextures.resize(1);
        ASSERT(1 <= aAsset.mTextures.size() && aAsset.mTextures.size() <= 2);
        aAsset.mTextures[aAsset.mTextures.size() - 1] = std::move(lTexture);
    }

    void AssetTransform::SetUniqueMaterialFromColor(Asset3D &aAsset, const glm::vec3 &aColor)
    {
        Texture lTexture;
        lTexture.mWidth = lTexture.mHeight = 1;
        lTexture.mBytesPerPixel = 3;
        lTexture.mPixels.resize(3, 0);
        lTexture.mPixels[0] = (uint8_t)(aColor.r * 255);
        lTexture.mPixels[1] = (uint8_t)(aColor.g * 255);
        lTexture.mPixels[2] = (uint8_t)(aColor.b * 255);
        SetUniqueMaterial(aAsset, Material(), std::move(lTexture));
    }

    void AssetTransform::SetUniqueMaterialFromTexture(Asset2D &aAsset, Texture&& aTexture)
    {
        if ( aAsset.mTextures.empty() )
            aAsset.mTextures.resize(1);
        ASSERT(1 <= aAsset.mTextures.size() && aAsset.mTextures.size() <= 2);
        aAsset.mTextures[aAsset.mTextures.size() - 1] = std::move(aTexture);
    }

    void AssetTransform::SetUniqueMaterialFromTexture(Asset3D &aAsset, Texture&& aTexture)
    {
        SetUniqueMaterial(aAsset, Material(), std::move(aTexture));
    }

    void AssetTransform::RecalculateNormals(Asset3D &aAsset)
    {
        /* Loop the asset indices and create a map for each vertex
        * containing the normals of the faces it touches */
        map<uint32_t, vector<glm::vec3> > lNormalsMap;

        uint32_t *lIndex = &aAsset.mVertexIndices[0];

        for (uint32_t i = 0; i < aAsset.mVertexIndices.size(); i += 3)
        {
            /* Calculate the normal for the face */
            glm::vec3 a = aAsset.mVertexData[lIndex[i]].mVertex - aAsset.mVertexData[lIndex[i + 1]].mVertex;
            glm::vec3 b = aAsset.mVertexData[lIndex[i + 2]].mVertex - aAsset.mVertexData[lIndex[i + 1]].mVertex;
            glm::vec3 lNormal = glm::cross(b, a);

            lNormalsMap[lIndex[i]].push_back(lNormal);
            lNormalsMap[lIndex[i + 1]].push_back(lNormal);
            lNormalsMap[lIndex[i + 2]].push_back(lNormal);
        }

        /* Now loop the normals map and calculate the average */
        for (map<uint32_t, vector<glm::vec3> >::iterator it = lNormalsMap.begin(); it != lNormalsMap.end(); ++it)
        {
            glm::vec3 lNormal(0.0f);
            for (vector<glm::vec3>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            {
                lNormal += *it2;
            }

            /* Divide by the number of normals in the average and set it to
            * the right vertex */
            aAsset.mVertexData[it->first].mNormal = glm::normalize(lNormal);
        }
    }
}
