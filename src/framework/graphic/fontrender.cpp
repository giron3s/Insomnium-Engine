/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Uses a TrueType font to render text on a RenderTarget
 ********************************************************************************/
#include "precompiled.h"

#include "graphic/fontrender.h"
#include "graphic/opengl/opengl_fontrenderer.h"

namespace Framework
{
    FontRenderer *FontRenderer::New(void) { return new OpenGLFontRenderer(); }
    void FontRenderer::Delete(FontRenderer *aFontRenderer) { delete aFontRenderer; }
}

