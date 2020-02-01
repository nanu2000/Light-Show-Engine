#ifndef QUAD_H
#define QUAD_H
#include "GameInfo.h"
#include "Shader.h"
#include "Shaders.h"
#include "Texture.h"
#include "Transform.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
class Quad {
public:
    void render3D(
        const Shader& shader,
        const Texture& texture,
        const glm::vec4& spriteOrientationInPixels,
        const glm::vec2& positionInPixels,
        const glm::vec2& scaleInPixels);

    void render3D(const Shader& shader, GLint textureID, glm::mat4& modelMatrix);

    void render2D(const Shader& shader, GLint textureID);

    ~Quad();

private:
    void init();

    bool hasInit                      = false;
    GLint positionAttribLocation      = 0;
    GLint textureCoordsAttribLocation = 0;

    GLuint quadVAO                    = 0;
    GLuint quadVBO                    = 0;
    std::vector<GLfloat> quadVertices = {
        //top left
        -1.0f, 1.0f,

        0, 1,

        //bottom left
        -1.0f, -1.0f,

        0, 0,

        // top right
        1.0f, 1.0f,

        1, 1,

        // bottom right
        1.0f, -1.0f,

        1, 0
    };
};
#endif
