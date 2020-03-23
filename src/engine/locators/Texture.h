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
    Texture(std::string loc, GLuint txture, GLuint w, GLuint h, bool istransparent);

    ~Texture();

    //!Fixes issues with opengl deletetexture.
    //!https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept
        : Texture() {
        swap(*this, other);
        other.texture = 0; //Use the "null" texture for the old object.
    }

    Texture& operator=(Texture&& other) noexcept {
        //ALWAYS check for self-assignment.
        if (this != &other) {
            freeTexture();
            swap(*this, other);
        }
    }

    inline bool checkTransparency() const { return isTransparent; }
    inline GLuint getWidth() const { return imageWidth; }
    inline GLuint getHeight() const { return imageHeight; }
    inline GLuint getTextureData() const { return texture; }
    inline std::string getLocation() const { return location; }

private:
    Texture() {}
    friend void swap(Texture& first, Texture& second) {
        std::swap(first.imageWidth, second.imageWidth);
        std::swap(first.imageHeight, second.imageHeight);
        std::swap(first.texture, second.texture);
        std::swap(first.isTransparent, second.isTransparent);
        std::swap(first.location, second.location);
    }

    void freeTexture() {
        DBG_LOG("Freeing memory for texture.\n");
        glDeleteTextures(1, &texture);
        texture = 0;
    }

    GLuint imageWidth  = 0;
    GLuint imageHeight = 0;
    GLuint texture     = 0;
    bool isTransparent = false;
    std::string location;
};

class CubeMap {
public:
    CubeMap(const std::vector<std::string>& faces, GLuint txture);
    ~CubeMap();

    CubeMap(const CubeMap&) = delete;
    CubeMap& operator=(const CubeMap&) = delete;

    CubeMap(CubeMap&& other)
        : CubeMap() {
        swap(*this, other);
        other.texture = 0; //Use the "null" texture for the old object.
    }

    CubeMap& operator=(CubeMap&& other) {
        //ALWAYS check for self-assignment.
        if (this != &other) {
            freeTextures();
            swap(*this, other);
        }
    }

    inline GLuint getCubeMapData() const { return texture; }
    inline const std::vector<std::string> getTexturePaths() const {
        return location;
    }

private:
    CubeMap() {}
    friend void swap(CubeMap& first, CubeMap& second) {
        std::swap(first.texture, second.texture);
        std::swap(first.location, second.location);
    }

    void freeTextures() {
        DBG_LOG("Freeing memory for cubemap.\n");
        glDeleteTextures(1, &texture);
        texture = 0;
    }

    std::vector<std::string> location;
    GLuint texture = 0;
};

//The TextureHandler class should be used for handling textures and cubemaps throughout the programs duration.
//It should be handled by the TextureLocator class.
class TextureHandler {
public:
    //Tries to retrieve a texture via file path, if it's not in the texture library then it adds it.
    Texture& getTexture(std::string filePath, GLint filtering = GL_LINEAR, bool repeatTexture = false);

    //Tries to retrieve a cubemap via file paths, if it's not in the cubemap library then it adds it.
    CubeMap& getCubeMap(const std::string identifier, const std::vector<std::string>& faces);

    //Free memory allocated for cubemaps and textures.
    //All allocated pointers are stored in textureLibrary and cubeMapLibrary
    ~TextureHandler();

private:
    //Fills Texture with data from filePath if it exists. if not it will use a checker pattern.
    Texture* parseTexture(std::string filePath, GLint filtering, bool repeatTexture);

    //Checks if the sdl surface is power of two. Only runs on debug
    void verifySurfaceDimensions(const SDL_Surface& surface);

    //Returns sdl_surface format. IE GL_RGB, GL_RGBA, etc...
    int getSurfaceFormat(const SDL_Surface& surface);

    //Generates filler image using glTexImage2D with fillerTexturePixels to create a texture to use.
    void teximage2DFillerTexture(GLenum target);

    //Add a new texture to the sorted texture library.
    Texture& addNewTexture(std::string filePath, GLint filtering, bool repeatTexture);

    Texture* binarySearchTextures(const std::string& key);

    //Used as data for filepaths which could not find a valid texture to use.
    //These bytes are a simple checkered pattern.
    GLubyte fillerTexturePixels[27] = { 0, 0, 0, 255, 255, 255, 0, 0, 0,

                                        255, 255, 255, 0, 0, 0, 255, 255, 255,

                                        0, 0, 0, 255, 255, 255, 0, 0, 0 };

    //Contains all dynamically allocated pointers for textures
    std::vector<Texture*> textureLibrary;

    //Contains all dynamically allocated pointers for cubemaps
    std::map<std::string, CubeMap*> cubeMapLibrary;
};

#endif
