#ifndef PLAYER_CONTROLLER_SYSTEM
#define PLAYER_CONTROLLER_SYSTEM

#include "Camera.h"
#include "CollisionMesh.h"
#include "Component.h"
#include "ControllerRayCollision.h"
#include "PhysicsWorld.h"
#include "PlayerController.h"
#include "UserControls.h"

//!This is sort of a test system just to mess around when I want to take a break from other tasks.
class PlayerControllerSystem {

public:
    PlayerControllerSystem() {
        assert(PlayerController::AMOUNT_OF_RAYS == 8);
    }

    void update(Transform& modelsTransform, PlayerController& playerController, Camera& camera, CollisionMesh& mesh);
    void fixedUpdate(Input& input, Transform& modelsTransform, CollisionMesh& mesh, PhysicsWorld& world, PlayerController& playerController, Camera& camera, const UserControls& userControls);
    void debugRender(PhysicsWorld& w, PlayerController& p);

private:
    void performJump(CollisionMesh& collisionMesh, float jumpVelocity);

    void getPlayerInput(Input& input, CollisionMesh& collisionMesh, PlayerController& playerController, Camera& camera, const UserControls& userControls);

    void handleRayHit(PlayerController& playerController, CollisionMesh& collisionMesh, float closest);

    int getLowestRayHitIndex(const RayCaster& rayResults, PlayerController& playerController);

    ControllerRayCollision getLowestRayHitForAllRays(PlayerController& playerController);

    void executeRayTesting(PlayerController& playerController, CollisionMesh& collisionMesh, const Transform& meshTransform, PhysicsWorld& world);

    glm::vec3 getSlopeSpeed(float sensitivity, const glm::vec3& rayNormal) { return rayNormal * sensitivity * GameInfo::fixedDeltaTime; }

    int getCurrentSlopeSpeedIndex(const glm::vec3& rayNormal, const PlayerController& playerController);

    void performCommands(CollisionMesh& collisionMesh, PlayerController& playerController);

    void applyForces(CollisionMesh& collisionMesh, PlayerController& playerController);

    void applyNewTransform(CollisionMesh& mesh, const glm::vec3& cameraForward, const PlayerController& playerController, Transform& oldTransform);

    void applyNewRotation(const glm::vec3& cameraForward, const PlayerController& playerController, Transform& oldTransform);
};

#endif
