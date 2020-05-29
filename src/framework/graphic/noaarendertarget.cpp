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
 *                The NOAA render target applies no anti-aliasing
 *******************************************************************************/


#include "precompiled.h"
#include "graphic/opengl/opengl_noaarendertarget.h"

namespace Framework
{
    NoAARenderTarget::NoAARenderTarget() : RenderTarget()
    {
        ASSERT(!mShader);
        mShader.reset(Shader::New("NoAA"));
        std::string lError;
        if ( mShader->Load("anti-aliasing/noaa", lError) == false )
            CRASH("ERROR loading shader anti-aliasing/noaa: %s\n", lError.c_str());
        mName.assign("RT_NoAA");
    }

    NoAARenderTarget *NoAARenderTarget::New()
    { return new OpenGLNoAARenderTarget(); }
}

