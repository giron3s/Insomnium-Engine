#version 330 core

/* Input from vertex shader */
in vec3 io_fragVertex;
in vec3 io_fragNormal;
in vec2 io_fragUVCoord;

layout (location = 0) out vec4 o_diffuse;
layout (location = 1) out vec3 o_modelId;
layout (location = 2) out vec3 o_position;
layout (location = 3) out vec4 o_normal;

uniform sampler2D u_diffuseMap;
uniform vec3      u_modelId;
uniform float     u_lightingFlag;/* lighting flag:
                                    0 = fragment is not subject to lighting,
                                    1 = fragment is subject to lighting but not a shadow receiver,
                                    2 = fragment is subject to lighting and a shadow receiver */


void main() 
{ 
    o_diffuse = texture(u_diffuseMap, io_fragUVCoord); // rgba
    o_modelId = u_modelId; 
    o_position = io_fragVertex; // xyz
    o_normal = vec4(io_fragNormal, u_lightingFlag); // xyz (don't normalize here) + lighting flag in w
}
