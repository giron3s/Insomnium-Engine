#version 330 core

uniform vec4 u_color;

layout (location = 0) out vec4 o_color;
layout (location = 1) out vec3 o_modelId; 
layout (location = 2) out vec3 o_position; 
layout (location = 3) out vec4 o_normal;

void main()
{
	o_color = u_color;
	o_modelId = vec3(0.0, 0.0, 0.0);
	o_position = vec3(0.0, 0.0, 0.0);
	o_normal = vec4(0.0, 0.0, 0.0, -1.0); // disable lighting on this fragment
}
