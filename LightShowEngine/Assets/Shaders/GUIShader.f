#version 330 core

struct Material 
{
	sampler2D texture_diffuse1;
};

uniform Material material;

in vec2 textureCoords_o;

out vec4 color;

void main()
{
    color = texture(material.texture_diffuse1, textureCoords_o);
} 