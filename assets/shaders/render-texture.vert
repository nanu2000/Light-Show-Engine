#version 330 core

layout(location = 0) in vec2 position;
layout(location = 2) in vec2 textureCoords;

out vec2 textureCoords_o;

void main()
{
    textureCoords_o = textureCoords;
    gl_Position = vec4(position.x, position.y, 0.0, 1.0); 
}  