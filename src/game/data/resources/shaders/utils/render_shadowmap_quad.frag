#version 330 core

uniform float u_v1_near_plane;
uniform float u_v1_far_plane;
uniform sampler2D u_s2dsm_shadowmap;

in vec2 io_fragUVCoord;
out vec4 o_color;

void main() 
{
	vec4 depth = texture(u_s2dsm_shadowmap, io_fragUVCoord.xy);
	o_color = vec4(depth.r, depth.r, depth.r, 1.0);
}