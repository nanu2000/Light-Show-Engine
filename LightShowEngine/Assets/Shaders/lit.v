#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoords;

out vec3 position_o;
out vec3 normal_o;
out vec2 textureCoords_o;
out vec3 fragPosition_o;
out vec4 fragmentPositionLightSpace_o;

void main()
{    
              	// Transform the vertex information based on bones.

    

    gl_Position 		= projection * view  * model  * vec4(position,1.0f) ;
    position_o 			= (view * model * vec4(position, 1.0)).xyz;	
    normal_o 			= transpose(inverse(mat3(model))) * normal;
	fragPosition_o		= vec3(model * vec4(position, 1.0f));
    textureCoords_o		= textureCoords;
    fragmentPositionLightSpace_o = lightSpaceMatrix * vec4(fragPosition_o, 1.0);
}