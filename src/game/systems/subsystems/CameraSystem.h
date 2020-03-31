#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H
#include "Camera.h"
#include "Component.h"
#include "GameInfo.h"
#include "HelpingHand.h"
#include "Locator.h"
#include "Scene.h"
#include "SystemBase.h"
#include <glm/gtc/matrix_transform.hpp>

class CameraSystem : public SystemBase {

public:
    //!Used to recieve message to refresh the camera
    virtual void recieveMessage(const BackEndMessages& msg, Scene& currentScene) override;

    void initializeCamera(Camera& camera);

    void update(Camera& camera);

    void refreshCamera(Camera& camera);
};

#endif
