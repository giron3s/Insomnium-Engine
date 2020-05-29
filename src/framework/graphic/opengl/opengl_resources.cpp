/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 * Brief        : OpengGL asset 2D/3D representation
 *
 ********************************************************************************/

#include "precompiled.h"
#include "graphic/asset2d.h"
#include "graphic/asset3d.h"
#include "graphic/opengl/opengl_resources.h"

namespace Framework
{

    OpenGLResources::OpenGLResources(const Asset2D& aAsset)
    {
        ASSERT(!aAsset.GetVertexData().empty() );
        if ( aAsset.GetVertexData().empty() )
            return;

        /* Generate a vertex array to reference the attributes */
        __(glGenVertexArrays(1, &mVAO));
        __(glBindVertexArray(mVAO));
        {
            /* Generate a buffer model for the vertices positions */
            __(glGenBuffers(1, &mVertexDataBO));
            __(glBindBuffer(GL_ARRAY_BUFFER, mVertexDataBO));
            {
                /* Upload the data for this buffer */
                __(glBufferData(GL_ARRAY_BUFFER, aAsset.GetVertexData().size() * sizeof aAsset.GetVertexData()[0], &(aAsset.GetVertexData()[0]), GL_STATIC_DRAW));

                /* First attribute contains the vertex coordinates */
                __(glEnableVertexAttribArray(0));
                __(glVertexAttribPointer(0,       // attribute index. No particular reason for 0, but must match the layout in the shader.
                    2,                            // size
                    GL_FLOAT,                     // type
                    GL_FALSE,                     // normalized?
                    sizeof(Asset2D::VertexData),  // stride
                    (void *)0                     // array buffer lOffset
                    ));

                /* Second attibute contains the UV coordinates */
                uint32_t lOffset = 8;
                __(glEnableVertexAttribArray(1));
                __(glVertexAttribPointer(1,           // attribute
                    2,                                // size
                    GL_FLOAT,                         // type
                    GL_FALSE,                         // normalized?
                    sizeof(Asset2D::VertexData),      // stride
                    reinterpret_cast<void *>(lOffset) // array buffer lOffset
                    ));
            }
            mIndicesBO = GL_NONE;
        }
        __(glBindVertexArray(GL_NONE));

        /* Upload Textures */
        mTexturesIDs.resize(aAsset.GetTextures().size());
        glGenTextures((GLsizei)aAsset.GetTextures().size(), mTexturesIDs.data());
        __(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
        for ( size_t i = 0; i < mTexturesIDs.size(); ++i )
        {
            const auto& lTexture = aAsset.GetTextures()[i];
            ASSERT(lTexture.mWidth && lTexture.mHeight && lTexture.mBytesPerPixel);
            __(glBindTexture(GL_TEXTURE_2D, mTexturesIDs[i]));
            {
                // the number of mipmap levels = log2( max(width, height) ) + 1
                uint32_t mipMapLevels = log2(max(lTexture.mWidth, lTexture.mHeight)) + 1;
                __(glTexStorage2D(GL_TEXTURE_2D, mipMapLevels, GL_RGBA8, lTexture.mWidth, lTexture.mHeight));
                __(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lTexture.mWidth, lTexture.mHeight, lTexture.mFormat, lTexture.mType, lTexture.mPixels.data()));
                __(glGenerateMipmap(GL_TEXTURE_2D));
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
            }
            __(glBindTexture(GL_TEXTURE_2D, GL_NONE));
        }
    }

