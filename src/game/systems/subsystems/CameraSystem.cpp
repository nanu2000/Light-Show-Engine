#include "CameraSystem.h"

void CameraSystem::initializeCamera(Camera& camera) {
    refreshCamera(camera);
}

void CameraSystem::update(Camera& camera) {

    float dt = GameInfo::getDeltaTime();

    camera.rotation.y += InputLocator::getService().getMouseDelta().x * dt * camera.speed;
    camera.rotation.z -= InputLocator::getService().getMouseDelta().y * dt * camera.speed;

    float cameraMaxPitch = 70.0f;

    if (camera.rotation.z >= cameraMaxPitch) {
        camera.rotation.z = cameraMaxPitch;
    } else if (camera.rotation.z <= -cameraMaxPitch) {
        camera.rotation.z = -cameraMaxPitch;
    }

    if (InputLocator::getService().isKeyDown(SDLK_o)) //use UserControls class for this type of thing
    {
        camera.setCameraDistance(camera.getCameraDistance() + camera.speed * dt);
    }
    if (InputLocator::getService().isKeyDown(SDLK_l)) {
        camera.setCameraDistance(camera.getCameraDistance() - camera.speed * dt);
    }

    camera.position = camera.targetPosition - hh::sphericalToCartisean(glm::radians(camera.rotation.y), glm::radians(camera.rotation.z), camera.cameraDistance);

    camera.viewMatrix = glm::lookAt(camera.position, camera.targetPosition, glm::vec3(0, 1, 0));

    camera.cameraFront = -glm::normalize(camera.position - camera.targetPosition);
}

void CameraSystem::refreshCamera(Camera& camera) {
    camera.projectionMatrix = glm::perspective(
        glm::radians(camera.currentFOV),
        static_cast<float>(GameInfo::getWindowWidth()) / static_cast<float>(GameInfo::getWindowHeight()),
        camera.nearPlane,
        camera.farPlane);
}
