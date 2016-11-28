#version 330 core


layout (location = 0) in vec3 position;
layout (location = 2) in vec2 textureCoords;
layout (location = 5) in vec3 particlePosition;
layout (location = 6) in float scale;
layout (location = 7) in vec4 particleColor;

uniform mat4 view;
uniform mat4 projection;
out vec2 textureCoords_o;
out vec4 particleColor_o;

void main()
{    
 mat4 p = mat4(1.0f);

 p[3][0] = particlePosition.x;
 p[3][1] = particlePosition.y;
 p[3][2] = particlePosition.z;
 p[3][3] = 1.0;

  mat4 modelView = view * p;

  
  // First column.
  modelView[0][0] = 1.0; 
  modelView[0][1] = 0.0;  //remove for cylimdrical rendering
  modelView[0][2] = 0.0; 
 
    // Second column.
  modelView[1][0] = 0.0;  
  modelView[1][1] = 1.0;  //remove for cylimdrical rendering
  modelView[1][2] = 0.0; 
  
 
  // Thrid column.
  modelView[2][0] = 0.0;  
  modelView[2][1] = 0.0;  //remove for cylimdrical rendering
  modelView[2][2] = 1.0; 

    gl_Position 		=  projection * modelView * vec4(position * scale, 1.0f);
    textureCoords_o		= vec2(textureCoords.x, 1.0f - textureCoords.y);

	particleColor_o = particleColor; 
}