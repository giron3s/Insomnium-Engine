#version 330 core

in vec2 io_fragUVCoord;

uniform sampler2D u_shapeMap;
uniform sampler2D u_overlayMap;
uniform bool      u_useOverlayMap;

uniform vec3 u_modelId;

layout(location = 0) out vec4 o_color;
layout(location = 1) out vec3 o_modelId;


void main()
{
	o_color = texture(u_shapeMap, io_fragUVCoord);
	if ( u_useOverlayMap )
		o_color.rgb = texture(u_overlayMap, io_fragUVCoord).rgb;
	o_modelId = u_modelId;
}
