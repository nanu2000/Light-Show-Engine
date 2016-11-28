#version 330 core

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


layout (location = 0) in vec3 position; // The position variable has attribute position 0

void main()
{	  
    gl_Position 		= projection * view * model * vec4(position,1.0f) ;
}