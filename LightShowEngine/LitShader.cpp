#include "LitShader.h"
void LitShader::recompileShader(const Settings& currentSettings)
{

	std::string		vertexCode = "";
	std::string		fragmentCode = "";
	std::string		geometryCode = "";


	if (getCode(vertexFilePath, fragmentFilePath, geometryFilePath, vertexCode, fragmentCode, geometryCode) == -1)
	{
		DBG_LOG("There was an error reading this file. (ShaderBase.cpp)\n");
	}
	

	updateTagValues(currentSettings, fragmentCode);

	createProgram(vertexCode, fragmentCode, geometryCode);

	glUseProgram(this->programID);

	for (GLuint i = 0; i < currentSettings.getLightsPerEntity(); i++)
	{
		glUniform1f(glGetUniformLocation(this->programID, ("pointLights[" + std::to_string(i) + "].linear").c_str()), 1.00f);
	}

}

LitShader::LitShader(const Settings& currentSettings, const std::string& vertexPath, const std::string& fragmentPath, const SHADER_TYPE& type, const std::string& geometryPath)
{
	
	std::string		vertexCode		= "";
	std::string		fragmentCode	= "";
	std::string		geometryCode	= "";

	vertexFilePath		= vertexPath;
	fragmentFilePath	= fragmentPath;
	geometryFilePath	= geometryPath;
	shaderType			= type;

	if (getCode(vertexPath, fragmentPath, geometryPath, vertexCode, fragmentCode, geometryCode) == -1)
	{
		DBG_LOG("There was an error reading this file. (ShaderBase.cpp)\n");
	}


	updateTagValues(currentSettings, fragmentCode);
	


	createProgram(vertexCode, fragmentCode, geometryCode);
	
	//Set all of the lights to default value

	glUseProgram(this->programID);

	for (GLuint i = 0; i < currentSettings.getLightsPerEntity(); i++)
	{
		glUniform1f(glGetUniformLocation(this->programID, ("pointLights[" + std::to_string(i) + "].linear").c_str()), 1.00f);
	}
	
}

void LitShader::setPointLight(const Settings& currentSettings, const PointLight & light, unsigned int index)
{
	if (index < currentSettings.getLightsPerEntity())
	{
		std::string location = "pointLights[" + std::to_string(index);

		supplyVec3fUniform	(location + "].position",	light.position);
		supplyVec3fUniform	(location + "].ambient",	light.ambient);
		supplyVec3fUniform	(location + "].diffuse",	light.diffuse);
		supplyVec3fUniform	(location + "].specular",	light.specular);
		supply1fUniform		(location + "].constant",	light.constant);
		supply1fUniform		(location + "].linear",		light.linear);
		supply1fUniform		(location + "].quadratic",	light.quadratic);

	}
}

void LitShader::setDirectionalLight(const DirectionalLight & directionalLight)
{
	supplyVec3fUniform(Shaders::getUniformName(Shaders::UniformName::DirectionalLightDirection),	directionalLight.direction);
	supplyVec3fUniform(Shaders::getUniformName(Shaders::UniformName::DirectionalLightAmbient),		directionalLight.ambient);
	supplyVec3fUniform(Shaders::getUniformName(Shaders::UniformName::DirectionalLightDiffuse),		directionalLight.diffuse);
	supplyVec3fUniform(Shaders::getUniformName(Shaders::UniformName::DirectionalLightSpecular),		directionalLight.specular);
}

void LitShader::setMaterial(const Material& material)
{
	supplyVec3fUniform	(Shaders::getUniformName(Shaders::UniformName::MaterialDiffuse),	material.diffuse	);
	supplyVec3fUniform	(Shaders::getUniformName(Shaders::UniformName::MaterialAmbient),	material.ambient	);
	supplyVec3fUniform	(Shaders::getUniformName(Shaders::UniformName::MaterialSpecular),	material.specular	);
	supply1fUniform		(Shaders::getUniformName(Shaders::UniformName::MaterialShininess),	material.shininess	);
}

void LitShader::setSimpleMaterial(const SimpleMaterial & material)
{
	setShininess(material.shininess);
}

void LitShader::setShininess(const float & shininess)
{
	supply1fUniform(Shaders::getUniformName(Shaders::UniformName::MaterialShininess), shininess);
}

void LitShader::updateTagValues(const Settings& currentSettings, std::string & fragmentCode)
{



	updateTagValue
		(
			Shaders::MAX_LIGHT_TAG,
			std::to_string(currentSettings.getLightsPerEntity()),
			fragmentCode
			);

	updateTagValue
		(
			Shaders::SHADOW_INTENSITY_TAG,
			std::to_string(currentSettings.getShadowIntensity()),
			fragmentCode
			);

	updateTagValue
		(
			Shaders::SHADOW_FILTERING_TAG,
			std::to_string(currentSettings.getShadowFilterAmount()),
			fragmentCode
			);

}
