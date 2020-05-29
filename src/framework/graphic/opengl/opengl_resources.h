/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential

 * Brief        : Asset3D representation prepared for the specific renderer
 *                This is a composite model generated through Renderer::prepareAsset3D()
 *                that prepares the asset for the specific renderer. For example in
 *                OpenGL it would generate the VBO specific to the model 3D and cache
 *                that value so it can be used later on

********************************************************************************/
#pragma once

#include <stdint.h>
#include "graphic/asset2d.h"
#include "graphic/asset3d.h"
#include "graphic/opengl/opengl.h"


namespace Framework
{
    class OpenGLResources final : public RendererResources
    {
      public:

        OpenGLResources(const Asset2D& aAsset);
        OpenGLResources(const Asset3D& aAsset);
        ~OpenGLResources() override;

        OpenGLResources(const OpenGLResources&) = delete;
        OpenGLResources(OpenGLResources&&) = delete;
        OpenGLResources& operator=(const OpenGLResources&) = delete;
        OpenGLResources& operator=(OpenGLResources&&) = delete;

        inline uint32_t                      GetVertexArrayID() const { return mVAO; }
        inline uint32_t                      GetIndicesArrayID() const { return mIndicesBO; }
        inline const std::vector<uint32_t>&  GetTexturesIDs() const { return mTexturesIDs; }

      private:
        GLuint                mVAO = GL_NONE;           /**< Vertex array object ID */
        GLuint                mVertexDataBO = GL_NONE;  /**< Vertex buffer object ID */
        GLuint                mIndicesBO = GL_NONE;     /**< Indices buffer object ID */
        std::vector<uint32_t> mTexturesIDs;             /**< Textures ID vector */
    };
}
