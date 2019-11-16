#ifndef THIRD_PERSON_CAMERA_H
#define THIRD_PERSON_CAMERA_H

#include "Camera.h"

class ThirdPersonCamera : public Component<ThirdPersonCamera>, public Camera {

public:
    ~ThirdPersonCamera() {}

    glm::vec3 getTargetPosition() const { return targetPosition; }
    void setTargetPosition(const glm::vec3& p) { targetPosition = p; }
    void setCameraDistance(float distance) { cameraDistance = distance; }

    float getCameraDistance() const { return cameraDistance; }

private:
    glm::vec3 targetPosition;
    float cameraDistance = 12;

    friend class ThirdPersonCameraSystem;
};

#endif
