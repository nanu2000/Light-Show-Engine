#ifndef SKY_BOX_SYSTEM_H
#define SKY_BOX_SYSTEM_H

#include "SkyBox.h"

class SkyBoxSystem {
public:
    void init(SkyBox& skybox) { skybox.getCube()->create(); }
    void render(SkyBox& skybox, Camera& camera, ShaderBase& shader)
    {
        glDepthFunc(GL_LEQUAL);
        glCullFace(GL_FRONT);
        skybox.getCube()->render(shader, camera, skybox.getCubeMap());
        glCullFace(GL_BACK);
        glDepthFunc(GL_LESS);
    }
    void update(SkyBox& skybox) {}
};
#endif
