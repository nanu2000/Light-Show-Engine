#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H
#include "GameInfo.h"
#include "HelpingHand.h"
#include "Shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//!The RenderTextureBase class is used to create a texture that you can render to. This is **not** a component
class RenderTextureBase {

public:
    //!Keep in mind that the destructor will call glDelete on openGL objects generated if properly initialized.
    RenderTextureBase() {}

    RenderTextureBase& operator=(const RenderTextureBase&) = delete;
    RenderTextureBase& operator=(RenderTextureBase&&) = delete;
    RenderTextureBase(const RenderTextureBase&)       = delete;
    RenderTextureBase(RenderTextureBase&&)            = delete;

    inline GLint getTextureID() const { return textureID; }
    inline GLuint getFBO() const { return textureFBO; }

    inline unsigned int getWidth() const { return width; }
    inline unsigned int getHeight() const { return height; }

    //!Initializes the texture
    virtual void initialize(unsigned int w, unsigned int h);

    //!Resizes the texture
    virtual void resize(unsigned int w, unsigned int h);

    virtual ~RenderTextureBase();

protected:
    //!Frees openGL id's
    void freeGLIds();

    unsigned int width  = 0;
    unsigned int height = 0;

    GLuint textureFBO = 0;
    GLuint textureID  = 0;
    GLuint textureRBO = 0;

    GLenum status    = 0;
    bool initialized = false;
};

//!The RenderTexture class is used to create a texture that you can render to. This is a component.
class RenderTexture : public RenderTextureBase, public Component<RenderTexture> {
    virtual ~RenderTexture() {}

    RenderTexture& operator=(const RenderTexture&) = delete;
    RenderTexture& operator=(RenderTexture&&) = delete;
    RenderTexture(const RenderTexture&)       = delete;
    RenderTexture(RenderTexture&&)            = delete;
};

//!The RenderTextureMS class is used to create a multisampled texture that you can render to. This is a component.
class RenderTextureMS : public RenderTextureBase, public Component<RenderTextureMS> {

public:
    virtual ~RenderTextureMS() {}

    //!Keep in mind that the destructor will call glDelete on openGL objects generated if properly initialized.
    RenderTextureMS() {}

    RenderTextureMS& operator=(const RenderTextureMS&) = delete;
    RenderTextureMS& operator=(RenderTextureMS&&) = delete;
    RenderTextureMS(const RenderTextureMS&)       = delete;
    RenderTextureMS(RenderTextureMS&&)            = delete;

    //!Retrieves the max amount of multisamples the computer can handle if it hasn't already.
    static int getMaxMultisample();

    //!Resize the texture
    void resize(unsigned int w, unsigned int h);

    //!Resize the texture and set the multisamples
    void resize(unsigned int w, unsigned int h, unsigned int samples);

    //!Initialize
    void initialize(unsigned int w, unsigned int h);

    void initialize(unsigned int w, unsigned int h, unsigned int samples);

    inline int getCurrentMultisampleCount() const {
        return currentMultisamples > 0 ? currentMultisamples : 1;
    }

private:
    //!used for resizing.
    int currentMultisamples = -1;

    //!Doesn't check if samples are supported. (prevents recursion in getMaxMultisample()).
    void initializeUnsafe(unsigned int w, unsigned int h, unsigned int samples);
};

#endif
