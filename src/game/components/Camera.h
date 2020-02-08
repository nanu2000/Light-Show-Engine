#ifndef CAMERA_H
#define CAMERA_H

#include "Component.h"
#include "GameInfo.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <GL/glew.h>

class Camera : public Component<Camera> {

public:
    Camera() {}
    ~Camera() {}

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);

    //Get the camera target position- where it's looking
    glm::vec3 getTargetPosition() const { return targetPosition; }

    //Distance from camera's target
    float getCameraDistance() const { return cameraDistance; }

    //Expensive, be weary
    glm::vec3 getRight() const { return glm::normalize(glm::cross(cameraFront, cameraUp)); }

    glm::vec3 getForward() const { return cameraFront; }

    glm::mat4* const getViewMatrix() { return &viewMatrix; }
    glm::mat4* const getProjectionMatrix() { return &projectionMatrix; }

    //Field of view
    float getFOV() { return currentFOV; }

    float getNearPlane() { return nearPlane; }
    float getFarPlane() { return farPlane; }

    //Set where the camera's looking
    void setTargetPosition(const glm::vec3& p) { targetPosition = p; }

    //Distance from camera target
    void setCameraDistance(float distance) { cameraDistance = distance; }

    void setFOV(float fov) { currentFOV = fov; }

private:
    float currentFOV = 70.0f;
    float nearPlane  = .5f;
    float farPlane   = 300.0f;

    float cameraDistance = 12;
    float speed          = 3.f;

    int currentWidthOfWindow  = 0;
    int currentHeightOfWindow = 0;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 rotation = glm::vec3(90.f, 0.f, -90.f);
    glm::vec3 targetPosition;

    friend class CameraSystem;
};

#endif // !CAMERA_H
