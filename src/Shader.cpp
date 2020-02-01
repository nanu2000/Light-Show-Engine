#include "Shader.h"
void Shader::recompileShader(const Settings& currentSettings) {

    std::string vertexCode   = "";
    std::string fragmentCode = "";
    std::string geometryCode = "";

    if (getCode(vertexFilePath, fragmentFilePath, geometryFilePath, vertexCode, fragmentCode, geometryCode) == -1) {
        DBG_LOG("There was an error reading this file. (Shader.cpp)\n");
    }

    updateTagValues(currentSettings, fragmentCode);

    createProgram(vertexCode, fragmentCode, geometryCode);

    glUseProgram(this->programID);

    for (GLuint i = 0; i < currentSettings.getLightsPerEntity(); i++) {
        glUniform1f(glGetUniformLocation(this->programID, ("pointLights[" + std::to_string(i) + "].linear").c_str()), 1.00f);
    }
}

Shader::Shader(const std::string& id, const Settings& currentSettings, const std::string& vertexPath, const std::string& fragmentPath, const SHADER_TYPE& type, const std::string& geometryPath) {

    identifier = id;

    std::string vertexCode   = "";
    std::string fragmentCode = "";
    std::string geometryCode = "";

    vertexFilePath   = vertexPath;
    fragmentFilePath = fragmentPath;
    geometryFilePath = geometryPath;
    shaderType       = type;

    if (getCode(vertexPath, fragmentPath, geometryPath, vertexCode, fragmentCode, geometryCode) == -1) {
        DBG_LOG("There was an error reading this file. (Shader.cpp)\n");
    }

    updateTagValues(currentSettings, fragmentCode);

    createProgram(vertexCode, fragmentCode, geometryCode);

    //Set all of the lights to default value

    glUseProgram(this->programID);

    for (GLuint i = 0; i < currentSettings.getLightsPerEntity(); i++) {
        glUniform1f(glGetUniformLocation(this->programID, ("pointLights[" + std::to_string(i) + "].linear").c_str()), 1.00f);
    }
}

void Shader::setPointLight(const Settings& currentSettings, const PointLight& light, unsigned int index) {
    if (index < currentSettings.getLightsPerEntity()) {
        std::string location  = "pointLights[" + std::to_string(index);
        std::string position  = location + "].position";
        std::string ambient   = location + "].ambient";
        std::string diffuse   = location + "].ambient";
        std::string specular  = location + "].specular";
        std::string constant  = location + "].constant";
        std::string linear    = location + "].linear";
        std::string quadratic = location + "].quadratic";
        supplyVec3fUniform(position, light.position);
        supplyVec3fUniform(ambient, light.ambient);
        supplyVec3fUniform(diffuse, light.diffuse);
        supplyVec3fUniform(specular, light.specular);
        supply1fUniform(constant, light.constant);
        supply1fUniform(linear, light.linear);
        supply1fUniform(quadratic, light.quadratic);
    }
}

void Shader::setDirectionalLight(const DirectionalLight& directionalLight) {
    supplyVec3fUniform(Shaders::getUniformName(Shaders::UniformName::DirectionalLightDirection), directionalLight.direction);
    supplyVec3fUniform(Shaders::getUniformName(Shaders::UniformName::DirectionalLightAmbient), directionalLight.ambient);
    supplyVec3fUniform(Shaders::getUniformName(Shaders::UniformName::DirectionalLightDiffuse), directionalLight.diffuse);
    supplyVec3fUniform(Shaders::getUniformName(Shaders::UniformName::DirectionalLightSpecular), directionalLight.specular);
}

void Shader::setMaterial(const Material& material) {
    supplyVec3fUniform(Shaders::getUniformName(Shaders::UniformName::MaterialDiffuse), material.diffuse);
    supplyVec3fUniform(Shaders::getUniformName(Shaders::UniformName::MaterialAmbient), material.ambient);
    supplyVec3fUniform(Shaders::getUniformName(Shaders::UniformName::MaterialSpecular), material.specular);
    supply1fUniform(Shaders::getUniformName(Shaders::UniformName::MaterialShininess), material.shininess);
}

void Shader::setSimpleMaterial(const SimpleMaterial& material) {
    setShininess(material.shininess);
}

void Shader::setShininess(const float& shininess) {
    supply1fUniform(Shaders::getUniformName(Shaders::UniformName::MaterialShininess), shininess);
}

void Shader::updateTagValues(const Settings& currentSettings, std::string& fragmentCode) {

    if (shaderType != SHADER_TYPE::Lit) {
        return;
    }

    try {

        updateTagValue(
            Shaders::MAX_LIGHT_TAG,
            std::to_string(currentSettings.getLightsPerEntity()),
            fragmentCode);

        updateTagValue(
            Shaders::SHADOW_INTENSITY_TAG,
            std::to_string(currentSettings.getShadowIntensity()),
            fragmentCode);

        updateTagValue(
            Shaders::SHADOW_FILTERING_TAG,
            std::to_string(currentSettings.getShadowFilterAmount()),
            fragmentCode);

    } catch (const std::exception& e) {
        DBG_LOG("Exception e: %s\n", e.what());
    }
}

