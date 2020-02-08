#version 330 core

in vec3 color_o;
out vec4 color;

void main() {
    color = vec4(color_o, 1);
}