#ifndef SHADER_BASE_H
#define SHADER_BASE_H

#include "Component.h"
#include "Debug.h"
#include "Shaders.h"
#include <GL/glew.h> // Include glew to get all the required OpenGL headers
#include <fstream> //Used for file streams
#include <glm/vec3.hpp>
#include <sstream> //used for string streams
#include <string> //Included for the std::string object
#include <vector>

using Shaders::SHADER_TASK;
using Shaders::SHADER_TYPE;

class ShaderBase {

public:
    ShaderBase() {}
    virtual ~ShaderBase() {}

    static SHADER_TASK getShaderTask();

    static void setShaderTaskShader(SHADER_TASK task, const GLint& id) { shadersForTasks[static_cast<unsigned int>(task)] = id; }
    static void setShaderTask(SHADER_TASK task) { currentTask = task; }

    SHADER_TYPE getShaderType() { return shaderType; }
    void setShaderType(SHADER_TYPE& type) { shaderType = type; }

    // Constructor reads and builds the shader
    ShaderBase(
        const std::string& vertexPath,
        const std::string& fragmentPath,
        const SHADER_TYPE& type,
        const std::string& geometryPath = "");

    GLint getProgramID() const {
        return currentTask == SHADER_TASK::Normal_Render_Task ? programID : shadersForTasks[static_cast<unsigned int>(currentTask)];
    }

    void useProgram();

    void supplyVec3fUniform(const std::string& location, const glm::vec3& value);

    void supply1fUniform(const std::string& location, const float& value);

    void recompileShader();

protected:
    static GLint shadersForTasks[static_cast<unsigned int>(SHADER_TASK::SHADER_TASK_MAX)];
    static SHADER_TASK currentTask;

    GLint createAndCompileShader(int GLShaderType, const GLchar* const* code);

    int getCode(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath, std::string& vertexCode, std::string& fragmentCode, std::string& geometryCode);
    void createProgram(const std::string& vShaderCode, const std::string& fShaderCode, const std::string& gShaderCode);

    GLint programID = -1;

    std::string vertexFilePath;
    std::string fragmentFilePath;
    std::string geometryFilePath;

    SHADER_TYPE shaderType = SHADER_TYPE::Default;
};

#endif
