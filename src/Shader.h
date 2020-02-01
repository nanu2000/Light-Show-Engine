#ifndef LIT_SHADER_H
#define LIT_SHADER_H
#include "Component.h"
#include "Debug.h"
#include "Lights.h"
#include "Material.h"
#include "Settings.h"
#include "Shaders.h"
#include <GL/glew.h> // Include glew to get all the required OpenGL headers
#include <algorithm>
#include <fstream> //Used for file streams
#include <glm/vec3.hpp>
#include <sstream> //used for string streams
#include <string> //Included for the std::string object
#include <vector>
using Shaders::SHADER_TASK;
using Shaders::SHADER_TYPE;

class Shader : public Component<Shader> {

public:
    Shader() {}

    ~Shader() {
    }

    static SHADER_TASK getShaderTask();

    static void setShaderTaskShader(SHADER_TASK task, const GLint& id) { shadersForTasks[static_cast<unsigned int>(task)] = id; }
    static void setShaderTask(SHADER_TASK task) { currentTask = task; }

    SHADER_TYPE getShaderType() { return shaderType; }
    void setShaderType(SHADER_TYPE& type) { shaderType = type; }

    // Constructor reads and builds the shader
    Shader(
        const std::string& id,
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

    Shader(
        const std::string& id,
        const Settings& currentSettings,
        const std::string& vertexPath,
        const std::string& fragmentPath,
        const SHADER_TYPE& type,
        const std::string& geometryPath = "");

    void recompileShader(const Settings& currentSettings);

    //Must use shader before calling!!!!
    void setPointLight(const Settings& currentSettings, const PointLight& light, unsigned int index);

    //Must use shader before calling!!!!
    void setDirectionalLight(const DirectionalLight& directionalLight);

    //Must use shader before calling!!!!
    void setMaterial(const Material& material);

    //Must use shader before calling!!!!
    void setSimpleMaterial(const SimpleMaterial& material);

    //Must use shader before calling!!!!
    void setShininess(const float& shininess);

    std::string getIdentifier() const { return identifier; }

private:
    void updateTagValues(const Settings& currentSettings, std::string& fragmentCode);

    void updateTagValue(const std::string& tag, const std::string& toAdd, std::string& stringToModify) {
        std::string strtString = stringToModify.substr(stringToModify.find(tag));
        std::string endString  = strtString.substr(0, strtString.find(Shaders::TAG_END));
        stringToModify.replace(stringToModify.find(endString), endString.length(), tag + toAdd);
    }

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

    std::string identifier = "";
};

class ShaderHandler {

public:
    Shader& getShader(const std::string& id,
                      const Settings& currentSettings,
                      const std::string& vertexPath,
                      const std::string& fragmentPath,
                      const SHADER_TYPE& type,
                      const std::string& geometryPath = "") {

        Shader* shader = binarySearchShaders(id);

        if (shader != nullptr) {
            DBG_LOG("Shader %s found.\n", id.c_str());
            return *shader;
        }

        return addNewShader(id, currentSettings, vertexPath, fragmentPath, type, geometryPath);
    }
    Shader& getShader(const std::string& id,
                      const std::string& vertexPath,
                      const std::string& fragmentPath,
                      const SHADER_TYPE& type,
                      const std::string& geometryPath = "") {

        Shader* shader = binarySearchShaders(id);

        if (shader != nullptr) {
            DBG_LOG("Shader %s found.\n", id.c_str());
            return *shader;
        }

        return addNewShader(id, vertexPath, fragmentPath, type, geometryPath);
    }

    ~ShaderHandler() {

        DBG_LOG("Freeing memory for shaders.\n");
        for (std::vector<Shader*>::iterator it = shaderLibrary.begin(); it != shaderLibrary.end(); ++it) {
            if (*it != nullptr) {
                delete *it;
                *it = nullptr;
            }
        }
    }

private:
    Shader* binarySearchShaders(const std::string& key) {

        unsigned int mid   = 0;
        unsigned int left  = 0;
        unsigned int right = shaderLibrary.size();

        while (left < right) {
            mid = left + (right - left) / 2;

            if (key > shaderLibrary.at(mid)->getIdentifier()) {
                left = mid + 1;
            } else if (key < shaderLibrary.at(mid)->getIdentifier()) {
                right = mid;
            } else {
                return shaderLibrary.at(mid);
            }
        }

        return nullptr;
    }
    Shader& addNewShader(const std::string& id,
                         const std::string& vertexPath,
                         const std::string& fragmentPath,
                         const SHADER_TYPE& type,
                         const std::string& geometryPath) {

        Shader* shader = new Shader(id, vertexPath, fragmentPath, type, geometryPath);

        // Add shader to library
        shaderLibrary.push_back(shader);

        // sort the library so I can use a binary search.
        std::sort(shaderLibrary.begin(),
                  shaderLibrary.end(),
                  [](Shader* a, Shader* b) // :)
                  { return a->getIdentifier() < b->getIdentifier(); });

        return *shader;
    }
    Shader& addNewShader(
        const std::string& id,
        const Settings& currentSettings,
        const std::string& vertexPath,
        const std::string& fragmentPath,
        const SHADER_TYPE& type,
        const std::string& geometryPath) {

        Shader* shader = new Shader(id, currentSettings, vertexPath, fragmentPath, type, geometryPath);

        // Add shader to library
        shaderLibrary.push_back(shader);

        // sort the library so I can use a binary search.
        std::sort(shaderLibrary.begin(),
                  shaderLibrary.end(),
                  [](Shader* a, Shader* b) // :)
                  { return a->getIdentifier() < b->getIdentifier(); });

        return *shader;
    }

    std::vector<Shader*> shaderLibrary;
};

#endif
