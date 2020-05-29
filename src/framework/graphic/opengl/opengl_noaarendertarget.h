/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Render target for OpenGL. A render target allows to render objects to it
 *                instead of to the main screen. Then the target can be rendered to the main screen as
 *                a texture
 *
 *                The NoAA render target applies no anti-aliasing
 ********************************************************************************/
#pragma once


#include "graphic/noaarendertarget.h"
#include "graphic/shader.h"

#include "graphic/opengl/opengl.h"
#include "graphic/opengl/opengl_filterrendertarget.h"

#pragma warning(disable : 4250)

namespace Framework
{
    class OpenGLNoAARenderTarget : public NoAARenderTarget, public OpenGLFilterRenderTarget
    {
      public:
        /*
        void SetCustomParams(void)
        {
            __(glDisable(GL_DEPTH_TEST));
            __(glEnable(GL_BLEND));
            __(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        }
        void UnsetCustomParams(void)
        {
            __(glDisable(GL_BLEND));
            __(glEnable(GL_DEPTH_TEST));
        }*/
    };
}
