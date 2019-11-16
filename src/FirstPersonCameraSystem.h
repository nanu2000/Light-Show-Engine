#ifndef FIRST_PERSON_CAMERA_SYSTEM
#define FIRST_PERSON_CAMERA_SYSTEM
#include "Component.h"
#include "FirstPersonCamera.h"
#include "GameInfo.h"
#include "UniqueVectors.h"
#include <glm/gtc/matrix_transform.hpp>

class FirstPersonCameraSystem {

public:
    void initializeCamera(FirstPersonCamera& camera);

    void update(FirstPersonCamera& camera);

    void refreshCamera(FirstPersonCamera& camera);
};

#endif
