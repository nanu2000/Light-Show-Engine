#include "EnemyControllerSystem.h"

void EnemyControllerSystem::calculateMovment(Input& input, const glm::quat& rotation, const glm::vec3& playersPosition, CollisionMesh& collisionMesh, EnemyController& enemyController) {

    ENEMY_ANIMATION_STATE currentMovmentState = ENEMY_ANIMATION_STATE::Idle;
    ENEMY_ROTATION currentRotation            = ENEMY_ROTATION::Unknown;

    switch (enemyController.enemyMovmentType) {

    case ENEMY_MOVEMENT_TYPE::Idle_Facing_Player:

        enemyController.lastKnownTargetPosition = playersPosition;
        currentRotation                         = ENEMY_ROTATION::Towards_Target;
        break;

    case ENEMY_MOVEMENT_TYPE::Idle:

        enemyController.lastKnownTargetPosition = playersPosition;
        currentRotation                         = ENEMY_ROTATION::Unknown;
        break;

    default:
        break;
    }

    enemyController.currentRotation = currentRotation;
    enemyController.currentState    = currentMovmentState;
}

void EnemyControllerSystem::handleRayHit(EnemyController& enemyController, CollisionMesh& collisionMesh, float closest) {

    collisionMesh.setVelocity(btVector3(collisionMesh.getVelocitybt().x(), ENEMY_CTRLR_NS::CLAMPING_VELOCITY, collisionMesh.getVelocitybt().z()));

    collisionMesh.translate(btVector3(0, closest, 0));

    enemyController.isTouchingGround = true;
    enemyController.jumping          = false;
}

int EnemyControllerSystem::getLowestRayHitIndex(const RayCaster& rayResults, EnemyController& enemyController) {
    float closest = 0;
    int index     = -1;

    for (int i = 0; i < rayResults.size(); i++) {

        CollisionTag* collider = static_cast<CollisionTag*>(rayResults.getCollisionObject(i)->getUserPointer());

        if (!collider) {
            continue;
        }

        if (collider->tagName == COLLISION_TAGS::TAG_EMPTY) {

            float newPosAddition = 1.0f - rayResults.getHitFraction(i);

            if (newPosAddition >= .5f) {
                //the hit position is above the player, so we ignore it. Maybe eventually it will be of use.
                continue;
            } else if (closest < newPosAddition) {
                closest = newPosAddition;
                index   = i;
            }
        }
    }

    return index;
}

ControllerRayCollision EnemyControllerSystem::getLowestRayHitForAllRays(EnemyController& enemyController) {
    ControllerRayCollision collisionInfo;

    for (unsigned int i = 0; i < ENEMY_CTRLR_NS::AMOUNT_OF_RAYS; i++) {
        if (ENEMY_CTRLR_NS::rayCast[i].hasHit()) {
            int index = getLowestRayHitIndex(ENEMY_CTRLR_NS::rayCast[i], enemyController);

            if (index != -1) {
                float amount = ENEMY_CTRLR_NS::rayCast[i].getLength() - ENEMY_CTRLR_NS::rayCast[i].getHitFractionRaySpace(index);

                if (amount > collisionInfo.lowestPointValue) {
                    collisionInfo.lowestPointIndex = index;
                    collisionInfo.lowestPointValue = amount;
                    collisionInfo.rayIndex         = i;
                }
            }
        }
    }

    return collisionInfo;
}

void EnemyControllerSystem::performCommands(CollisionMesh& collisionMesh, EnemyController& enemyController) {
}

void EnemyControllerSystem::applyForces(CollisionMesh& collisionMesh, EnemyController& enemyController) {
    collisionMesh.applyCentralImpulse(btVector3(0, enemyController.playerGravityImpulse, 0));
}

