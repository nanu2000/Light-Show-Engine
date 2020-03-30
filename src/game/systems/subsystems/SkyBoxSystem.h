#ifndef SKY_BOX_SYSTEM_H
#define SKY_BOX_SYSTEM_H

#include "SkyBox.h"
#include "SystemBase.h"

class SkyBoxSystem : public SystemBase {
public:
    void init(SkyBox& skybox) { skybox.getCube()->create(); }
    void render(SkyBox& skybox, Camera& camera, Shader& shader) {
        glDepthFunc(GL_LEQUAL);
        glCullFace(GL_FRONT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getCubeMap()->getCubeMapData());
        skybox.getCube()->render(shader, camera);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glCullFace(GL_BACK);
        glDepthFunc(GL_LESS);
    }
    // void update(SkyBox& skybox) {}
};
#endif
