/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Render target for OpenGL. A render target allows to render objects to it
 *               instead of to the main screen. Then the target can be rendered to the main screen as a texture
 *
*******************************************************************************/

#include "precompiled.h"
#include "graphic/opengl/opengl_filterrendertarget.h"
#include "graphic/opengl/opengl_renderer.h"


namespace Framework
{

    void OpenGLFilterRenderTarget::Blit(uint32_t aDstX, uint32_t aDstY, uint32_t aWidth, uint32_t aHeight, uint32_t srcColorBufferIndex, bool dstIsMainFB) const
    {
        if ( srcColorBufferIndex >= mNumColorBuffers )
        {
            ASSERT(0);
            return; // Invalid parameter
        }
        
        if ( dstIsMainFB )
        {
            __(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE));
        }
        // else we assume some other framebuffer is bound for writing
        // with glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dstFramebuffer)
        // and a destination color buffer is selected with
        // glDrawBuffer(GL_COLOR_ATTACHMENT0 + dstColorBufferIndex)

        // Set the rendering mode
        //__(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)); // default
        //__(glDisable(GL_LINE_SMOOTH)); // ?
        //__(glEnable(GL_CULL_FACE)); // default

        // Set the blending mode
        switch ( mBlendingMode ) {
            case BLENDING_ADDITIVE:
            __(glEnable(GL_BLEND));
            __(glBlendEquation(GL_FUNC_ADD));
            __(glBlendFunc(GL_ONE, GL_ONE));
            break;
            default:
            __(glDisable(GL_BLEND));
        }

        __(glViewport(aDstX, aDstY, aWidth, aHeight));
        __(glActiveTexture(GL_TEXTURE0));
        __(glBindTexture(GL_TEXTURE_2D, GetTextureId(GL_COLOR_ATTACHMENT0 + srcColorBufferIndex)));

        ASSERT(mShader); // must be loaded
        // Tell the shader which texture unit to use
        mShader->Attach();
        mShader->SetUniformTexture2D("u_image", 0);
        mShader->SetUniformVec4("u_screenRect", glm::vec4(aDstX, aDstY, aWidth, aHeight));
        //SetCustomParams();

        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);

        const OpenGLRenderer* lGLRenderer = static_cast<const OpenGLRenderer*>(Renderer::GetInstance());
        __(glBindVertexArray(lGLRenderer->GetTempVAO()));
        __(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4)); // a dummy call
        __(glBindVertexArray(GL_NONE));

        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);

        //UnsetCustomParams();
        mShader->Detach();
    }
}