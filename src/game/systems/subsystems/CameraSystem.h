#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H
#include "Camera.h"
#include "Component.h"
#include "GameInfo.h"
#include "HelpingHand.h"
#include "Locator.h"
#include <glm/gtc/matrix_transform.hpp>

class CameraSystem {

public:
    void initializeCamera(Camera& camera);

    void update(Camera& camera);

    void refreshCamera(Camera& camera);
};

#endif
