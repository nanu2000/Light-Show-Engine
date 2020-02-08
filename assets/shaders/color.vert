#version 330 core

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

layout(location = 0) in vec3 position;
layout(location = 8) in vec3 color;

out vec3 color_o;
void main() {
    color_o = color;
    gl_Position = projection * view * model * vec4(position, 1.0f);
}
