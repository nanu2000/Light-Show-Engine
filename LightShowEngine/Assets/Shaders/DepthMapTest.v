#version 330 core
out vec4 color;
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
out vec2 TextureCoords_o;

void main()
{             
gl_Position = vec4(position, 1);
TextureCoords_o = textureCoords;
}  