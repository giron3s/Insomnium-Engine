#version 330 core

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uvcoord;

uniform mat4 u_MVPMatrix;
uniform mat4 u_ModelMatrix;

out vec3 io_fragVertex;
out vec3 io_fragNormal;
out vec2 io_fragUVCoord;


void main()
{ 
	vec4 v = vec4(in_vertex, 1.0);
	gl_Position = u_MVPMatrix * v;
    io_fragVertex = (u_ModelMatrix * v).xyz; // vertex position in world space
    io_fragNormal = (u_ModelMatrix * vec4(in_normal, 0.0)).xyz; // normal in world space
    io_fragUVCoord = in_uvcoord;
}
