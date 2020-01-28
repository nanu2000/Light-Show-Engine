#version 330 core

uniform mat4 model;

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 textureCoords;

out vec2 textureCoords_o;

void main()
{
    textureCoords_o = textureCoords;
    gl_Position = model * vec4(position, 1.0); 
}