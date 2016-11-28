#ifndef FIRST_PERSON_CAMERA_SYSTEM
#define FIRST_PERSON_CAMERA_SYSTEM
#include "GameInfo.h"
#include <glm/gtc/matrix_transform.hpp>
#include "UniqueVectors.h"
#include "Component.h"
#include "FirstPersonCamera.h"

class FirstPersonCameraSystem
{

public:

	void initializeCamera(FirstPersonCamera& camera);

	void update(FirstPersonCamera& camera);

	void refreshCamera(FirstPersonCamera& camera);
	
};

#endif