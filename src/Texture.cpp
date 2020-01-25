#include "Texture.h"

Texture::Texture(std::string loc, GLuint txture, GLuint w, GLuint h, bool istransparent) {
    location      = loc;
    texture       = txture;
    imageHeight   = h;
    imageWidth    = w;
    isTransparent = istransparent;
}
Texture::~Texture() {
    DBG_LOG("Freeing memory for texture.\n");
    glDeleteTextures(1, &texture);
}

CubeMap::CubeMap(const std::vector<std::string>& faces, GLuint txture) {
    location = faces;
    texture  = txture;
}
CubeMap::~CubeMap() {
    DBG_LOG("Freeing memory for cubemap.\n");
    glDeleteTextures(1, &texture);
}

//Fills Texture with data from filePath if it exists. if not it will use a checker pattern.
Texture TextureHandler::parseTexture(std::string filePath, GLint filtering, bool repeatTexture) {

    GLuint textureHandle;
    SDL_Surface* surface;

    if ((surface = IMG_Load(filePath.c_str()))) {

        verifySurfaceDimensions(*surface);
        GLenum textureFormat = getSurfaceFormat(*surface);

        glGenTextures(1, &textureHandle);
        glBindTexture(GL_TEXTURE_2D, textureHandle);

        if (repeatTexture) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     textureFormat,
                     surface->w,
                     surface->h,
                     0,
                     textureFormat,
                     GL_UNSIGNED_BYTE,
                     surface->pixels);

        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        SDL_FreeSurface(surface);

        return Texture(filePath, textureHandle, surface->w, surface->h, (textureFormat == GL_RGBA));
    }

    DBG_LOG("Image could not load properly, using null texture\n");
    DBG_LOG("The location of the non functioning texture is ");
    DBG_LOG(filePath.c_str());
    DBG_LOG("\n");

    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_2D, textureHandle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenerateMipmap(GL_TEXTURE_2D);

    teximage2DFillerTexture(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return Texture(filePath, textureHandle, 3, 3, false);
}

//Checks if the sdl surface is power of two. Only runs on debug
void TextureHandler::verifySurfaceDimensions(const SDL_Surface& surface) {

#ifdef DEBUG
    if (surface.w >= 4096) {
        DBG_LOG("Image is %d pixels in width, think about scaling down to save "
                "memory\n",
                surface.w);
    }
    if (surface.h >= 4096) {
        DBG_LOG("Image is %d pixels in height, think about scaling down to save "
                "memory\n",
                surface.h);
    }

    if (!hh::isPowerOfTwo(surface.h)) {
        DBG_LOG("The image height (%d) is either bigger than 8192 or not power of "
                "two. Please think about changing dimensions of the image\n",
                surface.h);
    }
    if (!hh::isPowerOfTwo(surface.w)) {
        DBG_LOG("The image width (%d) is either bigger than 8192 or not power of "
                "two. Please think about changing dimensions of the image\n",
                surface.w);
    }
#endif
}

//Returns sdl_surface format. IE GL_RGB, GL_RGBA, etc...
GLenum TextureHandler::getSurfaceFormat(const SDL_Surface& surface) {

    GLint textureFormat = GL_RGBA;
    // get number of bytes per pixel (for colors)
    int numberOfColors = surface.format->BytesPerPixel;

    // contains an alpha channel
    if (numberOfColors == 4) {

        if (surface.format->Rmask == 0x000000ff) {
            return GL_RGBA;
        }

        return GL_BGRA;

    } else if (numberOfColors == 3) { // no alpha channel

        if (surface.format->Rmask == 0x000000ff) {
            return GL_RGB;
        }

        return GL_BGR;
    }

    DBG_LOG("Image is not truecolor\n");
    return GL_RGB;
}

