//----- fragment shader -----
#version 330 core

uniform vec4 Color;

layout (location = 0) out vec4 o_color;

void main() {
	gl_FragDepth = 0.999999;
	o_color = Color;
}
//---------------------------
