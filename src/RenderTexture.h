#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H
#include "GameInfo.h"
#include "HelpingHand.h"
#include "ShaderBase.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RenderTexture {

public:
    inline GLint getTextureID() { return textureID; }
    inline GLuint getFBO() { return textureFBO; }

    inline unsigned int getWidth() const { return width; }
    inline unsigned int getHeight() const { return height; }

    void initialize(unsigned int w, unsigned int h);
    void resize(unsigned int w, unsigned int h);

private:
    unsigned int width  = 0;
    unsigned int height = 0;

    GLuint textureFBO;
    GLuint textureID;
    GLuint textureRBO;
};

#endif
