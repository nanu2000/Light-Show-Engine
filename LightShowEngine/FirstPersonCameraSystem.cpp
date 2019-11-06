#include "FirstPersonCameraSystem.h"

void FirstPersonCameraSystem::initializeCamera(FirstPersonCamera& camera) {
    refreshCamera(camera);
}

void FirstPersonCameraSystem::update(FirstPersonCamera& camera) {

    glm::vec3 rotationInRadians = glm::vec3(
        glm::radians(camera.rotation.x),
        glm::radians(camera.rotation.y),
        glm::radians(camera.rotation.z));

    camera.cameraFront.x = cos(rotationInRadians.y) * cos(rotationInRadians.z);
    camera.cameraFront.y = sin(rotationInRadians.y);
    camera.cameraFront.z = cos(rotationInRadians.y) * sin(rotationInRadians.z);

    camera.cameraUp.x = cos(rotationInRadians.x);
    camera.cameraUp.y = sin(rotationInRadians.x);

    camera.cameraUp    = glm::normalize(camera.cameraUp);
    camera.cameraFront = glm::normalize(camera.cameraFront);

    camera.viewMatrix = glm::lookAt(camera.position, camera.position + camera.cameraFront, camera.cameraUp);
}

void FirstPersonCameraSystem::refreshCamera(FirstPersonCamera& camera) {
    camera.projectionMatrix = glm::perspective(
        glm::radians(camera.currentFOV),
        static_cast<float>(GameInfo::getWindowWidth()) / static_cast<float>(GameInfo::getWindowHeight()),
        camera.nearPlane,
        camera.farPlane);
}
