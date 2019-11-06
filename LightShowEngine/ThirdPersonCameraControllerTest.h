#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H
#include "Component.h"
#include "Locator.h"
#include "ThirdPersonCamera.h"
#include "camera.h"

class ThirdPersonCameraControllerTest : public Component<ThirdPersonCameraControllerTest> {
public:
    void update(ThirdPersonCamera* camera) {

        if (!camera) {
            return;
        }

        camera->rotation.y += InputLocator::getService().getMouseDelta().x * cameraSpeed;

        camera->rotation.z -= InputLocator::getService().getMouseDelta().y * cameraSpeed;

        float cameraMaxPitch = 70.0f;

        if (camera->rotation.z >= cameraMaxPitch) {
            camera->rotation.z = cameraMaxPitch;
        } else if (camera->rotation.z <= -cameraMaxPitch) {
            camera->rotation.z = -cameraMaxPitch;
        }

        if (InputLocator::getService().isKeyDown(SDLK_o)) //use UserControls class for this type of thing
        {
            camera->setCameraDistance(camera->getCameraDistance() + cameraSpeed);
        }
        if (InputLocator::getService().isKeyDown(SDLK_l)) {
            camera->setCameraDistance(camera->getCameraDistance() - cameraSpeed);
        }
    }

private:
    const float cameraSpeed = 10 * GameInfo::deltaTime;
};

#endif
