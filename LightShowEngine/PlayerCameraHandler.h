#ifndef PLAYER_CAMERA_HANDLER
#define PLAYER_CAMERA_HANDLER

#include "Component.h"
#include <glm/vec3.hpp>

class PlayerCameraHandler : public Component<PlayerCameraHandler> {

public:
    void setCameraTargetOffset(const glm::vec3& offset) { cameraTargetOffset = offset; }
    void setYInterpolationSpeed(float speed) { cameraYInterpolationSpeed = speed; }

    const glm::vec3* getCameraTargetOffset() { return &cameraTargetOffset; }
    float getYinterpolationSpeed() const { return cameraYInterpolationSpeed; }

private:
    glm::vec3 cameraTargetOffset    = glm::vec3(0, 2.5f, 0);
    float cameraYInterpolationSpeed = 5;
};

#endif
