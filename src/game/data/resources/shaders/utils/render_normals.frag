#version 330 core

in vec4 io_vertexColor;

layout (location = 0) out vec4 o_color;
layout (location = 1) out vec3 o_modelId; 
layout (location = 2) out vec3 o_position; 
layout (location = 3) out vec3 o_normal;

void main()
{
	o_color = io_vertexColor;
	o_modelId = vec3(0.0, 0.0, 0.0);
	o_position = vec3(0.0, 0.0, 0.0);
	o_normal = vec3(0.0, 0.0, 0.0); // to disable lighting on this fragment
}