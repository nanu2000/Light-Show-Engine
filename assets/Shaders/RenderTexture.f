#version 330 core
out vec4 FragColor;
  
in vec2 textureCoords_o;

struct Material {
    sampler2D texture_diffuse1;
};

uniform Material material;

void main()
{ 

    vec3 col = texture(material.texture_diffuse1, textureCoords_o).rgb;

    FragColor = vec4(col, 1.0);
}