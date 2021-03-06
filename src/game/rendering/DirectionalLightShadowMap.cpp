#include "DirectionalLightShadowMap.h"

void DirectionalLightShadowMap::updateDepthMap(const Camera& camera) {

    lightSpaceMatrix = glm::ortho(bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.zNear, bounds.zFar) * glm::lookAt(-lightDirection + camera.getTargetPosition(), camera.getTargetPosition(), hh::UP_VECTOR);

    depthMapShader.useProgram();

    glUniformMatrix4fv(
        Shaders::getUniformLocation(depthMapShader.getProgramID(), Shaders::UniformName::LightSpaceMatrix),
        1,
        GL_FALSE,
        glm::value_ptr(lightSpaceMatrix));
}

void DirectionalLightShadowMap::initialize() {

    Shader x = ShaderLocator::getService().getShader("dirshadowmap",
                                                     "assets/shaders/directional-light-shadow-map.vert",
                                                     "assets/shaders/empty.frag",
                                                     SHADER_TYPE::Default);

    depthMapShader = x;

    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);

    updateDepthMapResolution();

    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    initialized = true;
}
void DirectionalLightShadowMap::updateDepthMapResolution() {
    if (DEPTH_MAP_HEIGHT > 0 && DEPTH_MAP_WIDTH > 0) {

        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_DEPTH_COMPONENT24, // GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT32 also should work but 24 is a good inbetween
            DEPTH_MAP_WIDTH,
            DEPTH_MAP_HEIGHT,
            0,
            GL_DEPTH_COMPONENT,
            GL_FLOAT,
            NULL);
    } else {
        DBG_LOG("The Depth Map Resolution Needs To Be Greater Than 0.\n");
    }
}

DirectionalLightShadowMap::~DirectionalLightShadowMap() {
    if (!initialized) {
        return;
    }
    DBG_LOG("Freeing memory for Directional light depth map.\n");

    glDeleteFramebuffers(1, &depthMapFBO);
    glDeleteTextures(1, &depthMap);
    depthMapFBO = 0;
    depthMap    = 0;
    initialized = false;
}
