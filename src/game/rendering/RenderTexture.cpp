#include "RenderTexture.h"
void RenderTexture::initialize(unsigned int w, unsigned int h) {

    width  = w;
    height = h;

    glGenFramebuffers(1, &textureFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, textureFBO);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
    glGenRenderbuffers(1, &textureRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, textureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, textureRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        DBG_LOG("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    initialized = true;
}

void RenderTexture::resize(unsigned int w, unsigned int h) {

    glDeleteFramebuffers(1, &textureFBO);
    glDeleteTextures(1, &textureID);
    glDeleteRenderbuffers(1, &textureRBO);
    initialize(w, h);
}

RenderTexture::~RenderTexture() {
    if (!initialized) {
        return;
    }

    DBG_LOG("Freeing memory for render texture.\n");

    glDeleteFramebuffers(1, &textureFBO);
    glDeleteTextures(1, &textureID);
    glDeleteRenderbuffers(1, &textureRBO);
}