void EnemyControllerSystem::executeRayTesting(EnemyController& enemyController, CollisionMesh& collisionMesh, const Transform& meshTransform, PhysicsWorld& world) {
    float rayDistance = enemyController.colliderWidthAndDepth + ENEMY_CTRLR_NS::RAY_DISTANCE_CORRECTOR;

    ENEMY_CTRLR_NS::rayCast[0].rayTest(*world.getWorld(), glm::vec3(meshTransform.position.x - rayDistance, meshTransform.position.y + enemyController.halfLengthOfRay, meshTransform.position.z - rayDistance), glm::vec3(meshTransform.position.x - rayDistance, meshTransform.position.y - enemyController.halfLengthOfRay, meshTransform.position.z - rayDistance));
    ENEMY_CTRLR_NS::rayCast[1].rayTest(*world.getWorld(), glm::vec3(meshTransform.position.x - rayDistance, meshTransform.position.y + enemyController.halfLengthOfRay, meshTransform.position.z + rayDistance), glm::vec3(meshTransform.position.x - rayDistance, meshTransform.position.y - enemyController.halfLengthOfRay, meshTransform.position.z + rayDistance));
    ENEMY_CTRLR_NS::rayCast[2].rayTest(*world.getWorld(), glm::vec3(meshTransform.position.x + rayDistance, meshTransform.position.y + enemyController.halfLengthOfRay, meshTransform.position.z + rayDistance), glm::vec3(meshTransform.position.x + rayDistance, meshTransform.position.y - enemyController.halfLengthOfRay, meshTransform.position.z + rayDistance));
    ENEMY_CTRLR_NS::rayCast[3].rayTest(*world.getWorld(), glm::vec3(meshTransform.position.x + rayDistance, meshTransform.position.y + enemyController.halfLengthOfRay, meshTransform.position.z - rayDistance), glm::vec3(meshTransform.position.x + rayDistance, meshTransform.position.y - enemyController.halfLengthOfRay, meshTransform.position.z - rayDistance));

    ControllerRayCollision collision = getLowestRayHitForAllRays(enemyController);

    if (collision != ControllerRayCollision::INVALID_COLLISION) {

        handleRayHit(enemyController, collisionMesh, collision.lowestPointValue);

        if (collision.lowestPointIndex < rayCast[collision.rayIndex].size()) {

            enemyController.rayNormal = hh::toGlmVec3(rayCast[collision.rayIndex].getHitNormal(collision.lowestPointIndex));

            collisionMesh.setVelocity(collisionMesh.getVelocitybt());
        }
    } else if (enemyController.isTouchingGround) {
        enemyController.isTouchingGround = false;
    }
}

void EnemyControllerSystem::applyNewTransform(CollisionMesh& mesh, const glm::vec3& playersPosition, EnemyController& enemyController, Transform& oldTransform) {

    glm::vec3 forward = mesh.getPosition() - playersPosition;

    oldTransform.rotation
        = glm::quat(
            glm::vec3(
                hh::signedRadiansBetweenVectors(glm::vec3(forward.x, 0, forward.z), glm::vec3(1, 0, 0)),
                0,
                glm::radians(-90.f)));

    oldTransform.position = mesh.getPosition() + enemyController.offsetFromCollider;
}

void EnemyControllerSystem::update(Input& input, EntityTransform& modelsT, CollisionMesh& mesh, PhysicsWorld& world, const GlobalInformation& globalInformation, EnemyController& enemyController) {

    Transform& modelsTransform = modelsT.transform;

    mesh.activateRigidBody(true);

    //Perform Calculations
    executeRayTesting(enemyController, mesh, mesh.getTransformation(), world);
    ///////////////////////

    ////////////////Perform Actions
    calculateMovment(input, modelsTransform.rotation, globalInformation.getPlayersPosition(), mesh, enemyController);
    performCommands(mesh, enemyController);
    ////////////////

    //////////////Perform automatics
    //Apply gravity
    applyForces(mesh, enemyController);
    /////////////////

    /////////////////Transform Model Orientation according to collision mesh and camera.
    applyNewTransform(mesh, globalInformation.getPlayersPosition(), enemyController, modelsTransform);
    ///////////////////
}
