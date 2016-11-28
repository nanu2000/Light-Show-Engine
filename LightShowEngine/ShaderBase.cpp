#include "ShaderBase.h"

SHADER_TASK ShaderBase::currentTask	= SHADER_TASK::Normal_Render_Task;
GLint ShaderBase::shadersForTasks	  [SHADER_TASK::SHADER_TASK_MAX];

ShaderBase::ShaderBase(const std::string& vertexPath, const std::string& fragmentPath,const SHADER_TYPE& type, const std::string& geometryPath)
{
	std::string		vertexCode		= "";
	std::string		fragmentCode	= "";
	std::string		geometryCode	= "";
	shaderType						= type;

	vertexFilePath		= vertexPath;
	fragmentFilePath	= fragmentPath;
	geometryFilePath	= geometryPath;


	if (getCode(vertexPath, fragmentPath, geometryPath, vertexCode, fragmentCode, geometryCode) == -1)
	{
		DBG_LOG("There was an error reading this file. (ShaderBase.cpp)\n");
	}

	createProgram(vertexCode, fragmentCode, geometryCode);
		
}

SHADER_TASK ShaderBase::getShaderTask()
{
	return currentTask;
}

GLint ShaderBase::createAndCompileShader(int GLShaderType, const GLchar * const * code)
{
	GLint shader  = glCreateShader(GLShaderType);

	glShaderSource	(shader, 1, code, NULL);
	glCompileShader	(shader);

	#ifdef DEBUG
	GLint	success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == false)
	{
		GLchar	infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);

		DBG_LOG("Shader Compilation Failed. Error:\n");
		DBG_LOG(infoLog);
		DBG_LOG("\n");
	}
	#endif

	return shader;
}

int ShaderBase::getCode(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath, std::string& vertexCode, std::string& fragmentCode, std::string& geometryCode)
{
	std::ifstream	vShaderFile;
	std::ifstream	fShaderFile;

	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	
	try
	{
		std::stringstream vShaderStream;
		std::stringstream fShaderStream;

		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		if (!geometryPath.empty())
		{
			std::ifstream	gShaderFile;
			gShaderFile.exceptions(std::ifstream::badbit);
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode		= vShaderStream.str();
		fragmentCode	= fShaderStream.str();
		
	}
	catch (std::ifstream::failure e)
	{		
		return -1;
	}
		
	return 0;
}

void ShaderBase::recompileShader()
{

	std::string		vertexCode = "";
	std::string		fragmentCode = "";
	std::string		geometryCode = "";
	
	if (getCode(vertexFilePath, fragmentFilePath, geometryFilePath, vertexCode, fragmentCode, geometryCode) == -1)
	{
		DBG_LOG("There was an error reading this file. (ShaderBase.cpp)\n");
	}

	createProgram(vertexCode, fragmentCode, geometryCode);

}



void ShaderBase::createProgram(const std::string& vShaderCode, const std::string& fShaderCode, const std::string& gShaderCode)
{
	const char*c_str;

	GLint vertex	= createAndCompileShader(GL_VERTEX_SHADER, &(c_str = vShaderCode.c_str()));
	GLint fragment	= createAndCompileShader(GL_FRAGMENT_SHADER, &(c_str = fShaderCode.c_str()));
	GLint geometry;

	this->programID = glCreateProgram();

	if (!gShaderCode.empty())
	{
		geometry = createAndCompileShader(GL_GEOMETRY_SHADER, &(c_str = gShaderCode.c_str()));
		glAttachShader(this->programID, geometry);
	}

	glAttachShader(this->programID, vertex);
	glAttachShader(this->programID, fragment);

	glLinkProgram(this->programID);

	#ifdef DEBUG
	GLint	success;
	glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar	infoLog[512];
		glGetProgramInfoLog(this->programID, 512, NULL, infoLog);
		DBG_LOG("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
		DBG_LOG(infoLog);
		DBG_LOG("\n");

		if (fShaderCode.empty() || vShaderCode.empty())
		{
			DBG_LOG("EMPTY SHADER (ShaderBase.cpp ShaderBase::createProgram())\n");
		}
	}
	#endif

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDetachShader(this->programID, vertex);
	glDetachShader(this->programID, fragment);
	
	if (!gShaderCode.empty())
	{
		glDeleteShader(geometry);
		glDetachShader(this->programID, geometry);
	}


}

void ShaderBase::useProgram()
{
	static GLint lastUsed = -1;

	if (lastUsed != getProgramID())
	{
		glUseProgram(getProgramID());
		lastUsed = getProgramID();
	}
}

void ShaderBase::supplyVec3fUniform(const std::string & location, const glm::vec3 & value)
{
	glUniform3f
	(
		glGetUniformLocation
		(
			this->programID,
			location.c_str()
		),
		value.x,
		value.y,
		value.z
	);
}

void ShaderBase::supply1fUniform(const std::string & location, const float & value)
{
	glUniform1f
	(
		glGetUniformLocation
		(
		this->programID,
		location.c_str()
		),
		value
	);
}