//Generates filler image using glTexImage2D with fillerTexturePixels to create a texture to use.
void TextureHandler::teximage2DFillerTexture(GLenum target) {

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(target, 0, GL_RGB, 3, 3, 0, GL_RGB, GL_UNSIGNED_BYTE, fillerTexturePixels);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

//Add a new texture to the sorted texture library.
Texture& TextureHandler::addNewTexture(std::string filePath, GLint filtering, bool repeatTexture) {
    Texture* texture = new Texture(parseTexture(filePath, filtering, repeatTexture));

    // Add texture to library
    textureLibrary.push_back(texture);

    // sort the library so I can use a binary search.
    std::sort(textureLibrary.begin(),
              textureLibrary.end(),
              [](Texture* a, Texture* b) // :)
              { return a->getLocation() < b->getLocation(); });

    return *texture;
}

//Searches the texture library for a texture.
Texture* TextureHandler::binarySearchTextures(const std::string& key) {
    unsigned int mid   = 0;
    unsigned int left  = 0;
    unsigned int right = textureLibrary.size();

    while (left < right) {
        mid = left + (right - left) / 2;

        if (key > textureLibrary.at(mid)->getLocation()) {
            left = mid + 1;
        } else if (key < textureLibrary.at(mid)->getLocation()) {
            right = mid;
        } else {
            return textureLibrary.at(mid);
        }
    }

    return nullptr;
}

//Tries to retrieve a texture via file path, if it's not in the texture library then it adds it.
Texture& TextureHandler::getTexture(std::string filePath, GLint filtering, bool repeatTexture) {

    Texture* texture = binarySearchTextures(filePath);

    if (texture != nullptr) {
        return *texture;
    }

    return addNewTexture(filePath, filtering, repeatTexture);
}

//Tries to retrieve a cubemap via file paths, if it's not in the cubemap library then it adds it.
CubeMap& TextureHandler::getCubeMap(const std::string identifier, const std::vector<std::string>& faces) {

#ifdef DEBUG
    assert(faces.size() == 6);
#endif

    std::map<std::string, CubeMap*>::iterator itr;

    for (itr = cubeMapLibrary.begin(); itr != cubeMapLibrary.end(); ++itr) {

        if (itr->first == identifier && itr->second != nullptr) {
            DBG_LOG("Cubmap identifier %s exists, returning.\n", itr->first.c_str());
            return *itr->second;
        }
    }

    GLuint textureID;
    glGenTextures(1, &textureID);

    CubeMap* cubemap = new CubeMap(faces, textureID);

    cubeMapLibrary.insert({ identifier, cubemap });

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

    for (unsigned int i = 0; i < faces.size(); i++) {

        SDL_Surface* surface;
        if ((surface = IMG_Load(faces[i].c_str()))) { // get number of bytes per pixel (for colors)

            verifySurfaceDimensions(*surface);
            GLenum textureFormat = getSurfaceFormat(*surface);
            bool transparent     = textureFormat == GL_RGBA;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0,
                         textureFormat,
                         surface->w,
                         surface->h,
                         0,
                         textureFormat,
                         GL_UNSIGNED_BYTE,
                         surface->pixels);

            SDL_FreeSurface(surface);
        } else {

            DBG_LOG("Image could not load properly, using null texture\n");
            DBG_LOG("The location of the non functioning texture is ");
            DBG_LOG(faces[i].c_str());
            DBG_LOG("\nNOTE: If one or more but not all images in the map are not loaded, the whole cubemap will be black.\n");

            teximage2DFillerTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
        }
    }

    //For some reason unbinding the cubemap here will make the map appear black.
    //glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return *cubemap;
}

//Free memory allocated for cubemaps and textures.
//All allocated pointers are stored in textureLibrary and cubeMapLibrary
TextureHandler::~TextureHandler() {

    DBG_LOG("Freeing memory for cube maps.\n");

    for (std::map<std::string, CubeMap*>::iterator it = cubeMapLibrary.begin(); it != cubeMapLibrary.end(); ++it) {
        if (it->second != nullptr) {
            delete it->second;
            it->second = nullptr;
        }
    }
    cubeMapLibrary.clear();

    DBG_LOG("Freeing memory for textures.\n");
    for (std::vector<Texture*>::iterator it = textureLibrary.begin(); it != textureLibrary.end(); ++it) {
        if (*it != nullptr) {
            delete *it;
            *it = nullptr;
        }
    }
    textureLibrary.clear();
}
