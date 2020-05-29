#version 330 core

flat in int ColorIndex;
flat in int Id;

layout (location = 0) out vec4 o_diffuse; 
layout (location = 1) out vec3 o_modelId;
layout (location = 2) out vec3 o_position;
layout (location = 3) out vec4 o_normal_; // xyz - world coords, w - shadow factor

const vec3 palette[4] = vec3[]
(
	vec3(1.0, 0.0, 0.0), // red
	vec3(0.0, 1.0, 0.0), // green
	vec3(0.0, 0.0, 1.0), // blue
	vec3(0.8706, 0.8706, 0.8706)  // 'almost' white, corresponds to 222,222,222 in bytes
);

void main() {
	o_diffuse = vec4(palette[ColorIndex], 1.0);
	o_modelId = palette[Id];
	o_position = vec3(0.0, 0.0, 0.0);
	o_normal_ = vec4(0.0, 0.0, 0.0, -1.0); // disable lighting on this fragment
}
