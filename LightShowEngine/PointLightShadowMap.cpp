#include "PointLightShadowMap.h"

PointLightShadowMap::PointLightShadowMap() {
    lightPosition = glm::vec3(0, 6, 0);
    nearPlane     = .10f;
    farPlane      = 50.0f;
}

void PointLightShadowMap::initialize() {

    depthMapShader = ShaderBase(
        "Assets/Shaders/PointLightDepthMapAnimated.v",
        "Assets/Shaders/PointLightDepthMap.f",
        SHADER_TYPE::Default,
        "Assets/Shaders/PointLightDepthMap.g");

    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthCubeMap);

    updateDepthMapResolution();

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        DBG_LOG("The FrameBuffer Was Not Created Succesfully!! (PointLightShadowMap.cpp)");
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
