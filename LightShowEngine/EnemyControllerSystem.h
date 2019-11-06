#ifndef ENEMY_CONTROLLER_SYSTEM_H
#define ENEMY_CONTROLLER_SYSTEM_H

#include "CollisionMesh.h"
#include "Component.h"
#include "ControllerRayCollision.h"
#include "EnemyController.h"
#include "EntityTransform.h"
#include "GlobalInformation.h"
#include "PhysicsWorld.h"
#include "RayCaster.h"
#include "TestEnemyAI.h"
#include "ThirdPersonCamera.h"
#include "UserControls.h"

namespace ENEMY_CTRLR_NS {
const unsigned int AMOUNT_OF_RAYS  = 4;
const float CLAMPING_VELOCITY      = -5;
const float RAY_DISTANCE_CORRECTOR = -0.1f;
}

class EnemyControllerSystem {

public:
    EnemyControllerSystem() {
        assert(ENEMY_CTRLR_NS::AMOUNT_OF_RAYS == 4);
    }

    void update(Input& input, EntityTransform& modelsTransform, CollisionMesh& mesh, PhysicsWorld& world, const GlobalInformation& globalInformation, EnemyController& playerController, const UserControls& userControls, const TestEnemyAI& ai);

private:
    RayCaster rayCast[ENEMY_CTRLR_NS::AMOUNT_OF_RAYS];

    void performJump(CollisionMesh& collisionMesh, float jumpVelocity);

    void calculateMovment(Input& input, const glm::quat& rotation, const glm::vec3& playersPosition, CollisionMesh& collisionMesh, EnemyController& playerController, const UserControls& userControls, const TestEnemyAI& ai);

    void handleRayHit(EnemyController& playerController, CollisionMesh& collisionMesh, float closest);

    int getLowestRayHitIndex(const RayCaster& rayResults, EnemyController& playerController);

    ControllerRayCollision getLowestRayHitForAllRays(EnemyController& playerController);

    void executeRayTesting(EnemyController& playerController, CollisionMesh& collisionMesh, const Transform& meshTransform, PhysicsWorld& world);

    glm::vec3 getSlopeSpeed(float sensitivity, const glm::vec3& rayNormal) { return rayNormal * sensitivity * GameInfo::deltaTime; }

    int getCurrentSlopeSpeedIndex(const glm::vec3& rayNormal, const EnemyController& playerController);

    void performCommands(CollisionMesh& collisionMesh, EnemyController& playerController);

    void applyForces(CollisionMesh& collisionMesh, EnemyController& playerController);

    void applyNewTransform(CollisionMesh& mesh, const glm::vec3& playersPosition, EnemyController& playerController, Transform& oldTransform, const TestEnemyAI& ai);
};

#endif
