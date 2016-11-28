#ifndef THIRD_PERSON_CAMERA_SYSTEM
#define THIRD_PERSON_CAMERA_SYSTEM

#include "GameInfo.h"
#include <glm/gtc/matrix_transform.hpp>
#include "UniqueVectors.h"
#include "Component.h"
#include "HelpingHand.h"
#include "ThirdPersonCamera.h"

class ThirdPersonCameraSystem
{

public:

	void initializeCamera(ThirdPersonCamera& camera);
	
	void update(ThirdPersonCamera& camera);

	void refreshCamera(ThirdPersonCamera& camera);

};


#endif