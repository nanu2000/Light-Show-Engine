#include "Quad.h"

void Quad::init() {

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glEnableVertexAttribArray(Shaders::getAttribLocation(Shaders::AttribName::Position));
    glEnableVertexAttribArray(Shaders::getAttribLocation(Shaders::AttribName::TextureCoordinates));
    glBindVertexArray(0);

    hasInit = true;
}

//Renders a textured 3D quad. Used for gui.
void Quad::render3D(
    const Shader& shader,
    const Texture& texture,
    const glm::vec4& spriteOrientationInPixels,
    const glm::vec2& positionInPixels,
    const glm::vec2& scaleInPixels) {

    if (!hasInit) {
        init();
    }

    float widthCalculated     = spriteOrientationInPixels.z / texture.getWidth();
    float heightCalculated    = spriteOrientationInPixels.w / texture.getHeight();
    float xPositionCalculated = spriteOrientationInPixels.x / texture.getWidth();
    float yPositionCalculated = spriteOrientationInPixels.y / texture.getHeight();

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    int ww = GameInfo::getWindowWidth();
    int wh = GameInfo::getWindowHeight();

    glm::vec3 tv = glm::vec3(
        (positionInPixels.x) / GameInfo::getWindowWidth(),
        (positionInPixels.y) / GameInfo::getWindowHeight(),
        0.0f);

    modelMatrix = glm::translate(modelMatrix, tv);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleInPixels.x / GameInfo::getWindowWidth(), scaleInPixels.y / GameInfo::getWindowHeight(), 1.0f));

    if (quadVertices.size() == 16) {
        quadVertices.resize(20);
    }

    quadVertices = {
        //top left
        -1.0f, 1.0f, -1.0f,

        xPositionCalculated, heightCalculated + yPositionCalculated,

        -1.0f, -1.0f, -1.0f,

        //bottom left
        xPositionCalculated, yPositionCalculated,

        1.0f, 1.0f, -1.0f,

        // top right
        widthCalculated + xPositionCalculated, heightCalculated + yPositionCalculated,

        // bottom right
        1.0f, -1.0f, -1.0f,

        widthCalculated + xPositionCalculated, yPositionCalculated
    };

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(GLfloat), &quadVertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(Shaders::getAttribLocation(Shaders::AttribName::Position), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(Shaders::getAttribLocation(Shaders::AttribName::TextureCoordinates), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.getTextureData());
    glUniform1i(Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::DiffuseTexture), 0);

    glUniformMatrix4fv(
        Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::ModelMatrix),
        1,
        false,
        glm::value_ptr(modelMatrix));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

//Renders a 3D textured quad
void Quad::render3D(const Shader& shader, GLint textureID, glm::mat4& modelMatrix) {

    if (!hasInit) {
        init();

        quadVertices = {
            //top left
            -1.0f, 1.0f, -1.0f,

            0, 1,

            -1.0f, -1.0f, -1.0f,

            //bottom left
            0, 0,

            1.0f, 1.0f, -1.0f,

            // top right
            1, 1,

            // bottom right
            1.0f, -1.0f, -1.0f,

            1, 0
        };
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::DiffuseTexture), 0);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(GLfloat), &quadVertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(Shaders::getAttribLocation(Shaders::AttribName::Position), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(Shaders::getAttribLocation(Shaders::AttribName::TextureCoordinates), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    glUniformMatrix4fv(
        Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::ModelMatrix),
        1,
        false,
        glm::value_ptr(modelMatrix));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

//Renders a 2D textured quad
void Quad::render2D(const Shader& shader, GLint textureID) {

    if (!hasInit) {
        init();
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::DiffuseTexture), 0);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(GLfloat), &quadVertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(Shaders::getAttribLocation(Shaders::AttribName::Position), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(Shaders::getAttribLocation(Shaders::AttribName::TextureCoordinates), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

Quad::~Quad() {
    if (!hasInit) {
        return;
    }

    DBG_LOG("Freeing memory for Quad.\n");

    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
}
