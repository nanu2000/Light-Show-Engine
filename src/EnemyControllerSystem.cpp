#include "EnemyControllerSystem.h"

void EnemyControllerSystem::calculateMovment(Input& input, const glm::quat& rotation, const glm::vec3& playersPosition, CollisionMesh& collisionMesh, EnemyController& enemyController, const TestEnemyAI& ai) {

    glm::vec3 forwardFromRotation = glm::normalize(rotation * glm::vec3(0, 1, 0));

    forwardFromRotation = glm::vec3(-forwardFromRotation.x, 0, -forwardFromRotation.z);

    glm::vec2 forwardFromRotationDelta = glm::vec2(forwardFromRotation.x, forwardFromRotation.z) * enemyController.speed;

    glm::vec3 forward          = -glm::normalize(collisionMesh.getPosition() - playersPosition);
    glm::vec3 lastKnownForward = -glm::normalize(collisionMesh.getPosition() - enemyController.lastKnownTargetPosition);

    glm::vec3 right = glm::cross(forward, glm::vec3(0, 1, 0));

    glm::vec2 lastKnownForwardDelta = glm::vec2(lastKnownForward.x, lastKnownForward.z) * enemyController.speed;
    glm::vec2 forwardDelta          = glm::vec2(forward.x, forward.z) * enemyController.speed;

    glm::vec3 rightDelta = right * enemyController.speed;

    ENEMY_ANIMATION_STATE currentMovmentState = ENEMY_ANIMATION_STATE::Idle;
    ENEMY_ROTATION currentRotation            = ENEMY_ROTATION::Unknown;

    switch (ai.getMovementType()) {

    case ENEMY_MOVEMENT_TYPE::Idle_Facing_Player:

        enemyController.lastKnownTargetPosition = playersPosition;
        currentRotation                         = ENEMY_ROTATION::Towards_Target;
        break;

    case ENEMY_MOVEMENT_TYPE::Idle:

        enemyController.lastKnownTargetPosition = playersPosition;
        currentRotation                         = ENEMY_ROTATION::Unknown;
        break;

    case ENEMY_MOVEMENT_TYPE::Walk_Away_From_Player:
        collisionMesh.addVelocity(
            btVector3(
                enemyController.currentSlopeIntensity.x() - forwardDelta.x,
                0,
                enemyController.currentSlopeIntensity.z() - forwardDelta.y));

        enemyController.lastKnownTargetPosition = playersPosition;
        currentMovmentState                     = ENEMY_ANIMATION_STATE::Walking;
        currentRotation                         = ENEMY_ROTATION::Away_From_Target;
        break;

    case ENEMY_MOVEMENT_TYPE::Walk_Towards_Player:
        collisionMesh.addVelocity(
            btVector3(
                enemyController.currentSlopeIntensity.x() + forwardDelta.x,
                0,
                enemyController.currentSlopeIntensity.z() + forwardDelta.y));

        enemyController.lastKnownTargetPosition = playersPosition;
        currentMovmentState                     = ENEMY_ANIMATION_STATE::Walking;
        currentRotation                         = ENEMY_ROTATION::Towards_Target;

        break;

    case ENEMY_MOVEMENT_TYPE::Strafe_Left_From_Player:
        collisionMesh.addVelocity(
            btVector3(
                enemyController.currentSlopeIntensity.x() - rightDelta.x,
                0,
                enemyController.currentSlopeIntensity.z() - rightDelta.z));

        enemyController.lastKnownTargetPosition = playersPosition;
        currentMovmentState                     = ENEMY_ANIMATION_STATE::Walking;
        currentRotation                         = ENEMY_ROTATION::Towards_Target;
        break;

    case ENEMY_MOVEMENT_TYPE::Strafe_Right_From_Player:
        collisionMesh.addVelocity(
            btVector3(
                enemyController.currentSlopeIntensity.x() + rightDelta.x,
                0,
                enemyController.currentSlopeIntensity.z() + rightDelta.z));

        enemyController.lastKnownTargetPosition = playersPosition;
        currentMovmentState                     = ENEMY_ANIMATION_STATE::Walking;
        currentRotation                         = ENEMY_ROTATION::Towards_Target;
        break;

    case ENEMY_MOVEMENT_TYPE::Strafe_Left_And_Move_Towards_Player:
        collisionMesh.addVelocity(
            btVector3(
                enemyController.currentSlopeIntensity.x() + forwardDelta.x,
                0,
                enemyController.currentSlopeIntensity.z() + forwardDelta.y));
        collisionMesh.addVelocity(
            btVector3(
                enemyController.currentSlopeIntensity.x() - rightDelta.x,
                0,
                enemyController.currentSlopeIntensity.z() - rightDelta.z));

        enemyController.lastKnownTargetPosition = playersPosition;
        currentMovmentState                     = ENEMY_ANIMATION_STATE::Walking;
        currentRotation                         = ENEMY_ROTATION::Towards_Target;
        break;

    case ENEMY_MOVEMENT_TYPE::Strafe_Right_And_Move_Towards_Player:
        collisionMesh.addVelocity(
            btVector3(
                enemyController.currentSlopeIntensity.x() + forwardDelta.x,
                0,
                enemyController.currentSlopeIntensity.z() + forwardDelta.y));
        collisionMesh.addVelocity(
            btVector3(
                enemyController.currentSlopeIntensity.x() + rightDelta.x,
                0,
                enemyController.currentSlopeIntensity.z() + rightDelta.z));

        enemyController.lastKnownTargetPosition = playersPosition;
        currentMovmentState                     = ENEMY_ANIMATION_STATE::Walking;
        currentRotation                         = ENEMY_ROTATION::Towards_Target;
        break;

    case ENEMY_MOVEMENT_TYPE::Walk_Away_From_Last_Known_Player_Position:

        collisionMesh.addVelocity(
            btVector3(
                enemyController.currentSlopeIntensity.x() - lastKnownForwardDelta.x,
                0,
                enemyController.currentSlopeIntensity.z() - lastKnownForwardDelta.y));

        currentMovmentState = ENEMY_ANIMATION_STATE::Walking;
        currentRotation     = ENEMY_ROTATION::Away_From_Last_Target;

        break;

    case ENEMY_MOVEMENT_TYPE::Walk_Random_Direction:

        collisionMesh.addVelocity(
            btVector3(
                enemyController.currentSlopeIntensity.x() + forwardFromRotationDelta.x,
                0,
                enemyController.currentSlopeIntensity.z() + forwardFromRotationDelta.y));

        enemyController.lastKnownTargetPosition = playersPosition;
        currentMovmentState                     = ENEMY_ANIMATION_STATE::Walking;
        currentRotation                         = ENEMY_ROTATION::Random_Changing_Direction;
        break;

    default:
        break;
    }

    enemyController.currentRotation = currentRotation;
    enemyController.currentState    = currentMovmentState;
}

