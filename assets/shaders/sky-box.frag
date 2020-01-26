#version 330 core

uniform samplerCube skybox;
in vec3 textureCoords_o;
out vec4 FragColor;

void main() {
    FragColor = texture(skybox, textureCoords_o);
}
