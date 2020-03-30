#ifndef ENEMY_CONTROLLER_SYSTEM_H
#define ENEMY_CONTROLLER_SYSTEM_H

#include "Camera.h"
#include "CollisionMesh.h"
#include "Component.h"
#include "ControllerRayCollision.h"
#include "EnemyController.h"
#include "EntityTransform.h"
#include "GlobalInformation.h"
#include "PhysicsWorld.h"
#include "RayCaster.h"
#include "SystemBase.h"

namespace ENEMY_CTRLR_NS {
    const unsigned int AMOUNT_OF_RAYS  = 4;
    const float CLAMPING_VELOCITY      = -5;
    const float RAY_DISTANCE_CORRECTOR = -0.1f;
    static RayCaster rayCast[ENEMY_CTRLR_NS::AMOUNT_OF_RAYS];
}

class EnemyControllerSystem : public SystemBase {

public:
    EnemyControllerSystem() {
        assert(ENEMY_CTRLR_NS::AMOUNT_OF_RAYS == 4);
    }

    void update(Input& input, EntityTransform& modelsTransform, CollisionMesh& mesh, PhysicsWorld& world, const GlobalInformation& globalInformation, EnemyController& enemyController);

private:
    RayCaster rayCast[ENEMY_CTRLR_NS::AMOUNT_OF_RAYS];

    void calculateMovment(Input& input, const glm::quat& rotation, const glm::vec3& playersPosition, CollisionMesh& collisionMesh, EnemyController& enemyController);

    void handleRayHit(EnemyController& enemyController, CollisionMesh& collisionMesh, float closest);

    int getLowestRayHitIndex(const RayCaster& rayResults, EnemyController& enemyController);

    ControllerRayCollision getLowestRayHitForAllRays(EnemyController& enemyController);

    void executeRayTesting(EnemyController& enemyController, CollisionMesh& collisionMesh, const Transform& meshTransform, PhysicsWorld& world);

    void performCommands(CollisionMesh& collisionMesh, EnemyController& enemyController);

    void applyForces(CollisionMesh& collisionMesh, EnemyController& enemyController);

    void applyNewTransform(CollisionMesh& mesh, const glm::vec3& playersPosition, EnemyController& enemyController, Transform& oldTransform);
};

#endif
