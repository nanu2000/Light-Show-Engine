#include "EnemyControllerSystem.h"

void EnemyControllerSystem::calculateMovment(Input& input, const glm::quat& rotation, const glm::vec3& playersPosition, CollisionMesh& collisionMesh, EnemyController& playerController, const UserControls& userControls, const TestEnemyAI& ai) {

    glm::vec3 forwardFromRotation = glm::normalize(rotation * glm::vec3(0, 1, 0));

    forwardFromRotation = glm::vec3(-forwardFromRotation.x, 0, -forwardFromRotation.z);

    glm::vec2 forwardFromRotationDelta = glm::vec2(forwardFromRotation.x, forwardFromRotation.z) * playerController.speed;

    glm::vec3 forward          = -glm::normalize(collisionMesh.getPosition() - playersPosition);
    glm::vec3 lastKnownForward = -glm::normalize(collisionMesh.getPosition() - playerController.lastKnownTargetPosition);

    glm::vec3 right = glm::cross(forward, glm::vec3(0, 1, 0));

    glm::vec2 lastKnownForwardDelta = glm::vec2(lastKnownForward.x, lastKnownForward.z) * playerController.speed;
    glm::vec2 forwardDelta          = glm::vec2(forward.x, forward.z) * playerController.speed;

    glm::vec3 rightDelta = right * playerController.speed;

    ENEMY_ANIMATION_STATE currentMovmentState = ENEMY_ANIMATION_STATE::Idle;
    ENEMY_ROTATION currentRotation            = ENEMY_ROTATION::Unknown;

    switch (ai.getMovementType()) {

    case ENEMY_MOVEMENT_TYPE::Idle_Facing_Player:

        playerController.lastKnownTargetPosition = playersPosition;
        currentRotation                          = ENEMY_ROTATION::Towards_Target;
        break;

    case ENEMY_MOVEMENT_TYPE::Idle:

        playerController.lastKnownTargetPosition = playersPosition;
        currentRotation                          = ENEMY_ROTATION::Unknown;
        break;

    case ENEMY_MOVEMENT_TYPE::Walk_Away_From_Player:
        collisionMesh.addVelocity(
            btVector3(
                playerController.currentSlopeIntensity.x() - forwardDelta.x,
                0,
                playerController.currentSlopeIntensity.z() - forwardDelta.y));

        playerController.lastKnownTargetPosition = playersPosition;
        currentMovmentState                      = ENEMY_ANIMATION_STATE::Walking;
        currentRotation                          = ENEMY_ROTATION::Away_From_Target;
        break;

    case ENEMY_MOVEMENT_TYPE::Walk_Towards_Player:
        collisionMesh.addVelocity(
            btVector3(
                playerController.currentSlopeIntensity.x() + forwardDelta.x,
                0,
                playerController.currentSlopeIntensity.z() + forwardDelta.y));

        playerController.lastKnownTargetPosition = playersPosition;
        currentMovmentState                      = ENEMY_ANIMATION_STATE::Walking;
        currentRotation                          = ENEMY_ROTATION::Towards_Target;

        break;

    case ENEMY_MOVEMENT_TYPE::Strafe_Left_From_Player:
        collisionMesh.addVelocity(
            btVector3(
                playerController.currentSlopeIntensity.x() - rightDelta.x,
                0,
                playerController.currentSlopeIntensity.z() - rightDelta.z));

        playerController.lastKnownTargetPosition = playersPosition;
        currentMovmentState                      = ENEMY_ANIMATION_STATE::Walking;
        currentRotation                          = ENEMY_ROTATION::Towards_Target;
        break;

    case ENEMY_MOVEMENT_TYPE::Strafe_Right_From_Player:
        collisionMesh.addVelocity(
            btVector3(
                playerController.currentSlopeIntensity.x() + rightDelta.x,
                0,
                playerController.currentSlopeIntensity.z() + rightDelta.z));

        playerController.lastKnownTargetPosition = playersPosition;
        currentMovmentState                      = ENEMY_ANIMATION_STATE::Walking;
        currentRotation                          = ENEMY_ROTATION::Towards_Target;
        break;

    case ENEMY_MOVEMENT_TYPE::Strafe_Left_And_Move_Towards_Player:
        collisionMesh.addVelocity(
            btVector3(
                playerController.currentSlopeIntensity.x() + forwardDelta.x,
                0,
                playerController.currentSlopeIntensity.z() + forwardDelta.y));
        collisionMesh.addVelocity(
            btVector3(
                playerController.currentSlopeIntensity.x() - rightDelta.x,
                0,
                playerController.currentSlopeIntensity.z() - rightDelta.z));

        playerController.lastKnownTargetPosition = playersPosition;
        currentMovmentState                      = ENEMY_ANIMATION_STATE::Walking;
        currentRotation                          = ENEMY_ROTATION::Towards_Target;
        break;

    case ENEMY_MOVEMENT_TYPE::Strafe_Right_And_Move_Towards_Player:
        collisionMesh.addVelocity(
            btVector3(
                playerController.currentSlopeIntensity.x() + forwardDelta.x,
                0,
                playerController.currentSlopeIntensity.z() + forwardDelta.y));
        collisionMesh.addVelocity(
            btVector3(
                playerController.currentSlopeIntensity.x() + rightDelta.x,
                0,
                playerController.currentSlopeIntensity.z() + rightDelta.z));

        playerController.lastKnownTargetPosition = playersPosition;
        currentMovmentState                      = ENEMY_ANIMATION_STATE::Walking;
        currentRotation                          = ENEMY_ROTATION::Towards_Target;
        break;

    case ENEMY_MOVEMENT_TYPE::Walk_Away_From_Last_Known_Player_Position:

        collisionMesh.addVelocity(
            btVector3(
                playerController.currentSlopeIntensity.x() - lastKnownForwardDelta.x,
                0,
                playerController.currentSlopeIntensity.z() - lastKnownForwardDelta.y));

        currentMovmentState = ENEMY_ANIMATION_STATE::Walking;
        currentRotation     = ENEMY_ROTATION::Away_From_Last_Target;

        break;

    case ENEMY_MOVEMENT_TYPE::Walk_Random_Direction:

        collisionMesh.addVelocity(
            btVector3(
                playerController.currentSlopeIntensity.x() + forwardFromRotationDelta.x,
                0,
                playerController.currentSlopeIntensity.z() + forwardFromRotationDelta.y));

        playerController.lastKnownTargetPosition = playersPosition;
        currentMovmentState                      = ENEMY_ANIMATION_STATE::Walking;
        currentRotation                          = ENEMY_ROTATION::Random_Changing_Direction;
        break;

    default:
        break;
    }

    //if (playerController.canJump(collisionMesh.getPosition()))
    //{
    //	playerController.performJump = input.isKeyDown(userControls.getPlayerJumpButton());
    //}

    //if (playerController.jumping)
    //{
    //	currentMovmentState = ENEMY_ANIMATION_STATE::Jumping;
    //}

    playerController.currentRotation = currentRotation;
    playerController.currentState    = currentMovmentState;
}

