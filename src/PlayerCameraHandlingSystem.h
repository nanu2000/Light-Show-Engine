#ifndef PLAYER_CAMERA_HANDLING_SYSTEM
#define PLAYER_CAMERA_HANDLING_SYSTEM

#include "HelpingHand.h"
#include "PlayerCameraHandler.h"
#include "ThirdPersonCamera.h"

class PlayerCameraHandlingSystem {

public:
    void setThirdPersonCameraTargetPosition(PlayerCameraHandler& playerController, const Transform& modelsTransform, ThirdPersonCamera& camera) {
        //Modify this once a new camera controller component is created

        float newYPosition
            = hh::lerp(
                camera.getTargetPosition().y,
                modelsTransform.position.y + playerController.getCameraTargetOffset()->y,
                glm::clamp(GameInfo::deltaTime * playerController.getYinterpolationSpeed(), 0.0f, 1.0f));

        camera.setTargetPosition(
            glm::vec3(
                modelsTransform.position.x + playerController.getCameraTargetOffset()->x,
                newYPosition,
                modelsTransform.position.z + playerController.getCameraTargetOffset()->z));
    }
};

#endif
