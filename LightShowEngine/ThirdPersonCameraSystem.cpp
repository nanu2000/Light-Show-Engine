#include "ThirdPersonCameraSystem.h"

void ThirdPersonCameraSystem::initializeCamera(ThirdPersonCamera & camera)
{
	refreshCamera(camera);
}


void ThirdPersonCameraSystem::update(ThirdPersonCamera & camera)
{





	camera.position		= camera.targetPosition - hh::sphericalToCartisean(glm::radians(camera.rotation.y), glm::radians(camera.rotation.z), camera.cameraDistance);

	camera.viewMatrix	= glm::lookAt(camera.position, camera.targetPosition, glm::vec3(0,1,0));

	camera.cameraFront	= -glm::normalize(camera.position - camera.targetPosition);

}

void ThirdPersonCameraSystem::refreshCamera(ThirdPersonCamera & camera)
{
	camera.projectionMatrix = glm::perspective
	(
		glm::radians(camera.currentFOV),
		static_cast<float>(GameInfo::getWindowWidth()) / static_cast<float>(GameInfo::getWindowHeight()),
		camera.nearPlane,
		camera.farPlane
	);
}
