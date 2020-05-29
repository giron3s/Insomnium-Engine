//----- fragment shader -----
#version 330 core

uniform vec4 Color;

layout (location = 0) out vec4 o_color;

void main() {
	o_color = Color;
}
//---------------------------
