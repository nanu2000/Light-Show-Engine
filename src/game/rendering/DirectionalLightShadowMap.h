#ifndef DIRECTIONAL_LIGHT_SHADOW_MAP_H
#define DIRECTIONAL_LIGHT_SHADOW_MAP_H
#include "Camera.h"
#include "Debug.h"
#include "HelpingHand.h"
#include "Locator.h"
#include "Shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct OrthographicBounds {
    float left;
    float right;
    float bottom;
    float top;
    float zNear;
    float zFar;
};

class DirectionalLightShadowMap {

public:
    void initialize();

    //!Keep in mind that the destructor will call glDelete on FBO's and textures generated if the shadow map was properly initialized.
    DirectionalLightShadowMap() {}

    ~DirectionalLightShadowMap();

    DirectionalLightShadowMap(const DirectionalLightShadowMap&) = delete;
    DirectionalLightShadowMap(DirectionalLightShadowMap&&)      = delete;
    DirectionalLightShadowMap& operator=(const DirectionalLightShadowMap&) = delete;
    DirectionalLightShadowMap& operator=(DirectionalLightShadowMap&&) = delete;

    const glm::mat4* const getLightSpaceMatrix() { return &lightSpaceMatrix; }
    GLint getDepthMapShader() const { return depthMapShader.getProgramID(); }
    GLint getDepthMap() const { return depthMap; }
    GLuint getFBO() const { return depthMapFBO; }

    unsigned int getDepthMapWidth() const { return DEPTH_MAP_WIDTH; }
    unsigned int getDepthMapHeight() const { return DEPTH_MAP_HEIGHT; }

    void setDepthMapResolution(const unsigned int& width, const unsigned int& height) {
        DEPTH_MAP_WIDTH  = width;
        DEPTH_MAP_HEIGHT = height;
        updateDepthMapResolution();
    }

    void setShadowActive(bool t) { lightSupplied = t; }
    bool isActive() { return lightSupplied; }

    glm::vec3 getCurrentLightDirection() { return lightDirection; }
    void setCurrentLightDirection(const glm::vec3& lightDir) { lightDirection = glm::vec3(lightDir.x + securityAdditiveForDirection, lightDir.y + securityAdditiveForDirection, lightDir.z); }

    void updateDepthMap(const Camera& camera);

private:
    //4096 is amazing quality. Best quality would use multiple maps and csm
    unsigned int DEPTH_MAP_WIDTH  = 4096;
    unsigned int DEPTH_MAP_HEIGHT = 4096;

    //This floating point value will be added to two of the light direction vector's axes.
    //If 2/3 of the direction vector's axes is equal to 0, the shadows will not work.
    float securityAdditiveForDirection = .0001f;

    void updateDepthMapResolution();

    OrthographicBounds bounds { -40.0f, 40.0f, -40.0f, 40.0f, -150.0f, 150.0f }; //TODO: better ? makes shadow map cover more distance

    glm::vec3 lightDirection = glm::vec3(0.0001f, -1.f, 0);

    bool lightSupplied = false;

    glm::mat4 lightSpaceMatrix;
    GLuint depthMapFBO;
    Shader depthMapShader;
    GLuint depthMap;
    bool initialized = false;
};

#endif
