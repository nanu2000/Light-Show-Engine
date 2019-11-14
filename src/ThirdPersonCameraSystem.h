#ifndef THIRD_PERSON_CAMERA_SYSTEM
#define THIRD_PERSON_CAMERA_SYSTEM

#include "Component.h"
#include "GameInfo.h"
#include "HelpingHand.h"
#include "ThirdPersonCamera.h"
#include "UniqueVectors.h"
#include <glm/gtc/matrix_transform.hpp>

class ThirdPersonCameraSystem {

public:
    void initializeCamera(ThirdPersonCamera& camera);

    void update(ThirdPersonCamera& camera);

    void refreshCamera(ThirdPersonCamera& camera);
};

#endif
