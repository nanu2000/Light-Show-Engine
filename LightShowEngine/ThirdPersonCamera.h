#ifndef THIRD_PERSON_CAMERA_H
#define THIRD_PERSON_CAMERA_H

#include "GameInfo.h"
#include <glm/gtc/matrix_transform.hpp>
#include "UniqueVectors.h"
#include "Component.h"
#include "Camera.h"

class ThirdPersonCamera : public Component<ThirdPersonCamera>, public Camera
{

public:

	~ThirdPersonCamera(){}

	glm::vec3 getTargetPosition	 () const { return targetPosition; }
	void setTargetPosition	 (const glm::vec3& position)	{ targetPosition = position; }
	void setCameraDistance	 (float distance)				{ cameraDistance = distance; }

	float getCameraDistance  () const { return cameraDistance; }

private:

	glm::vec3 targetPosition;
	float cameraDistance = 20;

	friend class ThirdPersonCameraSystem;
};

#endif