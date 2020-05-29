/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "graphic/asset3d.h"
#include "core/graphic/zcompression.h"

namespace Framework
{

    void Asset3D::Normalize()
    {
        vector<VertexData>::iterator it;
        glm::vec3 cm = glm::vec3(0.0f, 0.0f, 0.0f);

        for (const auto& lElement : mVertexData)
            cm += lElement.mVertex;
        cm /= mVertexData.size();

        /* Substract the center of mass to all vertices */
        float maxLength = numeric_limits<float>::min();

        for (auto& lElement : mVertexData) 
        {
            lElement.mVertex -= cm;
            /* Calculate maximum length */
            float length = glm::length(lElement.mVertex);
            if (length > maxLength)
                maxLength = length;
        }

        /* Finally divide by maxLength to make the model fit in a sphere
         * of radius 1.0 */
        for (auto& lElement : mVertexData)
            lElement.mVertex /= maxLength;
    }


    string Asset3D::ToString() const
    {

        uint32_t i = 0, lStrSize = 0;
        string lStr;

        lStr.resize(1000000000);
        lStrSize += sprintf((char*)lStr.c_str() + lStrSize,"[Asset3D]\n");
        lStrSize += sprintf((char*)lStr.c_str() + lStrSize,"[Vertices] %zu\n", mVertexData.size());
        for (vector<Asset3D::VertexData>::const_iterator it = mVertexData.begin(); it != mVertexData.end(); ++it, ++i)
        {

            lStrSize += sprintf((char*)lStr.c_str() + lStrSize, "    [%05d] vertex: [%f, %f, %f]\n", i, it->mVertex.x, it->mVertex.y, it->mVertex.z);
            lStrSize += sprintf((char*)lStr.c_str() + lStrSize, "            normal: [%f, %f, %f]\n", it->mNormal.x, it->mNormal.y, it->mNormal.z);
            lStrSize += sprintf((char*)lStr.c_str() + lStrSize, "            uvcoord:[%f, %f]\n", it->mUVCoord.x, it->mUVCoord.y);
        }

        lStrSize += sprintf((char*)lStr.c_str() + lStrSize,"  [Indices] %zu\n", mVertexIndices.size());
        i = 0;
        for (vector<uint32_t>::const_iterator it = mVertexIndices.begin(); it != mVertexIndices.end(); ++it, ++i)
        {
            lStrSize += sprintf((char*)lStr.c_str() + lStrSize,"    [%05d] index: %d\n", i, *it);
        }

        lStrSize += sprintf((char*)lStr.c_str() + lStrSize,"  [Materials] %zu\n", mMaterials.size());
        for (i = 0; i < mMaterials.size(); ++i)
        {
            lStrSize += sprintf((char*)lStr.c_str() + lStrSize,"    [%05d]\t material:  Ka [%f, %f, %f]\n", i, mMaterials[i].GetAmbient().r,  mMaterials[i].GetAmbient().g,  mMaterials[i].GetAmbient().b);
            lStrSize += sprintf((char*)lStr.c_str() + lStrSize,"           \t            Kd [%f, %f, %f]\n",    mMaterials[i].GetDiffuse().r,  mMaterials[i].GetDiffuse().g,  mMaterials[i].GetDiffuse().b);
            lStrSize += sprintf((char*)lStr.c_str() + lStrSize,"           \t            Ks [%f, %f, %f]\n",    mMaterials[i].GetSpecular().r, mMaterials[i].GetSpecular().g, mMaterials[i].GetSpecular().b);
            lStrSize += sprintf((char*)lStr.c_str() + lStrSize,"           \t            alpha [%f]\n", mMaterials[i].GetAlpha());
            lStrSize += sprintf((char*)lStr.c_str() + lStrSize,"           \t            shine [%f]\n", mMaterials[i].GetShininess());
            lStrSize += sprintf((char*)lStr.c_str() + lStrSize,"           \t offset: %d\n", mIndicesOffsets[i]);
            lStrSize += sprintf((char*)lStr.c_str() + lStrSize,"           \t count:  %d\n", mIndicesCount[i]);
        }
        return lStr;
    }

