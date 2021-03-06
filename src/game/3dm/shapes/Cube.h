#ifndef CUBE_SHAPE
#define CUBE_SHAPE

#include "Shader.h"
#include "Shaders.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/vec3.hpp"
#include <vector>

class CubeShape {
public:
    void create() {

        glGenVertexArrays(1, &VAOID);
        glGenBuffers(1, &vertID);

        glBindVertexArray(VAOID);
        glBindBuffer(GL_ARRAY_BUFFER, vertID);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(Shaders::getAttribLocation(Shaders::AttribName::Position));
        glVertexAttribPointer(Shaders::getAttribLocation(Shaders::AttribName::Position), 3, GL_FLOAT, GL_FALSE, 0, 0);
        initialized = true;
    }

    void render(Shader& shader, Camera& camera) {
        GLuint transformLoc = Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::ModelMatrix);

        glm::mat4 view = glm::mat4(glm::mat3(*camera.getViewMatrix()));

        glBindVertexArray(VAOID);

        glUniformMatrix4fv(
            Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::ViewMatrix),
            1,
            GL_FALSE,
            glm::value_ptr(view));

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4()));

        glBindBuffer(GL_ARRAY_BUFFER, vertID);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glBindVertexArray(0);
    }

    ~CubeShape() {

        if (!initialized) {
            return;
        }

        DBG_LOG("Freeing memory for cube.\n");

        glDeleteVertexArrays(1, &VAOID);
        glDeleteBuffers(1, &vertID);
    }

private:
    std::vector<GLfloat> vertices = {
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
    };
    GLuint VAOID;
    GLuint vertID;
    bool initialized = false;
};

#endif
