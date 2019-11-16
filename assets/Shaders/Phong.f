#version 330 core
out vec4 color;

struct Material {
	sampler2D texture_diffuse1;
    vec3 specular;
    float shininess;
}; 
struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;

uniform Light light;    
  
uniform vec3 viewPosition;

in vec2 textureCoords_o;
in vec3 normal_o;
in vec3 fragPosition_o; 

void main()
{

	//Diffuse
	vec3 norm		= normalize(normal_o);
	vec3 lightDir	= normalize(light.position - fragPosition_o);  
	float diff		= max(dot(norm, lightDir), 0.0);
	vec3 diffuse	= light.diffuse * diff * vec3(texture(material.texture_diffuse1, textureCoords_o));  

	//Ambient
	vec3 ambient	= light.ambient * vec3(texture(material.texture_diffuse1, textureCoords_o));
	
	//Specular
	vec3 viewDir	= normalize(viewPosition - fragPosition_o);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec		= pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular	= light.specular * (spec * material.specular);  

	



    vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0f);

}