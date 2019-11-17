#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H
#include "Camera.h"
#include "Component.h"
#include "Locator.h"
#include "ThirdPersonCamera.h"

class ThirdPersonCameraControllerTest : public Component<ThirdPersonCameraControllerTest> {
public:
    void update(ThirdPersonCamera* camera) {

        if (!camera) {
            return;
        }

        float dt = GameInfo::getDeltaTime();

        camera->rotation.y += InputLocator::getService().getMouseDelta().x * dt * cameraSpeed;
        camera->rotation.z -= InputLocator::getService().getMouseDelta().y * dt * cameraSpeed;

        float cameraMaxPitch = 70.0f;

        if (camera->rotation.z >= cameraMaxPitch) {
            camera->rotation.z = cameraMaxPitch;
        } else if (camera->rotation.z <= -cameraMaxPitch) {
            camera->rotation.z = -cameraMaxPitch;
        }

        if (InputLocator::getService().isKeyDown(SDLK_o)) //use UserControls class for this type of thing
        {
            camera->setCameraDistance(camera->getCameraDistance() + cameraSpeed * dt);
        }
        if (InputLocator::getService().isKeyDown(SDLK_l)) {
            camera->setCameraDistance(camera->getCameraDistance() - cameraSpeed * dt);
        }
    }

private:
    const float cameraSpeed = 2.f;
};

#endif
