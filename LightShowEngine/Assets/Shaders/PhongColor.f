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
#define SHADOW_FILTER_DISTANCE  /*SFD*/1.2//

const float shadowFade = 1;

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

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

uniform float opacityControl;

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
 
// array of offset direction for sampling
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
	vec4 coord	= vec4(shadowCoord, 1. - shadowCoord);
	vec4 mu		= clamp(coord / shadowFade, 0., 1.);
	vec2 mu2	= min(mu.xy, mu.zw);
	return min(mu2.x, mu2.y);
}

float directionalShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords		= (fragPosLightSpace.xyz / fragPosLightSpace.w) * 0.5f + 0.5f;
    float bias			= max(0.001f * (1.0f - dot(normalize(normal_o), normalize(directionalLight.direction))), 0.002f);  
    float shadow		= 0.0f;
	vec2 textureSize	= textureSize(directionalShadowMap, 0);

	vec2 filterAmount = ((1.0 / textureSize) / 1 * (SHADOW_FILTER_DISTANCE * (textureSize / 2048)));

    for(int i = 0; i < SHADOW_FILTERING; ++i)
    {
            float pcfDepth = 
			texture(directionalShadowMap, projCoords.xy + sampleOffsetDirections[i].xy * filterAmount).r; 
            
			if(projCoords.z - bias > pcfDepth)
			{
				shadow += 1.0f / SHADOW_FILTERING;   
			} 
    }    
  
	shadow *= fadeShadowOverDistance(projCoords.xy);
	shadow *=  SHADOW_INTENSITY;
    if(projCoords.z > 1.0f)
	{
        shadow = 0.0f;
    }
	                
    return shadow;
	
}

float pointLightShadowCalculation(vec3 fragPos)
{
    vec3 fragToLight	= fragPos - lightPosition;
    float currentDepth	= length(fragToLight);
    float shadow		= 0.0f;
    float bias			= 0.15f;
    float viewDistance	= length(viewPosition - fragPos);
    float diskRadius	= (1.0f + (viewDistance / farPlane)) / 25.0f * SHADOW_FILTER_DISTANCE;

    for(int i = 0; i < SHADOW_FILTERING; ++i)
    {
        float closestDepth = texture(pointShadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= farPlane;

        if(currentDepth - bias > closestDepth)
		{
            shadow += 1.0 / SHADOW_FILTERING;
		}
	}
    return shadow;
} 
void main()
{
    // Properties
    vec3 norm		= normalize(normal_o);
    vec3 viewDir	= normalize(viewPosition - fragPosition_o);

    // Phase 1: Directional lighting
    vec3 result = CalcDirLight(directionalLight, norm, viewDir);

    // Phase 2: Point lights
	float pointLightShadow = (1.0f - pointLightShadowCalculation(fragPosition_o) / float(AMOUNT_OF_POINT_LIGHTS));
    for(int i = 0; i < AMOUNT_OF_POINT_LIGHTS; i++)
	{
       result += CalcPointLight(pointLights[i], norm, fragPosition_o, viewDir, pointLightShadow);    
	}	

    color = vec4(result, opacityControl);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Combine results

	vec3 ambient  = light.ambient * material.ambient;
	vec3 diffuse  = light.diffuse * (diff * material.diffuse);
	vec3 specular = light.specular * (spec * material.specular);  
	
	

    return (ambient +(1.0f - directionalShadowCalculation(fragmentPositionLightSpace_o)) * (diffuse + specular));
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
    float attenuation = 1.0f / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // Combine results
	
	vec3 ambient  = light.ambient * material.ambient;
	vec3 diffuse  = light.diffuse * (diff * material.diffuse);
	vec3 specular = light.specular * (spec * material.specular);
	  
	ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
	
    return (ambient + shadowCalculation * (diffuse + specular));
} 