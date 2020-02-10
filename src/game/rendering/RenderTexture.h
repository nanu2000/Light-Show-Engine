#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H
#include "GameInfo.h"
#include "HelpingHand.h"
#include "Shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//!The RenderTexture class is used to create a texture that you can render to.
class RenderTexture {

public:
    inline GLint getTextureID() const { return textureID; }
    inline GLuint getFBO() const { return textureFBO; }

    inline unsigned int getWidth() const { return width; }
    inline unsigned int getHeight() const { return height; }

    //!Initializes the texture
    virtual void initialize(unsigned int w, unsigned int h);

    //!Resizes the texture
    virtual void resize(unsigned int w, unsigned int h);

    virtual ~RenderTexture();

protected:
    //!Frees openGL id's
    void freeGLIds();

    unsigned int width  = 0;
    unsigned int height = 0;

    GLuint textureFBO = 0;
    GLuint textureID  = 0;
    GLuint textureRBO = 0;

    GLenum status;
    bool initialized = false;
};

//!The RenderTextureMS class is used to create a multisampled texture that you can render to.
class RenderTextureMS : public RenderTexture {

public:
    virtual ~RenderTextureMS() {}

    //!Retrieves the max amount of multisamples the computer can handle if it hasn't already.
    static int getMaxMultisample();

    //!Resize the texture
    void resize(unsigned int w, unsigned int h);

    //!Resize the texture and set the multisamples
    void resize(unsigned int w, unsigned int h, unsigned int samples);

    //!Initialize
    void initialize(unsigned int w, unsigned int h);

    void initialize(unsigned int w, unsigned int h, unsigned int samples);

private:
    //!used for resizing.
    int currentMultisamples = -1;

    //!Doesn't check if samples are supported. (prevents recursion in getMaxMultisample()).
    void initializeUnsafe(unsigned int w, unsigned int h, unsigned int samples);
};

#endif
