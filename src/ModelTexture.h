#ifndef MODEL_TEXTURE
#define MODEL_TEXTURE
#include <string>

namespace _3DM {
    enum TextureType {
        Diffuse  = 0,
        Specular = 1,
        Normals  = 2
    };
    struct ModelTexture {
        std::string imagePath;
        TextureType imageType;

        //These variables do not need serialization
        std::string uniformName;
        unsigned int imageID;
    };
};

#endif
