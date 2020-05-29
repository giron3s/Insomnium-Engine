#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 2) in vec3 normal;
layout(location = 1) in vec3 UVCoord;

uniform vec2 u_scale;
uniform vec2 u_offset;
out vec2 io_fragUVCoord;

void main() 
{
    vec2 lScaledVertex = (vertex.xy * u_scale);
	io_fragUVCoord = (lScaledVertex.xy + 1.0) * 0.5;
	gl_Position  = vec4(lScaledVertex + u_offset, 0.0, 1.0);
}