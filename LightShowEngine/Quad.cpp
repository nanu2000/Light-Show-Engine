#include "Quad.h"

void Quad::RenderQuad(
    const ShaderBase& shader,
    const Texture& texture,
    const glm::vec4& spriteOrientationInPixels,
    const glm::vec2& positionInPixels,
    const glm::vec2& scaleInPixels) {

    float widthCalculated = spriteOrientationInPixels.z / texture.getWidth();

    float heightCalculated = spriteOrientationInPixels.w / texture.getHeight();

    float xPositionCalculated = spriteOrientationInPixels.x / texture.getWidth();

    float yPositionCalculated = spriteOrientationInPixels.y / texture.getHeight();

    glm::mat4 modelMatrix;
    modelMatrix = glm::translate(
        modelMatrix,
        glm::vec3(
            (positionInPixels.x) / GameInfo::getWindowWidth(),
            (positionInPixels.y) / GameInfo::getWindowHeight(),
            0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleInPixels.x / GameInfo::getWindowWidth(), scaleInPixels.y / GameInfo::getWindowHeight(), 1.0f));

    GLfloat quadVertices[20] = {
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

    if (quadVAO == 0) {
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        positionAttribLocation      = Shaders::getAttribLocation(Shaders::AttribName::Position);
        textureCoordsAttribLocation = Shaders::getAttribLocation(Shaders::AttribName::TextureCoordinates);
    }

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(positionAttribLocation);
    glEnableVertexAttribArray(textureCoordsAttribLocation);

    glVertexAttribPointer(positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(textureCoordsAttribLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

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
