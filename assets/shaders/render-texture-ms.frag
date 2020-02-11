#version 330 core

struct Material {
    sampler2DMS texture_diffuse1;
};

uniform Material material;
uniform int MultisampleCount;

in vec2 textureCoords_o;

out vec4 FragColor;

void main(){ 
    vec2 texSize = textureSize(material.texture_diffuse1);
    ivec2 texCoord = ivec2(textureCoords_o * texSize);

    vec4 color = vec4(0.0);

    for (int i = 0; i < MultisampleCount; i++)
        color += texelFetch(material.texture_diffuse1, texCoord, i);

    color /= float(MultisampleCount);


    FragColor = color;
}