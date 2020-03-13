#version 330 core

layout(location = 0) in vec3 position; // The position variable has attribute position 0

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 textureCoords_o;

void main() {
    textureCoords_o     = position;
    gl_Position         = (projection * view * vec4(position, 1.0)).xyww;
}
