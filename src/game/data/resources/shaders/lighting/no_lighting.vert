#version 330 core

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec2 in_uvcoord;

uniform mat4 u_MVPMatrix;

out vec2 io_fragUVCoord;

void main()
{
	gl_Position = u_MVPMatrix * vec4(in_vertex, 1.0);
	io_fragUVCoord = in_uvcoord;
}