void EnemyControllerSystem::handleRayHit(EnemyController& enemyController, CollisionMesh& collisionMesh, float closest) {
    collisionMesh.setVelocity(
        btVector3(
            collisionMesh.getVelocitybt().x(),
            ENEMY_CTRLR_NS::CLAMPING_VELOCITY,
            collisionMesh.getVelocitybt().z()));

    enemyController.lastTouchedPosition = glm::vec3(collisionMesh.getPosition().x, collisionMesh.getPosition().y + closest, collisionMesh.getPosition().z);

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

void EnemyControllerSystem::performJump(CollisionMesh& collisionMesh, float jumpVelocity) {
    collisionMesh.setVelocity(
        btVector3(
            collisionMesh.getVelocitybt().x(),
            jumpVelocity,
            collisionMesh.getVelocitybt().z()));
}

ControllerRayCollision EnemyControllerSystem::getLowestRayHitForAllRays(EnemyController& enemyController) {
    ControllerRayCollision collisionInfo;

    for (unsigned int i = 0; i < ENEMY_CTRLR_NS::AMOUNT_OF_RAYS; i++) {
        if (rayCast[i].hasHit()) {
            int index = getLowestRayHitIndex(rayCast[i], enemyController);

            if (index != -1) {
                float amount = rayCast[i].getLength() - rayCast[i].getHitFractionRaySpace(index);

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

int EnemyControllerSystem::getCurrentSlopeSpeedIndex(const glm::vec3& rayNormal, const EnemyController& enemyController) {
    for (int i = 0; i < enemyController.MAX_AMOUNT_SLOPE_CALCULATIONS; i++) {
        if (i + 1 < enemyController.MAX_AMOUNT_SLOPE_CALCULATIONS) {
            if ((abs(rayNormal.z) > enemyController.slopes[i].slopeAmount && abs(rayNormal.z) < enemyController.slopes[i + 1].slopeAmount) || (abs(rayNormal.x) > enemyController.slopes[i].slopeAmount && abs(rayNormal.x) < enemyController.slopes[i + 1].slopeAmount)) {
                return i;
            }
        } else {
            if ((abs(rayNormal.z) > enemyController.slopes[i].slopeAmount) || (abs(rayNormal.x) > enemyController.slopes[i].slopeAmount)) {
                return i;
            }
        }
    }

    return -1;
}

void EnemyControllerSystem::performCommands(CollisionMesh& collisionMesh, EnemyController& enemyController) {
    if (enemyController.performJump) {
        performJump(collisionMesh, enemyController.jumpVelocity);
        enemyController.jumping     = true;
        enemyController.performJump = false;
    }
}

void EnemyControllerSystem::applyForces(CollisionMesh& collisionMesh, EnemyController& enemyController) {
    collisionMesh.applyCentralImpulse(btVector3(0, enemyController.playerGravityImpulse, 0));

    collisionMesh.addVelocity(
        glm::vec3(
            collisionMesh.getVelocitybt().x() * -enemyController.getResistance(),
            0,
            collisionMesh.getVelocitybt().z() * -enemyController.getResistance()));
}

void EnemyControllerSystem::executeRayTesting(EnemyController& enemyController, CollisionMesh& collisionMesh, const Transform& meshTransform, PhysicsWorld& world) {
    float rayDistance = enemyController.colliderWidthAndDepth + ENEMY_CTRLR_NS::RAY_DISTANCE_CORRECTOR;

    rayCast[0].rayTest(*world.getWorld(), glm::vec3(meshTransform.position.x - rayDistance, meshTransform.position.y + enemyController.halfLengthOfRay, meshTransform.position.z - rayDistance), glm::vec3(meshTransform.position.x - rayDistance, meshTransform.position.y - enemyController.halfLengthOfRay, meshTransform.position.z - rayDistance));
    rayCast[1].rayTest(*world.getWorld(), glm::vec3(meshTransform.position.x - rayDistance, meshTransform.position.y + enemyController.halfLengthOfRay, meshTransform.position.z + rayDistance), glm::vec3(meshTransform.position.x - rayDistance, meshTransform.position.y - enemyController.halfLengthOfRay, meshTransform.position.z + rayDistance));
    rayCast[2].rayTest(*world.getWorld(), glm::vec3(meshTransform.position.x + rayDistance, meshTransform.position.y + enemyController.halfLengthOfRay, meshTransform.position.z + rayDistance), glm::vec3(meshTransform.position.x + rayDistance, meshTransform.position.y - enemyController.halfLengthOfRay, meshTransform.position.z + rayDistance));
    rayCast[3].rayTest(*world.getWorld(), glm::vec3(meshTransform.position.x + rayDistance, meshTransform.position.y + enemyController.halfLengthOfRay, meshTransform.position.z - rayDistance), glm::vec3(meshTransform.position.x + rayDistance, meshTransform.position.y - enemyController.halfLengthOfRay, meshTransform.position.z - rayDistance));

    ControllerRayCollision collision = getLowestRayHitForAllRays(enemyController);

    if (collision != ControllerRayCollision::INVALID_COLLISION) {

        handleRayHit(enemyController, collisionMesh, collision.lowestPointValue);

        if (collision.lowestPointIndex < rayCast[collision.rayIndex].size()) {

            enemyController.rayNormal = hh::toGlmVec3(rayCast[collision.rayIndex].getHitNormal(collision.lowestPointIndex));

            enemyController.currentSlopeIntensity = btVector3(0, 0, 0);

            int index = getCurrentSlopeSpeedIndex(enemyController.rayNormal, enemyController);

            if (index != -1) {
                glm::vec3 slopeSpeed = getSlopeSpeed(enemyController.slopes[index].slopeGravity, enemyController.rayNormal);

                enemyController.currentSlopeIntensity = btVector3(slopeSpeed.x, 0, slopeSpeed.z);
            }

            collisionMesh.setVelocity(enemyController.currentSlopeIntensity + collisionMesh.getVelocitybt());
        }
    } else if (enemyController.isTouchingGround) {
        enemyController.isTouchingGround = false;
    }
}

void EnemyControllerSystem::applyNewTransform(CollisionMesh& mesh, const glm::vec3& playersPosition, EnemyController& enemyController, Transform& oldTransform, const TestEnemyAI& ai) {

    glm::vec3 forward     = mesh.getPosition() - playersPosition;
    glm::vec3 lastForward = mesh.getPosition() - enemyController.lastKnownTargetPosition;

    switch (enemyController.currentRotation) {

    case ENEMY_ROTATION::Away_From_Target:
        oldTransform.rotation
            = glm::quat(
                glm::vec3(
                    glm::radians(-90.0f),
                    -hh::signedRadiansBetweenVectors(glm::vec3(forward.x, 0, forward.z), glm::vec3(0, 0, 1)),
                    0));
        break;

    case ENEMY_ROTATION::Away_From_Last_Target:
        oldTransform.rotation
            = glm::quat(
                glm::vec3(
                    glm::radians(-90.0f),
                    -hh::signedRadiansBetweenVectors(glm::vec3(lastForward.x, 0, lastForward.z), glm::vec3(0, 0, 1)),
                    0));
        break;

    case ENEMY_ROTATION::Towards_Target:
        oldTransform.rotation
            = glm::quat(
                glm::vec3(
                    glm::radians(-90.0f),
                    -hh::signedRadiansBetweenVectors(glm::vec3(forward.x, 0, forward.z), glm::vec3(0, 0, -1)),
                    0));
        break;

    case ENEMY_ROTATION::Random_Changing_Direction:

        if (enemyController.changeDirectionCooldown <= 0.0f) {
            enemyController.changeDirectionCooldown = ai.getChangeInDirectionForEveryXSeconds();
            enemyController.targetRotation          = glm::radians(hh::getRandomRange(0.0f, 360.0f));
        } else {
            glm::quat newRot = glm::quat(
                glm::vec3(
                    glm::radians(-90.0f),
                    enemyController.targetRotation,
                    0));

            oldTransform.rotation = glm::slerp(oldTransform.rotation, newRot, GameInfo::fixedDeltaTime * 3.0f);

            enemyController.changeDirectionCooldown -= GameInfo::fixedDeltaTime;
        }

        break;

    case ENEMY_ROTATION::Unknown:
        break;
    default:
        break;
    }

    oldTransform.position = mesh.getPosition() + enemyController.offsetFromCollider;
}

void EnemyControllerSystem::update(Input& input, EntityTransform& modelsT, CollisionMesh& mesh, PhysicsWorld& world, const GlobalInformation& globalInformation, EnemyController& enemyController, const TestEnemyAI& ai) {

    Transform& modelsTransform = modelsT.transform;

    mesh.activateRigidBody(true);

    //Perform Calculations
    executeRayTesting(enemyController, mesh, mesh.getTransformation(), world);
    ///////////////////////

    ////////////////Perform Actions
    calculateMovment(input, modelsTransform.rotation, globalInformation.getPlayersPosition(), mesh, enemyController, ai);
    performCommands(mesh, enemyController);
    ////////////////

    //////////////Perform automatics
    //Apply gravity
    applyForces(mesh, enemyController);
    /////////////////

    /////////////////Transform Model Orientation according to collision mesh and camera.
    applyNewTransform(mesh, globalInformation.getPlayersPosition(), enemyController, modelsTransform, ai);
    ///////////////////
}
