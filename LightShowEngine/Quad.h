#ifndef QUAD_H
#define QUAD_H
#include "GameInfo.h"
#include "ShaderBase.h"
#include "Shaders.h"
#include "Texture.h"
#include "Transform.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
class Quad {
public:
    void RenderQuad(
        const ShaderBase& shader,
        const Texture& texture,
        const glm::vec4& spriteOrientationInPixels,
        const glm::vec2& positionInPixels,
        const glm::vec2& scaleInPixels);

private:
    GLint positionAttribLocation = 0;
    GLint textureCoordsAttribLocation = 0;

    GLuint quadVAO = 0;
    GLuint quadVBO = 0;
};
#endif
