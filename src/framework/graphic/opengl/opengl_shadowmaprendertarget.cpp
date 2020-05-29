/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential

 * Brief        : Render target for OpenGL. A render target allows to render objects to it
 *                instead of to the main screen. Then the target can be rendered to the main screen as
 *                a texture
 *
 *                The ShadowMap render target applies no anti-aliasing
*******************************************************************************/

#include "precompiled.h"
#include "graphic/opengl/opengl_shadowmaprendertarget.h"
#include "graphic/opengl/opengl_renderer.h"

namespace Framework
{
    
    void OpenGLShadowMapRenderTarget::Blit(uint32_t aDstX, uint32_t aDstY, uint32_t aWidth, uint32_t aHeight, uint32_t srcColorBufferIndex, bool dstIsMainFB) const
    {
        if ( srcColorBufferIndex )
        {
            ASSERT(0);
            return; // Invalid parameter
        }

        if (dstIsMainFB)
        {
            __(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE));
        }
        // else we assume some other framebuffer is bound for writing
        // with glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dstFramebuffer)
        // and a destination color buffer is selected with
        // glDrawBuffer(GL_COLOR_ATTACHMENT0 + dstColorBufferIndex)

        __(glViewport(aDstX, aDstY, aWidth, aHeight));
        __(glActiveTexture(GL_TEXTURE0));
        __(glBindTexture(GL_TEXTURE_2D, GetTextureId(GL_DEPTH_ATTACHMENT)));

        // Tell the shader which texture unit to use
        mShader->Attach();
        mShader->SetUniformTexture2D("u_depthMap", 0);
        mShader->SetUniformVec4("u_screenRect", glm::vec4(aDstX, aDstY, aWidth, aHeight));

        __(glDisable(GL_DEPTH_TEST));
        glDepthMask(GL_FALSE);
        //__(glEnable(GL_BLEND));
        //__(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        const OpenGLRenderer* lGLRenderer = static_cast<const OpenGLRenderer*>(Renderer::GetInstance());
        __(glBindVertexArray(lGLRenderer->GetTempVAO()));
        __(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4)); // a dummy call
        __(glBindVertexArray(GL_NONE));

        //__(glDisable(GL_BLEND));
        glDepthMask(GL_TRUE);
        __(glEnable(GL_DEPTH_TEST));

        mShader->Detach();
    }

}
