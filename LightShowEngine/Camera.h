#ifndef CAMERA_H
#define CAMERA_H
#include "Camera.h"
#include "Component.h"
#include "GameInfo.h"
#include "UniqueVectors.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <GL/glew.h>

class Camera {
public:
    virtual ~Camera() {}

    glm::vec3 position           = glm::vec3(0.0f, 0.0f, 3.0f);
    UniqueVec3<GLfloat> rotation = UniqueVec3<GLfloat>(90.f, 0.f, -90.f);

    /******************************************
	* Returns the right vector of the camera. *
	*******************************************/
    glm::vec3 getRight() const { return glm::normalize(glm::cross(cameraFront, cameraUp)); }

    /********************************************
	* Returns the forward vector of the camera. *
	*********************************************/
    glm::vec3 getForward() const { return cameraFront; }

    /*****************************************
	* Returns the view matrix of the camera. *
	******************************************/
    glm::mat4* const getViewMatrix() { return &viewMatrix; }

    /***********************************************
	* Returns the projection matrix of the camera. *
	************************************************/
    glm::mat4* const getProjectionMatrix() { return &projectionMatrix; }

    float getFOV() { return currentFOV; }
    void setFOV(float fov) { currentFOV = fov; }

    float getNearPlane() { return nearPlane; }
    float getFarPlane() { return farPlane; }

protected:
    float currentFOV = 70.0f;

    float nearPlane = .5f;
    float farPlane  = 300.0f;

    int currentWidthOfWindow  = 0;
    int currentHeightOfWindow = 0;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
};

#endif // !CAMERA_H
