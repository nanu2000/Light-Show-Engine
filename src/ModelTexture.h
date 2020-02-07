#ifndef MODEL_TEXTURE
#define MODEL_TEXTURE
#include <string>

namespace _3DM {
    enum class TextureType {
        Diffuse  = 0,
        Specular = 1,
        Normals  = 2
    };
    struct ModelTexture {
        std::string imagePath = "";
        _3DM::TextureType imageType;

        //These variables do not need serialization
        std::string uniformName = "";
        unsigned int imageID    = 0;
    };
};

#endif
