//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

in vec2 f_texcoord;

uniform sampler2D glyph;
uniform vec4 color;

out vec4 fragColor;

void main(void) {
    float intensity = texture(glyph, f_texcoord).r;
    fragColor = color * intensity;
}