    bool Asset3D::Save(const string &aName)
    {
        ofstream lFile(aName, ios::binary | ios::out | ios::trunc);
        ZCompression lComp;

        if (lFile.is_open() == false)
        {
            CRASH("ERROR opening file %s\n", aName.c_str());
            return false;
        }

        if (lComp.Init() == false)
        {
            CRASH("ERROR initializing compression in Asset3DStorage::Save\n");
            lFile.close();
            return false;
        }

        /* Write vertex data size */
        uint32_t lDataSize = (uint32_t)mVertexData.size();
        lComp.Write(lFile, (const char *)&lDataSize, sizeof lDataSize);

        /* Write vertex data */
        lComp.Write(lFile, (const char *)&mVertexData[0], lDataSize * sizeof mVertexData[0]);

        /* Write material data size */
        lDataSize = (uint32_t)mMaterials.size();
        lComp.Write(lFile, (const char *)&lDataSize, sizeof lDataSize);
       
        /* Write materials */
        lComp.Write(lFile, (const char *)&mMaterials[0], lDataSize * sizeof mMaterials[0]);

        /* Write texture data size */
        lDataSize = (uint32_t)mTextures.size();
        lComp.Write(lFile, (const char *)&lDataSize, sizeof lDataSize);

        /* Write texture data */
        for (const auto& lTexture : mTextures)
        {
            lComp.Write(lFile, &lTexture.mWidth, sizeof lTexture.mWidth);
            lComp.Write(lFile, &lTexture.mHeight, sizeof lTexture.mHeight);
            lComp.Write(lFile, &lTexture.mBytesPerPixel, sizeof lTexture.mBytesPerPixel);
            lComp.Write(lFile, lTexture.mPixels.data(), lTexture.mWidth * lTexture.mHeight * lTexture.mBytesPerPixel);
        }

        /* Write indices data size */
        lDataSize = (uint32_t)mVertexIndices.size();
        lComp.Write(lFile, (const char *)&lDataSize, sizeof lDataSize);

        /* Write indices data */
        lComp.Write(lFile, (const char *)&mVertexIndices[0], lDataSize * sizeof mVertexIndices[0]);

        /* Write indices offsets data size */
        lDataSize = (uint32_t)mIndicesOffsets.size();
        lComp.Write(lFile, (const char *)&lDataSize, sizeof lDataSize);

        /* Write indices offsets data */
        lComp.Write(lFile, (const char *)&mIndicesOffsets[0], lDataSize * sizeof mIndicesOffsets[0]);

        /* Write indices count data size */
        lDataSize = (uint32_t)mIndicesCount.size();
        lComp.Write(lFile, (const char *)&lDataSize, sizeof lDataSize);

        /* Write indices count data */
        lComp.Write(lFile, (const char *)&mIndicesCount[0], lDataSize * sizeof mIndicesCount[0], true);

        lComp.Finish();

        if (lFile.bad() == true)
        {
            CRASH("ERROR writing data to lFile %s\n", aName.c_str());
            lFile.close();
            return false;
        }

        /* Close the lFile */
        lFile.close();

        return true;
    }

