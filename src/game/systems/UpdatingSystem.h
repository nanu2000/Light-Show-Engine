#ifndef UPDATING_SYSTEM
#define UPDATING_SYSTEM

#include "Camera.h"
#include "CollisionMesh.h"
#include "GlobalInformation.h"
#include "Locator.h"
#include "Messenger.h"
#include "ParticleSystem.h"
#include "PauseMenu.h"
#include "Scene.h"
#include "Settings.h"
#include "SystemBase.h"
#include "UserControls.h"
class UpdatingSystem : public SystemBase {

public:
    ~UpdatingSystem() {}

    void initialize(Scene& scene, Settings& settings, PhysicsWorld& world, SubSystems& systems) override;

    //Called from Game.cpp
    void update();

private:
};

#endif
