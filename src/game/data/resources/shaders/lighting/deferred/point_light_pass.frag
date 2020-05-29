#version 330 core

layout(location = 0) out vec4 o_color;

uniform sampler2D u_diffuseMap;
uniform sampler2D u_positionMap;
uniform sampler2D u_normalMap; /* xyz = normal in world space, w = lighting flag:
                                  0 = fragment is not subject to lighting,
                                  1 = fragment is subject to lighting but not a shadow receiver,
                                  2 = fragment is subject to lighting and a shadow receiver */

uniform vec2      u_screenSize;

uniform vec3      u_LightPosition; // in world space
uniform vec4      u_LightColor;
uniform float     u_LightRadius;

uniform sampler2DShadow u_shadowMap;
uniform mat4            u_LVPMatrix_sb; // light view-projection matrix, scaled and biased


void main() 
{ 
	o_color = vec4(0.0, 0.0, 0.0, 0.0); // output color assiming it is applied via additive blending
	vec2 texCoord = gl_FragCoord.xy/u_screenSize;
	vec4 normal_ = texture(u_normalMap, texCoord);
	if ( normal_.w == 0.0 )
		return; // fragment Not subject to lighting
	
	vec3 position = texture(u_positionMap, texCoord).xyz; // vertex position in world space
	vec3 N = normalize(normal_.xyz); // vertex normal vector in world space, normalized
	vec3 L = normalize(u_LightPosition - position); // vertex to light vector in world space, normalized
	float kd = dot(N, L); // diffuse factor
	if ( kd > 0.0 ) {
		float dist = distance(u_LightPosition, position);
	    float att = clamp(1.0 - dist*dist/(u_LightRadius*u_LightRadius), 0.0, 1.0);
		att *= att;
		if ( att > 0.0 )
		{
			vec4 materialDiffuseColor = texture(u_diffuseMap, texCoord);
			o_color += att * u_LightColor * materialDiffuseColor * kd; // add diffuse term
			vec3 V = normalize(-position); // vertex to eye vector in world space, normalized
			vec3 H = normalize(u_LightPosition + V); // halfway vector in world space, normalized
			float ks = dot(N, H); // specular factor
			if ( ks > 0.0 )
			{
				const vec4 materialSpecularColor = vec4(0.5, 0.5, 0.5, 1.0); // TODO...
				const vec4 lightSpecularColor = vec4(0.5, 0.5, 0.5, 1.0); // TODO...
				const float materialSpecularIntensity = 60.0; // TODO...
				o_color += att * lightSpecularColor* materialSpecularColor * pow(ks, materialSpecularIntensity); // add specular term
			}
			// apply shadow
			if ( normal_.w == 2.0 ) { // if a shadow receiver is being rendered
				vec4 shadow_coordinates = u_LVPMatrix_sb * vec4(position, 1.0); // vertex position from light point of view in clip volume [0,1]
				if ( shadow_coordinates.w != 0.0 ) { // if shadow exists
					shadow_coordinates /= shadow_coordinates.w; // xyz = perspective corrected shadow coordinates
					if( shadow_coordinates.x >= 0.0 && shadow_coordinates.y >= 0.0 && shadow_coordinates.z >= 0.0 && 
						shadow_coordinates.x <= 1.0 && shadow_coordinates.y <= 1.0 && shadow_coordinates.z <= 1.0 )
					{
						const float z_bias = 0.0005;
						float shadow_factor = texture(u_shadowMap, shadow_coordinates.xyz, z_bias); // : [0.0, 1.0] = [fully shadowed, not shadowed]
						o_color.rgb *= max(0.5, shadow_factor);
						o_color.a = 0.5;
					}
				}
			}
		}
	}
}
