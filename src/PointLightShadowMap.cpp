#include "PointLightShadowMap.h"

void PointLightShadowMap::initialize() {

    depthMapShader = ShaderBase(
        "assets/Shaders/PointLightDepthMapAnimated.v",
        "assets/Shaders/PointLightDepthMap.f",
        SHADER_TYPE::Default,
        "assets/Shaders/PointLightDepthMap.g");

    glGenTextures(1, &depthCubeMap);

    glGenFramebuffers(1, &depthMapFBO);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

    updateDepthMapResolution();

    //Needs to be called after updateDepthMapResolution
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        DBG_LOG("The FrameBuffer Was Not Created Succesfully!! Status: %i (PointLightShadowMap.cpp)", status);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 PointLightShadowMap::getShadowTransformation(const glm::vec3& eye, const glm::vec3& up) {
    return glm::mat4(shadowProjection * glm::lookAt(lightPosition, lightPosition + eye, up));
}

void PointLightShadowMap::updateDepthMap() {

    shadowProjection = glm::perspective(FOV, aspect, nearPlane, farPlane);

    shadowTransforms[0] = getShadowTransformation(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    shadowTransforms[1] = getShadowTransformation(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    shadowTransforms[2] = getShadowTransformation(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    shadowTransforms[3] = getShadowTransformation(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    shadowTransforms[4] = getShadowTransformation(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    shadowTransforms[5] = getShadowTransformation(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

    glViewport(0, 0, DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(depthMapShader.getProgramID());

    std::string loc = Shaders::getUniformName(Shaders::UniformName::ShadowMatrices) + std::string("[");
    for (GLuint i = 0; i < 6; ++i) {
        glUniformMatrix4fv(
            glGetUniformLocation(
                depthMapShader.getProgramID(),
                (loc + std::to_string(i) + "]").c_str()),
            1,
            GL_FALSE,
            glm::value_ptr(shadowTransforms[i]));
    }

    glUniform1f(Shaders::getUniformLocation(depthMapShader.getProgramID(), Shaders::UniformName::FarPlane), farPlane);

    glUniform3fv(
        Shaders::getUniformLocation(depthMapShader.getProgramID(), Shaders::UniformName::LightPosition),
        1,
        &lightPosition[0]);
}
