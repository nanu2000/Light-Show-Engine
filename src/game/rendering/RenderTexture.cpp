#include "RenderTexture.h"
void RenderTextureBase::initialize(unsigned int w, unsigned int h) {

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

    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        DBG_LOG("Framebuffer is not complete! %i\n", status);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    initialized = true;
}

void RenderTextureBase::resize(unsigned int w, unsigned int h) {
    freeGLIds();
    initialize(w, h);
}

RenderTextureBase::~RenderTextureBase() {
    DBG_LOG("Freeing memory for render texture.\n");
    freeGLIds();
}

void RenderTextureBase::freeGLIds() {
    if (initialized) {
        glDeleteFramebuffers(1, &textureFBO);
        glDeleteTextures(1, &textureID);
        glDeleteRenderbuffers(1, &textureRBO);
        textureFBO  = 0;
        textureID   = 0;
        textureRBO  = 0;
        initialized = false;
    }
}

//---RenderTextureMS

int RenderTextureMS::getMaxMultisample() {
    static int samples = -1;

    //We found the max amount of samples already!
    if (samples > -1) {
        return samples;
    }

    DBG_LOG("Getting max multisample count.\n");

    samples = 16;
    RenderTextureMS ms;
    ms.initializeUnsafe(1, 1, samples);
    while (ms.status != GL_FRAMEBUFFER_COMPLETE && samples != 1) {
        samples /= 2;
        ms.freeGLIds(); //resize will cause recursion since it uses initialize- so we avoid it.
        ms.initializeUnsafe(1, 1, samples);
    }

    DBG_LOG("Max multisample count is %i\n", samples);
    return samples;
}

void RenderTextureMS::resize(unsigned int w, unsigned int h) {
    freeGLIds();

    //If currentsamples is not set, use max samples.
    initialize(w, h, currentMultisamples == -1 ? RenderTextureMS::getMaxMultisample() : currentMultisamples);
}

void RenderTextureMS::resize(unsigned int w, unsigned int h, unsigned int samples) {
    freeGLIds();
    initialize(w, h, samples);
}

void RenderTextureMS::initialize(unsigned int w, unsigned int h) {
    initialize(w, h, RenderTextureMS::getMaxMultisample());
}

void RenderTextureMS::initialize(unsigned int w, unsigned int h, unsigned int samples) {

    if (samples > RenderTextureMS::getMaxMultisample()) {
        DBG_LOG("RenderTextureMS::initialize: Samples need to be less than max ammount! using getMaxMultisample value.\n");
        samples = RenderTextureMS::getMaxMultisample();
    }

    initializeUnsafe(w, h, samples);
}

void RenderTextureMS::initializeUnsafe(unsigned int w, unsigned int h, unsigned int samples) {

    width  = w;
    height = h;

    glGenFramebuffers(1, &textureFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, textureFBO);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureID);
    glTexImage2DMultisample(
        GL_TEXTURE_2D_MULTISAMPLE,
        samples,
        GL_RGB,
        width, height,
        GL_TRUE);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureID, 0);
    glGenRenderbuffers(1, &textureRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, textureRBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, textureRBO);

    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        DBG_LOG("Framebuffer is not complete! %i\n", status);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    currentMultisamples = samples;
    initialized         = true;
}
