/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Render target for OpenGL. A render target allows to render objects to it
 *                instead of to the main screen. Then the target can be rendered to the main screen as a texture
 *
 *                The NOAA render target applies no anti-aliasing
 ********************************************************************************/

#pragma once

#include "graphic/msaarendertarget.h"
#include "graphic/shader.h"
#include "graphic/opengl/opengl.h"

namespace Framework
{
    class OpenGLMSAARenderTarget : public MSAARenderTarget
    {
      public:
        ~OpenGLMSAARenderTarget() override;
        bool Init(uint32_t aWidth, uint32_t aHeight, uint32_t aMaxSamples = 0, uint32_t aNumTargets = 1);
        void SetSize(uint32_t aWidth, uint32_t aHeight);
        void Bind();
        void BindDepth();
        void Unbind();
        bool Blit(uint32_t aDstX, uint32_t aDstY, uint32_t aWidth, uint32_t aHeight, uint32_t aTarget = 0, bool aBindMainFB = true);
        void Clear();

        static uint32_t GetMaxSamples();

      private:
        GLuint   mFrameBuffer;  /**< Frame buffer object containing the color and depth buffers */
        uint32_t mNumTargets;   /**< Number of color attachments for this target */
        GLuint * mColorBuffer;  /**< Array of GL allocated IDs for the color buffers */
        GLuint * mAttachments;  /**< Array of color attachments locations for the draw buffers */
        GLuint   mDepthBuffer;  /**< GL allocated ID for the depth buffer */
        GLuint   mVertexArray;  /**< VAO for the render target surface */
        GLuint   mVertexBuffer; /**< VBO for the render target surface */
        uint32_t mSamples;      /**< Number of samples for the multi-sample filter */
    };
}
