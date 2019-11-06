#ifndef POINT_LIGHT_SHADOW_MAP_H
#define POINT_LIGHT_SHADOW_MAP_H
#include "ShaderBase.h"
#include "Texture.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class PointLightShadowMap {
public:
    PointLightShadowMap();

    const float FOV = glm::radians(90.0f);

    GLuint getCubeMap() { return depthCubeMap; }
    GLint getDepthMapShader() { return depthMapShader.getProgramID(); }
    GLfloat getFarPlane() { return farPlane; }
    glm::vec3 getCurrentLightPosition() { return lightPosition; }
    void setCurrentLightPosition(const glm::vec3& lightPos) { lightPosition = lightPos; }

    unsigned int getDepthMapWidth() const { return DEPTH_MAP_WIDTH; }
    unsigned int getDepthMapHeight() const { return DEPTH_MAP_HEIGHT; }
    void setDepthMapResolution(const unsigned int& width, const unsigned int& height) {
        DEPTH_MAP_WIDTH  = width;
        DEPTH_MAP_HEIGHT = height;
        updateDepthMapResolution();
    }

    GLuint getFBO() { return depthMapFBO; }
    void setShadowActive(bool t) { lightSupplied = t; }
    bool isActive() { return lightSupplied; }

    void initialize();
    void updateDepthMap();

private:
    float aspect;
    void updateDepthMapResolution() {

        if (DEPTH_MAP_HEIGHT > 0 && DEPTH_MAP_WIDTH > 0) {
            aspect = static_cast<float>(DEPTH_MAP_WIDTH) / static_cast<float>(DEPTH_MAP_HEIGHT);

            glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

            for (GLuint i = 0; i < 6; ++i) {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    GL_DEPTH_COMPONENT24, // GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT32 also should work but 24 is a good inbetween,
                    DEPTH_MAP_WIDTH,
                    DEPTH_MAP_HEIGHT,
                    0,
                    GL_DEPTH_COMPONENT,
                    GL_FLOAT,
                    NULL);
            }
        } else {
            DBG_LOG("The Depth Map Resolution Needs To Be Greater Than 0.\n");
        }
    }
    //1024 is amazing / great quality
    unsigned int DEPTH_MAP_WIDTH  = 512;
    unsigned int DEPTH_MAP_HEIGHT = 512;

    ShaderBase depthMapShader;

    bool lightSupplied = false;

    glm::mat4 getShadowTransformation(const glm::vec3& eye, const glm::vec3& up);
    GLuint depthMapFBO = -1;
    GLuint depthCubeMap;
    glm::mat4 shadowTransforms[6];

    glm::vec3 lightPosition;

    GLfloat nearPlane;
    GLfloat farPlane;

    glm::mat4 shadowProjection;
};

#endif // !DEPTH_MAP_H
