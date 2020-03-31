#ifndef UPDATING_SYSTEM
#define UPDATING_SYSTEM

#include "Camera.h"
#include "CollisionMesh.h"
#include "GlobalInformation.h"
#include "Locator.h"
#include "MainSystemBase.h"
#include "Messenger.h"
#include "ParticleSystem.h"
#include "PauseMenu.h"
#include "RenderTexture.h"
#include "Scene.h"
#include "Settings.h"
#include "UserControls.h"
class UpdatingSystem : public MainSystemBase {

public:
    ~UpdatingSystem() {}

    void initialize(Scene& scene, Settings& settings, PhysicsWorld& world, SubSystems& systems) override;

    void recieveSystemMessage(const BackEndMessages& msg, RenderTextureBase& renderTexture);

    //Called from Game.cpp
    void update();

private:
};

#endif
