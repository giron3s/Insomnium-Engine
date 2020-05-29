#version 330 core

in vec3 in_vertex;

uniform mat4 u_MVPMatrix;

void main() {
    gl_Position = u_MVPMatrix * vec4(in_vertex, 1.0f);
}
