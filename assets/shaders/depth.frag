#version 330 core

struct Material {
    sampler2D texture_diffuse1;
};

uniform Material material;

in vec2 textureCoords_o;

out vec4 FragColor;
  
void main(){ 
    float depthValue = texture(material.texture_diffuse1, textureCoords_o).r;
    FragColor = vec4(vec3(depthValue), 1.0);
}