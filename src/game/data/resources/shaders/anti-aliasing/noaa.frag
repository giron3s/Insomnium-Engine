#version 330 core

uniform sampler2D u_image;
uniform vec4      u_screenRect; // (left, bottom, width, height)

out vec4 o_color;

/*float sRGB2Linear(float c) {
    if (c <= 0.04045) {
        return c/12.92;
    }
    return pow((c + 0.055)/1.055, 2.4);
}*/

void main(void) {
	vec2 texCoord = (gl_FragCoord.xy - u_screenRect.xy)/u_screenRect.zw;
	o_color = texture(u_image, texCoord);
/*
    // sRGB to linear conversion, OpenGL does not support
    // the shader outputting sRGB when GL_FRAMEBUFFER_SRGB is
    // enabled  
    o_color= vec4(sRGB2Linear(o_color.r),
                    sRGB2Linear(o_color.g),
                    sRGB2Linear(o_color.b),
                    o_color.a);
*/
}
