#version 330 core

in vec3 textureCoords_o;
uniform samplerCube skybox;
out vec4 FragColor;

void main() {
    FragColor = texture(skybox, textureCoords_o);
}
