#version 330 core
out vec4 color;


//**********************************************
//These macros are modified on runtime. 
//the /*TAG*/ before the value tells the 
//compiler what value needs to be set and where.
//***********************************************

#define AMOUNT_OF_POINT_LIGHTS	/*MAL*/4//
#define SHADOW_INTENSITY		/*SI*/5//
#define SHADOW_FILTERING		/*SF*/5//

const float shadowFade = 1;

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
    float shininess;
}; 
struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

  

uniform PointLight pointLights[AMOUNT_OF_POINT_LIGHTS];

uniform DirectionalLight directionalLight;



uniform Material	material;
uniform samplerCube	pointShadowMap;  
uniform sampler2D	directionalShadowMap;  
uniform float		farPlane;
uniform vec3		viewPosition;
uniform vec3		lightPosition;

in vec4 fragmentPositionLightSpace_o;
in vec2 textureCoords_o;
in vec3 normal_o;
in vec3 fragPosition_o; 

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadowCalculation);

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1),
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1)
);


float fadeShadowOverDistance(vec2 shadowCoord)
{
	vec4 coord = vec4(shadowCoord, 1. - shadowCoord);
	vec4 mu = clamp(coord / shadowFade, 0., 1.);
	vec2 mu2 = min(mu.xy, mu.zw);
	return min(mu2.x, mu2.y);
}


float directionalShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords		= (fragPosLightSpace.xyz / fragPosLightSpace.w) * 0.5f + 0.5f;

	float bias = max(0.0003 * (1.0 - dot(normal_o, directionalLight.direction)), .00015);  
    float shadow		= 0.0f;
	vec2 textureSize	= textureSize(directionalShadowMap, 0);

	float closestDepth = texture(directionalShadowMap, projCoords.xy).r; 


	vec2 texelSize = 1.0 / textureSize;
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;



  
	shadow *= fadeShadowOverDistance(projCoords.xy);
	shadow *=  SHADOW_INTENSITY;
    if(projCoords.z > 1.0f)
	{
        shadow = 0.0f;
    }
	                
    return shadow;
	
}

float pointLightShadowCalculation(vec3 fragPos, PointLight pointLight)
{
	vec3 fragToLight = fragPos - pointLight.position;
	float currentDepth = length(fragToLight);  
	float shadow = 0.0;
	float bias   = 0.15;
	float viewDistance = length(viewPosition - fragPos);
	float diskRadius = 0.05;
	for(int i = 0; i < SHADOW_FILTERING; ++i)
	{
		float closestDepth = texture(pointShadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= farPlane;
		if(currentDepth - bias > closestDepth){
			shadow += 1.0;
		}
	}
	shadow /= float(SHADOW_FILTERING);  

    return shadow;
} 

void main()
{
    // Properties
    vec3 norm		= normalize(normal_o);
    vec3 viewDir	= normalize(viewPosition - fragPosition_o);

    // Phase 1: Directional lighting
    vec3 result = CalcDirLight(directionalLight, norm, viewDir);
	
	
	float pointLightShadow = (1.0f - pointLightShadowCalculation(fragPosition_o, pointLights[0]));

    // Phase 2: Point lights
    for(int i = 0; i < AMOUNT_OF_POINT_LIGHTS; i++)
	{
       result += CalcPointLight(pointLights[i], norm, fragPosition_o, viewDir, pointLightShadow / AMOUNT_OF_POINT_LIGHTS);    
	}

	//for opacity, use the W member of the texture like so:
    //color = vec4(result, texture(material.texture_diffuse1, textureCoords_o).w);
	
    color = vec4(result,texture(material.texture_diffuse1, textureCoords_o).w);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);


    // Specular shading

	vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);


    // Combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, textureCoords_o));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, textureCoords_o));
    vec3 specular = .04f * light.specular * spec;

	return (ambient + (1.0 - directionalShadowCalculation(fragmentPositionLightSpace_o)) * ( diffuse + specular));    

}  

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadowCalculation)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, textureCoords_o));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, textureCoords_o));
    vec3 specular = light.specular * spec;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
	  return (ambient + shadowCalculation * (diffuse + specular));

} 
