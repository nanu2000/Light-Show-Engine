#ifndef PLAYER_CAMERA_HANDLING_SYSTEM
#define PLAYER_CAMERA_HANDLING_SYSTEM

#include "Camera.h"
#include "HelpingHand.h"
#include "PlayerCameraHandler.h"

class PlayerCameraHandlingSystem {

public:
    void setThirdPersonCameraTargetPosition(PlayerCameraHandler& playerController, const Transform& modelsTransform, Camera& camera) {
        //Modify this once a new camera controller component is created

        float newYPosition = modelsTransform.position.y + playerController.getCameraTargetOffset()->y;

        camera.setTargetPosition(
            glm::vec3(
                modelsTransform.position.x + playerController.getCameraTargetOffset()->x,
                newYPosition,
                modelsTransform.position.z + playerController.getCameraTargetOffset()->z));
    }
};

#endif