    bool Asset3D::Load(const string &aName)
    {
        uint32_t lDataSize;
        ifstream lFile(aName, ios::binary | ios::in);
        ZDecompression lDeComp;

        if (lFile.is_open() == false)
        {
            CRASH("ERROR opening file %s\n", aName.c_str());
            return false;
        }

        if (lDeComp.Init() == false)
        {
            CRASH("ERROR initializing Decompression in Asset3DStorage::Load\n");
            lFile.close();
            return false;
        }

        /* Read vertex data size */
        lDeComp.Read(lFile, (char *)&lDataSize, sizeof lDataSize);
        mVertexData.resize(lDataSize);

        /* Read vertex data */
        lDeComp.Read(lFile, (char *)&mVertexData[0], lDataSize * sizeof mVertexData[0]);
        
        /* Read materials data size */
        lDeComp.Read(lFile, (char *)&lDataSize, sizeof lDataSize);
        mMaterials.resize(lDataSize);

        /* Read materials data */
        lDeComp.Read(lFile, (char *)&mMaterials[0], lDataSize * sizeof mMaterials[0]);

        /* Read textures data size */
        lDeComp.Read(lFile, (char *)&lDataSize, sizeof lDataSize);
        mTextures.resize(lDataSize);

        /* Now read the texture data */
        bool lPrintTexSizeWarn = false;
        for ( auto& lTexture : mTextures ) 
        {
            lDeComp.Read(lFile, &lTexture.mWidth, sizeof lTexture.mWidth);
            lDeComp.Read(lFile, &lTexture.mHeight, sizeof lTexture.mHeight);
            lDeComp.Read(lFile, &lTexture.mBytesPerPixel, sizeof lTexture.mBytesPerPixel);
            uint32_t lSize = lTexture.mWidth * lTexture.mHeight * lTexture.mBytesPerPixel;
            if ( lSize )
            {
                lTexture.mPixels.resize(lSize, 0);
                lDeComp.Read(lFile, lTexture.mPixels.data(), lSize);
            }
            if ( lTexture.mWidth > 1024 || lTexture.mHeight > 1024 )
                lPrintTexSizeWarn = true;
        }
        if ( lPrintTexSizeWarn )
            INFO(LogLevel::eLEVEL2, "Warning: A texture for model '%s' has size(s) > 1024", aName.c_str());

        /* Read indices data size */
        lDeComp.Read(lFile, (char *)&lDataSize, sizeof lDataSize);
        mVertexIndices.resize(lDataSize);

        /* Read the indices data */
        lDeComp.Read(lFile, (char *)&mVertexIndices[0], lDataSize * sizeof mVertexIndices[0]);

        /* Read the indices offsets data size */
        lDeComp.Read(lFile, (char *)&lDataSize, sizeof lDataSize);
        mIndicesOffsets.resize(lDataSize);

        /* Now read the indices offsets data */
        lDeComp.Read(lFile, (char *)&mIndicesOffsets[0], lDataSize * sizeof mIndicesOffsets[0]);

        /* Read the indices count data size */
        lDeComp.Read(lFile, (char *)&lDataSize, sizeof lDataSize);
        mIndicesCount.resize(lDataSize);

        /* Now read the indices count data */
        lDeComp.Read(lFile, (char *)&mIndicesCount[0], lDataSize * sizeof mIndicesCount[0]);

        if (lFile.bad() == true)
        {
            CRASH("ERROR reading data from file %s\n", aName.c_str());
            lFile.close();
            return false;
        }

        /* Close the lFile */
        lFile.close();
		lDeComp.Finish();

        return true;
    }


    void Asset3D::RenderReady()
    {
        if( mVertexData.empty() )
            return;
        glm::vec3& lMin = const_cast<glm::vec3&>(mBoundingBox.GetMin());
        glm::vec3& lMax = const_cast<glm::vec3&>(mBoundingBox.GetMax());
        lMin = lMax = mMaxLengthVertex = glm::vec3(0,0,0);
        float maxLength = 0.0f;
        for ( const auto& lElement : mVertexData )
        {
            // Calculate maximum lLength
            float lLength = glm::length(lElement.mVertex);
            if ( lLength > maxLength ) {
                maxLength = lLength;
                mMaxLengthVertex = lElement.mVertex;
            }
            // Calculate the maximum and minimum for each axis
            if ( lElement.mVertex.x < lMin.x )
                lMin.x = lElement.mVertex.x;
            else if ( lElement.mVertex.x > lMax.x )
                lMax.x = lElement.mVertex.x;
            if ( lElement.mVertex.y < lMin.y )
                lMin.y = lElement.mVertex.y;
            else if ( lElement.mVertex.y > lMax.y )
                lMax.y = lElement.mVertex.y;
            if ( lElement.mVertex.z < lMin.z )
                lMin.z = lElement.mVertex.z;
            else if ( lElement.mVertex.z > lMax.z )
                lMax.z = lElement.mVertex.z;
        }
        mResourceName.clear();
        mVertexData.clear();
        mVertexIndices.clear();
        mTextures.clear();
    }

}
