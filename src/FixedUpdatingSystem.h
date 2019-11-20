#ifndef FIXED_UPDATING_SYSTEM
#define FIXED_UPDATING_SYSTEM

#include "DebuggingController.h"
#include "DirectionalLightShadowMap.h"
#include "DisplayStatistics.h"
#include "EntityStatsDisplayer.h"
#include "FountainParticleEmitter.h"
#include "GUIResizingInfo.h"
#include "GlobalInformation.h"
#include "GuiButton.h"
#include "Messenger.h"
#include "PlayerController.h"
#include "PointLightShadowMap.h"
#include "RayCaster.h"
#include "Scene.h"
#include "Shader.h"
#include "SystemBase.h"
#include "ThirdPersonCamera.h"
#include "ThirdPersonCameraControllerTest.h"

class FixedUpdatingSystem : public SystemBase {

public:
    void initialize(Scene& scene, Settings& settings, PhysicsWorld& world, SubSystems& systems) override;

    void fixedUpdate(const Time& time, PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap);

    void handleBackEndMessage(BackEndMessages msg);

private:
    //Updates GUI (Will be called even if the game is paused) will return true if the game is paused
    bool updateGUI(const Time& time, PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap);

    //Updates the shadow maps and sets the current shadows light position.
    void updateShadowMaps(PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap, Camera& currentCamera);

    //Updates any collision logic if necessery.
    void updateCollision(const int32_t entity, CollisionMesh& collisionMesh);

    //checks to see if an entity is colliding with another entity.
    void updateCollisionTriggers(const CollisionTag& thisTag);

    //executes any logic regarding collisions.
    void handleCollisionTrigger(const CollisionTag& thisTag, const CollisionTag& otherTag);
};

#endif
