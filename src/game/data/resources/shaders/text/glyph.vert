//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

// Input parameters
layout(location = 0) in vec2 vertex; // in screen space (in pixels)
layout(location = 1) in vec2 texcoord;

// Output parameters for the fragment shader
out vec2 f_texcoord;

uniform mat4 glyphTransform; // to NDC
uniform vec2 glyphPos; // in screen space (in pixels)
uniform float scale;

void main(void) {
    gl_Position = glyphTransform * vec4(glyphPos + vertex * scale, 0.0, 1.0);
    f_texcoord = texcoord;
}
