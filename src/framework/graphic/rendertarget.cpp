/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*  Brief       : A render target allows to render objects to it instead of
*                to the main screen. The target can then be blitted onto the
*                screen
*
*                The ShadowMap render target applies no anti-aliasing
*
*******************************************************************************/

#include "precompiled.h"
#include "graphic/opengl/opengl_rendertarget.h"

namespace Framework
{

    RenderTarget* RenderTarget::New()
    {
        return new OpenGLRenderTarget();
    }


    void RenderTarget::SetClearValue(uint32_t aAttachment, const float* aClearValue)
    {
        auto attachment = mAttachments.find(aAttachment);
        if ( attachment != mAttachments.end() )
        {
            if ( GL_COLOR_ATTACHMENT0 <= attachment->first && attachment->first <= GL_COLOR_ATTACHMENT0 + mNumColorBuffers )
            {
                if ( 0.0f <= aClearValue[0] && aClearValue[0] <= 1.0f &&
                     0.0f <= aClearValue[1] && aClearValue[1] <= 1.0f &&
                     0.0f <= aClearValue[2] && aClearValue[2] <= 1.0f &&
                     0.0f <= aClearValue[3] && aClearValue[3] <= 1.0f )
                    memcpy(attachment->second.ClearValue, aClearValue, sizeof(float) * 4);
                else
                    ASSERT(0); // Invalid parameter
            }
            else if ( attachment->first == GL_DEPTH_ATTACHMENT ||
                      attachment->first == GL_STENCIL_ATTACHMENT )
            {
                if ( 0.0f <= aClearValue[0] && aClearValue[0] <= 1.0f )
                    attachment->second.ClearValue[0] = aClearValue[0];
                else
                    ASSERT(0); // Invalid parameter
            }
            else if ( attachment->first == GL_DEPTH_STENCIL_ATTACHMENT )
            {
                if ( 0.0f <= aClearValue[0] && aClearValue[0] <= 1.0f &&
                     0.0f <= aClearValue[1] && aClearValue[1] <= 1.0f )
                    memcpy(attachment->second.ClearValue,  aClearValue, sizeof(float) * 2);
                else
                    ASSERT(0); // Invalid parameter
            }
            else
                ASSERT(0); // Invalid parameter
        }
    }

}
