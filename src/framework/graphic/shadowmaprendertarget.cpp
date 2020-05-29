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
#include "graphic/opengl/opengl_shadowmaprendertarget.h"

namespace Framework
{

    ShadowMapRenderTarget::ShadowMapRenderTarget()
        : RenderTarget()
    {
        /* TODO: we have 1 shadowmaprendertarget per light, so if there are
        N lights we load depth2color shader N times. Waste of resources! */
        ASSERT(!mShader);
        mShader.reset(Shader::New("depth to color"));
        std::string lError;
        if ( mShader->Load("utils/depth2color", lError) == false )
            CRASH("ERROR loading shader utils/depth2color: %s\n", lError.c_str());
        mName.assign("RT_ShadowMap");
    }

    ShadowMapRenderTarget *ShadowMapRenderTarget::New(void)
    { return new OpenGLShadowMapRenderTarget(); }
}
