#version 330 core


layout (location = 0) in vec3 position;
layout (location = 2) in vec2 textureCoords;
uniform mat4 model;

out vec2 textureCoords_o;

void main()
{    

    gl_Position 		= model * vec4(position,1.0f) ;
    textureCoords_o		= vec2(textureCoords.x, 1.0f - textureCoords.y);;
}