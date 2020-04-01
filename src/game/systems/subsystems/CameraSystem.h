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
    //!Initialize the camera
    virtual void initialize(Scene& currentScene, Engine::SystemVitals& systemVitals) override;

    //!Used to recieve message to refresh the camera
    virtual void recieveMessage(const BackEndMessages& msg, Scene& currentScene, Engine::SystemVitals& systemVitals) override;

    void update(Camera& camera);

    void refreshCamera(Camera& camera);
};

#endif
