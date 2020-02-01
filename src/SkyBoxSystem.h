#ifndef SKY_BOX_SYSTEM_H
#define SKY_BOX_SYSTEM_H

#include "SkyBox.h"

class SkyBoxSystem {
public:
    void init(SkyBox& skybox) { skybox.getCube()->create(); }
    void render(SkyBox& skybox, Camera& camera, Shader& shader) {
        glDepthFunc(GL_LEQUAL);
        glCullFace(GL_FRONT);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getCubeMap()->getCubeMapData());
        skybox.getCube()->render(shader, camera);
        glCullFace(GL_BACK);
        glDepthFunc(GL_LESS);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    // void update(SkyBox& skybox) {}
};
#endif
