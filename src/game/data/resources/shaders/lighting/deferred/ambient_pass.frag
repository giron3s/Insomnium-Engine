#version 330 core

layout(location = 0) out vec4 o_color;

uniform sampler2D u_diffuseMap;
uniform sampler2D u_normalMap;

uniform vec2      u_screenSize;


void main() 
{ 
	vec2 texCoord = gl_FragCoord.xy/u_screenSize;
	vec4 materialDiffuseColor = texture(u_diffuseMap, texCoord);
	o_color = materialDiffuseColor; // color for fragment that is not subject to lighting
	
	vec4 normal_ = texture(u_normalMap, texCoord); // normal_: .xyz = world normal, .w = shadow
	if ( normal_.w >= 0.0 )
		o_color *= 0.05; // ambient term
	// else fragment is not subject to lighting
}