SHADER_TASK Shader::currentTask = SHADER_TASK::Normal_Render_Task;
GLint Shader::shadersForTasks[static_cast<unsigned int>(SHADER_TASK::SHADER_TASK_MAX)];

Shader::Shader(const std::string& id, const std::string& vertexPath, const std::string& fragmentPath, const SHADER_TYPE& type, const std::string& geometryPath) {

    identifier               = id;
    std::string vertexCode   = "";
    std::string fragmentCode = "";
    std::string geometryCode = "";
    shaderType               = type;

    vertexFilePath   = vertexPath;
    fragmentFilePath = fragmentPath;
    geometryFilePath = geometryPath;

    if (getCode(vertexPath, fragmentPath, geometryPath, vertexCode, fragmentCode, geometryCode) == -1) {
        DBG_LOG("There was an error reading this file. (Shader.cpp)\n");
    }

    createProgram(vertexCode, fragmentCode, geometryCode);
}

SHADER_TASK Shader::getShaderTask() {
    return currentTask;
}

GLint Shader::createAndCompileShader(int GLShaderType, const GLchar* const* code) {
    GLint shader = glCreateShader(GLShaderType);

    glShaderSource(shader, 1, code, NULL);
    glCompileShader(shader);

#ifdef DEBUG
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == false) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);

        DBG_LOG("Shader Compilation Failed. Error:\n");
        DBG_LOG(infoLog);
        DBG_LOG("\n");
    }
#endif

    return shader;
}

int Shader::getCode(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath, std::string& vertexCode, std::string& fragmentCode, std::string& geometryCode) {
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);

    try {
        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        if (vertexPath.empty()) {
            DBG_LOG("No path for vertex shader\n");
        }
        if (fragmentPath.empty()) {
            DBG_LOG("No path for fragment shader\n");
        }

        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        if (!geometryPath.empty()) {
            std::ifstream gShaderFile;
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

        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();

#ifdef DEBUG
        if (vertexCode.empty()) {
            DBG_LOG("No code for vertex shader: \n");
            DBG_LOG(vertexPath.c_str());
            DBG_LOG("\n");
        }

        if (fragmentCode.empty()) {
            DBG_LOG("No code for fragment shader: \n");
            DBG_LOG(fragmentPath.c_str());
            DBG_LOG("\n");
        }

        if (geometryCode.empty() && !geometryPath.empty()) {
            DBG_LOG("No code for geometry shader: \n");
            DBG_LOG(geometryPath.c_str());
            DBG_LOG("\n");
        }
#endif

    } catch (std::ifstream::failure e) {
        return -1;
    }

    return 0;
}

void Shader::recompileShader() {

    std::string vertexCode   = "";
    std::string fragmentCode = "";
    std::string geometryCode = "";

    if (getCode(vertexFilePath, fragmentFilePath, geometryFilePath, vertexCode, fragmentCode, geometryCode) == -1) {
        DBG_LOG("There was an error reading this file. (Shader.cpp)\n");
    }

    createProgram(vertexCode, fragmentCode, geometryCode);
}

void Shader::createProgram(const std::string& vShaderCode, const std::string& fShaderCode, const std::string& gShaderCode) {
    const char* c_str;

    GLint vertex   = createAndCompileShader(GL_VERTEX_SHADER, &(c_str = vShaderCode.c_str()));
    GLint fragment = createAndCompileShader(GL_FRAGMENT_SHADER, &(c_str = fShaderCode.c_str()));
    GLint geometry = 0;

    this->programID = glCreateProgram();

    if (!gShaderCode.empty()) {
        geometry = createAndCompileShader(GL_GEOMETRY_SHADER, &(c_str = gShaderCode.c_str()));
        glAttachShader(this->programID, geometry);
    }

    glAttachShader(this->programID, vertex);
    glAttachShader(this->programID, fragment);

    glLinkProgram(this->programID);

#ifdef DEBUG
    GLint success;
    glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(this->programID, 512, NULL, infoLog);
        DBG_LOG("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
        DBG_LOG(infoLog);
        DBG_LOG("\n");

        if (fShaderCode.empty() || vShaderCode.empty()) {
            DBG_LOG("EMPTY SHADER (Shader.cpp ShaderBase::createProgram())\n");
        }
    }
#endif

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDetachShader(this->programID, vertex);
    glDetachShader(this->programID, fragment);

    if (!gShaderCode.empty()) {
        glDeleteShader(geometry);
        glDetachShader(this->programID, geometry);
    }
}

void Shader::useProgram() {
    static GLint lastUsed = -1;

    if (lastUsed != getProgramID()) {
        glUseProgram(getProgramID());
        lastUsed = getProgramID();
    }
}

void Shader::supplyVec3fUniform(const std::string& location, const glm::vec3& value) {
    glUniform3f(
        glGetUniformLocation(
            this->programID,
            location.c_str()),
        value.x,
        value.y,
        value.z);
}

void Shader::supply1fUniform(const std::string& location, const float& value) {
    glUniform1f(
        glGetUniformLocation(
            this->programID,
            location.c_str()),
        value);
}