void EnemyControllerSystem::handleRayHit(EnemyController& playerController, CollisionMesh& collisionMesh, float closest) {
    collisionMesh.setVelocity(
        btVector3(
            collisionMesh.getVelocitybt().x(),
            ENEMY_CTRLR_NS::CLAMPING_VELOCITY,
            collisionMesh.getVelocitybt().z()));

    playerController.lastTouchedPosition = glm::vec3(collisionMesh.getPosition().x, collisionMesh.getPosition().y + closest, collisionMesh.getPosition().z);

    collisionMesh.translate(btVector3(0, closest, 0));

    playerController.isTouchingGround = true;
    playerController.jumping          = false;
}

int EnemyControllerSystem::getLowestRayHitIndex(const RayCaster& rayResults, EnemyController& playerController) {
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

ControllerRayCollision EnemyControllerSystem::getLowestRayHitForAllRays(EnemyController& playerController) {
    ControllerRayCollision collisionInfo;

    for (unsigned int i = 0; i < ENEMY_CTRLR_NS::AMOUNT_OF_RAYS; i++) {
        if (rayCast[i].hasHit()) {
            int index = getLowestRayHitIndex(rayCast[i], playerController);

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

int EnemyControllerSystem::getCurrentSlopeSpeedIndex(const glm::vec3& rayNormal, const EnemyController& playerController) {
    for (int i = 0; i < playerController.MAX_AMOUNT_SLOPE_CALCULATIONS; i++) {
        if (i + 1 < playerController.MAX_AMOUNT_SLOPE_CALCULATIONS) {
            if ((abs(rayNormal.z) > playerController.slopes[i].slopeAmount && abs(rayNormal.z) < playerController.slopes[i + 1].slopeAmount) || (abs(rayNormal.x) > playerController.slopes[i].slopeAmount && abs(rayNormal.x) < playerController.slopes[i + 1].slopeAmount)) {
                return i;
            }
        } else {
            if ((abs(rayNormal.z) > playerController.slopes[i].slopeAmount) || (abs(rayNormal.x) > playerController.slopes[i].slopeAmount)) {
                return i;
            }
        }
    }

    return -1;
}

void EnemyControllerSystem::performCommands(CollisionMesh& collisionMesh, EnemyController& playerController) {
    if (playerController.performJump) {
        performJump(collisionMesh, playerController.jumpVelocity);
        playerController.jumping     = true;
        playerController.performJump = false;
    }
}

void EnemyControllerSystem::applyForces(CollisionMesh& collisionMesh, EnemyController& playerController) {
    collisionMesh.applyCentralImpulse(btVector3(0, playerController.playerGravityImpulse, 0));

    collisionMesh.addVelocity(
        glm::vec3(
            collisionMesh.getVelocitybt().x() * -playerController.getResistance(),
            0,
            collisionMesh.getVelocitybt().z() * -playerController.getResistance()));
}

void EnemyControllerSystem::executeRayTesting(EnemyController& playerController, CollisionMesh& collisionMesh, const Transform& meshTransform, PhysicsWorld& world) {
    float rayDistance = playerController.colliderWidthAndDepth + ENEMY_CTRLR_NS::RAY_DISTANCE_CORRECTOR;

    rayCast[0].rayTest(*world.getWorld(), glm::vec3(meshTransform.position.x - rayDistance, meshTransform.position.y + playerController.halfLengthOfRay, meshTransform.position.z - rayDistance), glm::vec3(meshTransform.position.x - rayDistance, meshTransform.position.y - playerController.halfLengthOfRay, meshTransform.position.z - rayDistance));
    rayCast[1].rayTest(*world.getWorld(), glm::vec3(meshTransform.position.x - rayDistance, meshTransform.position.y + playerController.halfLengthOfRay, meshTransform.position.z + rayDistance), glm::vec3(meshTransform.position.x - rayDistance, meshTransform.position.y - playerController.halfLengthOfRay, meshTransform.position.z + rayDistance));
    rayCast[2].rayTest(*world.getWorld(), glm::vec3(meshTransform.position.x + rayDistance, meshTransform.position.y + playerController.halfLengthOfRay, meshTransform.position.z + rayDistance), glm::vec3(meshTransform.position.x + rayDistance, meshTransform.position.y - playerController.halfLengthOfRay, meshTransform.position.z + rayDistance));
    rayCast[3].rayTest(*world.getWorld(), glm::vec3(meshTransform.position.x + rayDistance, meshTransform.position.y + playerController.halfLengthOfRay, meshTransform.position.z - rayDistance), glm::vec3(meshTransform.position.x + rayDistance, meshTransform.position.y - playerController.halfLengthOfRay, meshTransform.position.z - rayDistance));

    ControllerRayCollision collision = getLowestRayHitForAllRays(playerController);

    if (collision != ControllerRayCollision::INVALID_COLLISION) {

        handleRayHit(playerController, collisionMesh, collision.lowestPointValue);

        if (collision.lowestPointIndex < rayCast[collision.rayIndex].size()) {

            playerController.rayNormal = hh::toGlmVec3(rayCast[collision.rayIndex].getHitNormal(collision.lowestPointIndex));

            playerController.currentSlopeIntensity = btVector3(0, 0, 0);

            int index = getCurrentSlopeSpeedIndex(playerController.rayNormal, playerController);

            if (index != -1) {
                glm::vec3 slopeSpeed = getSlopeSpeed(playerController.slopes[index].slopeGravity, playerController.rayNormal);

                playerController.currentSlopeIntensity = btVector3(slopeSpeed.x, 0, slopeSpeed.z);
            }

            collisionMesh.setVelocity(playerController.currentSlopeIntensity + collisionMesh.getVelocitybt());
        }
    } else if (playerController.isTouchingGround) {
        playerController.isTouchingGround = false;
    }
}

void EnemyControllerSystem::applyNewTransform(CollisionMesh& mesh, const glm::vec3& playersPosition, EnemyController& playerController, Transform& oldTransform, const TestEnemyAI& ai) {

    glm::vec3 forward     = mesh.getPosition() - playersPosition;
    glm::vec3 lastForward = mesh.getPosition() - playerController.lastKnownTargetPosition;

    switch (playerController.currentRotation) {

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

        if (playerController.changeDirectionCooldown <= 0.0f) {
            playerController.changeDirectionCooldown = ai.getChangeInDirectionForEveryXSeconds();
            playerController.targetRotation          = glm::radians(hh::getRandomRange(0.0f, 360.0f));
        } else {
            glm::quat newRot = glm::quat(
                glm::vec3(
                    glm::radians(-90.0f),
                    playerController.targetRotation,
                    0));

            oldTransform.rotation = glm::slerp(oldTransform.rotation, newRot, GameInfo::deltaTime * 3.0f);

            playerController.changeDirectionCooldown -= GameInfo::deltaTime;
        }

        break;

    case ENEMY_ROTATION::Unknown:
        break;
    default:
        break;
    }

    oldTransform.position = mesh.getPosition() + playerController.offsetFromCollider;
}

void EnemyControllerSystem::update(Input& input, EntityTransform& modelsT, CollisionMesh& mesh, PhysicsWorld& world, const GlobalInformation& globalInformation, EnemyController& playerController, const UserControls& userControls, const TestEnemyAI& ai) {

    Transform& modelsTransform = modelsT.transform;

    mesh.activateRigidBody(true);

    //Perform Calculations
    executeRayTesting(playerController, mesh, mesh.getTransformation(), world);
    ///////////////////////

    ////////////////Perform Actions
    calculateMovment(input, modelsTransform.rotation, globalInformation.getPlayersPosition(), mesh, playerController, userControls, ai);
    performCommands(mesh, playerController);
    ////////////////

    //////////////Perform automatics
    //Apply gravity
    applyForces(mesh, playerController);
    /////////////////

    /////////////////Transform Model Orientation according to collision mesh and camera.
    applyNewTransform(mesh, globalInformation.getPlayersPosition(), playerController, modelsTransform, ai);
    ///////////////////
}