    OpenGLResources::OpenGLResources(const Asset3D& aAsset)
    {
        ASSERT(!aAsset.GetVertexData().empty() && !aAsset.GetIndexData().empty());
        if ( aAsset.GetVertexData().empty() || aAsset.GetIndexData().empty() )
            return;

        /* Generate a vertex array to reference the attributes */
        __(glGenVertexArrays(1, &mVAO));
        __(glBindVertexArray(mVAO));
        {
            /* Generate a buffer model for the vertices positions */
            __(glGenBuffers(1, &mVertexDataBO));
            __(glBindBuffer(GL_ARRAY_BUFFER, mVertexDataBO));
            {
                /* Upload the data for this buffer */
                __(glBufferData(GL_ARRAY_BUFFER, aAsset.GetVertexData().size() * sizeof aAsset.GetVertexData()[0], &(aAsset.GetVertexData()[0]), GL_STATIC_DRAW));

                /* First attribute contains the vertex coordinates */
                __(glEnableVertexAttribArray(0));
                __(glVertexAttribPointer(0, // attribute index. No particular reason for 0, but must match the layout in the shader.
                    3,         // size
                    GL_FLOAT,  // type
                    GL_FALSE,  // normalized?
                    sizeof(Asset3D::VertexData),  // stride
                    (void *)0                     // array buffer lOffset
                    ));

                /* Second attibute contains the normals */
                uint32_t lOffset = 12;
                __(glEnableVertexAttribArray(1));
                __(glVertexAttribPointer(1,           // attribute
                    3,                                // size
                    GL_FLOAT,                         // type
                    GL_FALSE,                         // normalized?
                    sizeof(Asset3D::VertexData),      // stride
                    reinterpret_cast<void *>(lOffset) // array buffer lOffset
                    ));

                /* Third attribute contains the UV coordinates */
                lOffset = 24;
                __(glEnableVertexAttribArray(2));
                __(glVertexAttribPointer(2,           // attribute
                    2,                                // size
                    GL_FLOAT,                         // type
                    GL_FALSE,                         // normalized?
                    sizeof(Asset3D::VertexData),      // stride
                    reinterpret_cast<void *>(lOffset) // array buffer lOffset
                    ));
            }

            /* Generate the buffer models for the indices */
            __(glGenBuffers(1, &mIndicesBO));
            __(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicesBO));
            {
                /* Upload the data */
                __(glBufferData(GL_ELEMENT_ARRAY_BUFFER, aAsset.GetIndexData().size() * sizeof(aAsset.GetIndexData()[0]), &(aAsset.GetIndexData()[0]), GL_STATIC_DRAW));
            }
        }
        __(glBindVertexArray(GL_NONE));

        /* Upload the lTextures */
        mTexturesIDs.resize(aAsset.GetTextures().size());
        glGenTextures((GLsizei)aAsset.GetTextures().size(), mTexturesIDs.data());
        __(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
        for ( size_t i = 0; i < mTexturesIDs.size(); ++i )
        {
            const auto& lTexture = aAsset.GetTextures()[i];
            /* Prepare the mipmap generation */
            if ( lTexture.mWidth == 0 || lTexture.mHeight == 0 )
            { // what kind of case it that?
                continue;
            }
            /* TODO: Once we use our own format, this should not be needed */
            __(glBindTexture(GL_TEXTURE_2D, mTexturesIDs[i]));
            {
                // the number of mipmap levels = log2( max(width, height) ) + 1
                uint32_t mipMapLevels = log2(max(lTexture.mWidth, lTexture.mHeight)) + 1;
                __(glTexStorage2D(GL_TEXTURE_2D, mipMapLevels, GL_RGBA8, lTexture.mWidth, lTexture.mHeight));
                __(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lTexture.mWidth, lTexture.mHeight, GL_RGB, GL_UNSIGNED_BYTE, lTexture.mPixels.data()));
                __(glGenerateMipmap(GL_TEXTURE_2D));
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
            }
        }
        __(glBindTexture(GL_TEXTURE_2D, GL_NONE));
    }


    OpenGLResources::~OpenGLResources()
    {
        if ( mVertexDataBO != GL_NONE )
            __(glDeleteBuffers(1, &mVertexDataBO));
        if ( mIndicesBO != GL_NONE )
            __(glDeleteBuffers(1, &mIndicesBO));
        if ( mVAO != GL_NONE )
            __(glDeleteVertexArrays(1, &mVAO));
        if ( !mTexturesIDs.empty() )
            __(glDeleteTextures((GLsizei)mTexturesIDs.size(), &mTexturesIDs[0]));
    }

}
