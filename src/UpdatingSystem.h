#ifndef UPDATING_SYSTEM
#define UPDATING_SYSTEM

#include "CollisionMesh.h"
#include "GlobalInformation.h"
#include "Locator.h"
#include "Messenger.h"
#include "PauseMenu.h"
#include "Scene.h"
#include "Settings.h"
#include "SystemBase.h"
#include "ThirdPersonCameraControllerTest.h"
#include "UserControls.h"
class UpdatingSystem : public SystemBase {

public:
    void initialize(Scene& scene, Settings& settings, PhysicsWorld& world, SubSystems& systems) override;

    //Called from Game.cpp
    void update();

private:
};

#endif
