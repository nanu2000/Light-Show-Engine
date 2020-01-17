#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <GL/glew.h>
#include <SDL_image.h>
#include <SDL_opengl.h>

#include <algorithm>
#include <map>
#include <string>

#include "Debug.h"
#include "HelpingHand.h"

class Texture {
public:
    Texture();

    ~Texture() {
        DBG_LOG("Freeing memory for texture.\n");
        glDeleteTextures(1, &texture);
    }

    Texture(std::string loc, GLuint txture, GLuint w, GLuint h,
            bool istransparent);

    inline bool checkTransparency() const { return isTransparent; }
    inline GLuint getWidth() const { return imageWidth; }
    inline GLuint getHeight() const { return imageHeight; }
    inline GLuint getTextureData() const { return texture; }
    inline std::string getLocation() const { return location; }

private:
    GLuint imageWidth  = 0;
    GLuint imageHeight = 0;
    GLuint texture     = 0;
    bool isTransparent = false;
    std::string location;
};

class CubeMap {
public:
    CubeMap();
    ~CubeMap() {
        DBG_LOG("Freeing memory for cubemap.\n");
        glDeleteTextures(1, &texture);
    }
    CubeMap(const std::vector<std::string>& faces, GLuint txture);

    inline GLuint getCubeMapData() const { return texture; }
    const std::vector<std::string> getLocations() const {
        return location;
    }

private:
    std::vector<std::string> location;
    GLuint texture = 0;
};

class TextureHandler {
public:
    // Will return Texture if it already exists, but will create one if it doesn't
    // exist.
    Texture& getTexture(std::string filePath, GLint filtering = GL_LINEAR, bool repeatTexture = false);

    CubeMap& getCubeMap(const std::string identifier, const std::vector<std::string>& faces);

    ~TextureHandler();

private:
    Texture& createTexture(std::string filePath, GLint filtering,
                           bool repeatTexture);

    Texture* binarySearchTextures(const std::string& key);
    Texture& addNew(std::string filePath, GLint filtering, bool repeatTexture);
    void addThenSort(Texture& texture);
    std::vector<Texture*> textureLibrary;
    std::map<std::string, CubeMap*> cubeMaps;
};

#endif
