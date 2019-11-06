#version 330 core

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 TexCoords;
layout (location = 0) in vec3 position; // The position variable has attribute position 0

void main()
{	  
	TexCoords = position;
    vec4 pos = projection * view * vec4(position, 1.0);
    gl_Position = pos.xyww;
}