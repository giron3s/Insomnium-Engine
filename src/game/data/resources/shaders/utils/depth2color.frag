#version 330 core

uniform sampler2D u_depthMap;
uniform vec4      u_screenRect; // (left, bottom, width, height)

out vec4 o_color;

/*float zFar  = 100.0f;
float zNear = 1.0f;

float LinearizeDepth(float z_b)
{
    float z_n = 2.0 * z_b - 1.0;
    float z_e = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));

    return z_e;
}*/

void main(void) {
	vec2 texCoord = (gl_FragCoord.xy - u_screenRect.xy)/u_screenRect.zw;
	o_color = texture(u_depthMap, texCoord);
}
