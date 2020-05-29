/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*  Brief       : A render target allows to render objects to it instead of
*                to the main screen. The target can then be blitted onto the screen
*
*                The ShadowMap render target applies no anti-aliasing
*******************************************************************************/
#pragma once

#include "graphic/rendertarget.h"

namespace Framework
{
    class ShadowMapRenderTarget : public virtual RenderTarget
    {
    public:
        ShadowMapRenderTarget();

        static ShadowMapRenderTarget *New();
    };
}

