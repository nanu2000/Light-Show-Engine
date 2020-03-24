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
    //!On destruction the txture will be deleted using glDeleteTextures
    Texture(std::string loc, GLuint txture, GLuint w, GLuint h, bool istransparent);
    ~Texture();

    //!Fixes issues with opengl deleteTexture.
    //!https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls
    Texture(const Texture&) = delete;
    Texture(Texture&&)      = delete;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

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
    //!On destruction the txture will be deleted using glDeleteTextures
    CubeMap(const std::vector<std::string>& faces, GLuint txture);
    ~CubeMap();

    CubeMap(const CubeMap&)  = delete;
    CubeMap(CubeMap&& other) = delete;
    CubeMap& operator=(const CubeMap&) = delete;
    CubeMap& operator=(CubeMap&& other) = delete;

    inline GLuint getCubeMapData() const { return texture; }
    inline const std::vector<std::string> getTexturePaths() const {
        return location;
    }

private:
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
