#version 330 core
out vec4 color;
in vec2 TextureCoords_o;

uniform sampler2D depthMap;

void main() {
    float depthValue = texture(depthMap, TextureCoords_o).r;
    color            = vec4(vec3(depthValue), 1.0);
}
