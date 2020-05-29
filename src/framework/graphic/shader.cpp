/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Interface for software/hardware renderers
 *******************************************************************************/

#include "precompiled.h"
#include "graphic/shader.h"
#include "graphic/opengl/opengl_shader.h"

namespace Framework
{
    Shader *Shader::New(const std::string& aName) 
    { return new OpenGLShader(aName); }
}
