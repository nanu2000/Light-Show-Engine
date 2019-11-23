#include "PlayerControllerSystem.h"

void PlayerControllerSystem::getPlayerInput(Input& input, CollisionMesh& collisionMesh, PlayerController& playerController, Camera& camera, const UserControls& userControls) {
    glm::vec2 normalizedForwardXZ = glm::normalize(glm::vec2(camera.getForward().x, camera.getForward().z));
    glm::vec2 forwardDelta        = normalizedForwardXZ * playerController.speed;
    glm::vec3 rightDelta          = camera.getRight() * playerController.speed;

    PLAYER_ANIMATION_STATE currentMovmentState = PLAYER_ANIMATION_STATE::Idle;

    if (input.keyPressedOnce(userControls.getPlayerViewKey())) {
        playerController.rotateAwayFromCamera = !playerController.rotateAwayFromCamera;
    }

    if (input.isKeyDown(userControls.getPlayerLeftKey())) {
        collisionMesh.addVelocity(
            btVector3(
                playerController.currentSlopeIntensity.x() - rightDelta.x,
                0,
                playerController.currentSlopeIntensity.z() - rightDelta.z));
        currentMovmentState = PLAYER_ANIMATION_STATE::Walking;
    }

    if (input.isKeyDown(userControls.getPlayerBackwardsKey())) {
        collisionMesh.addVelocity(
            btVector3(
                playerController.currentSlopeIntensity.x() - forwardDelta.x,
                0,
                playerController.currentSlopeIntensity.z() - forwardDelta.y));
        currentMovmentState = PLAYER_ANIMATION_STATE::Walking;
    }

    if (input.isKeyDown(userControls.getPlayerRightKey())) {
        collisionMesh.addVelocity(
            btVector3(
                playerController.currentSlopeIntensity.x() + rightDelta.x,
                0,
                playerController.currentSlopeIntensity.z() + rightDelta.z));
        currentMovmentState = PLAYER_ANIMATION_STATE::Walking;
    }

    if (input.isKeyDown(userControls.getPlayerForwardKey())) {
        collisionMesh.addVelocity(
            btVector3(
                playerController.currentSlopeIntensity.x() + forwardDelta.x,
                0,
                playerController.currentSlopeIntensity.z() + forwardDelta.y));
        currentMovmentState = PLAYER_ANIMATION_STATE::Walking;
    }

    if (playerController.canJump(collisionMesh.getPosition())) {
        playerController.performJump = input.isKeyDown(userControls.getPlayerJumpKey());
    }

    if (playerController.jumping) {
        //currentMovmentState = PLAYER_ANIMATION_STATE::Jumping;
    }

    playerController.currentState = currentMovmentState;
}

void PlayerControllerSystem::handleRayHit(PlayerController& playerController, CollisionMesh& collisionMesh, float closest) {
    collisionMesh.setVelocity(
        btVector3(
            collisionMesh.getVelocitybt().x(),
            PLR_CTRLR_NS::CLAMPING_VELOCITY,
            collisionMesh.getVelocitybt().z()));

    playerController.lastTouchedPosition = glm::vec3(collisionMesh.getPosition().x, collisionMesh.getPosition().y + closest, collisionMesh.getPosition().z);

    collisionMesh.translate(btVector3(0, closest, 0));

    playerController.isTouchingGround = true;
    playerController.jumping          = false;
}

int PlayerControllerSystem::getLowestRayHitIndex(const RayCaster& rayResults, PlayerController& playerController) {
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

void PlayerControllerSystem::performJump(CollisionMesh& collisionMesh, float jumpVelocity) {
    collisionMesh.applyCentralImpulse(
        btVector3(0,
                  jumpVelocity,
                  0));
}

ControllerRayCollision PlayerControllerSystem::getLowestRayHitForAllRays(PlayerController& playerController) {
    ControllerRayCollision collisionInfo;

    for (unsigned int i = 0; i < PLR_CTRLR_NS::AMOUNT_OF_RAYS; i++) {
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

int PlayerControllerSystem::getCurrentSlopeSpeedIndex(const glm::vec3& rayNormal, const PlayerController& playerController) {
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

void PlayerControllerSystem::performCommands(CollisionMesh& collisionMesh, PlayerController& playerController) {
    if (playerController.performJump) {
        performJump(collisionMesh, playerController.jumpVelocity);
        playerController.jumping     = true;
        playerController.performJump = false;
    }
}

void PlayerControllerSystem::applyForces(CollisionMesh& collisionMesh, PlayerController& playerController) {
    collisionMesh.applyCentralImpulse(btVector3(0, playerController.playerGravityImpulse, 0));

    collisionMesh.addVelocity(
        glm::vec3(
            collisionMesh.getVelocitybt().x() * -playerController.getResistance(),
            0,
            collisionMesh.getVelocitybt().z() * -playerController.getResistance()));
}

void PlayerControllerSystem::executeRayTesting(PlayerController& playerController, CollisionMesh& collisionMesh, const Transform& meshTransform, PhysicsWorld& world) {
    float rayDistance = playerController.colliderWidthAndDepth + PLR_CTRLR_NS::RAY_DISTANCE_CORRECTOR;

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

void PlayerControllerSystem::applyNewTransform(CollisionMesh& mesh, const glm::vec3& cameraForward, const PlayerController& playerController, Transform& currentTransform) {

    currentTransform.position = mesh.getPosition() + playerController.offsetFromCollider;
}

void PlayerControllerSystem::applyNewRotation(const glm::vec3& cameraForward, const PlayerController& playerController, Transform& currentTransform) {
    if (playerController.rotateAwayFromCamera == false) {

        currentTransform.rotation
            = glm::quat(
                glm::vec3(
                    hh::signedRadiansBetweenVectors(glm::vec3(cameraForward.x, 0, cameraForward.z), glm::vec3(1, 0, 0)),
                    0,
                    glm::radians(-90.f)));
    }
}

void PlayerControllerSystem::fixedUpdate(Input& input, Transform& modelsTransform, CollisionMesh& mesh, PhysicsWorld& world, PlayerController& playerController, Camera& camera, const UserControls& userControls) {

    mesh.activateRigidBody(true);

    //Perform Calculations
    executeRayTesting(playerController, mesh, mesh.getTransformation(), world);
    ///////////////////////

    ////////////////Perform Actions
    getPlayerInput(input, mesh, playerController, camera, userControls);
    performCommands(mesh, playerController);
    ////////////////

    //////////////Perform automatics
    //Apply gravity
    applyForces(mesh, playerController);
    /////////////////

    /////////////////Transform Model Orientation according to collision mesh and camera.
    applyNewTransform(mesh, camera.getForward(), playerController, modelsTransform);
    ///////////////////
}

void PlayerControllerSystem::update(Transform& modelsTransform, PlayerController& playerController, Camera& camera, CollisionMesh& mesh) {

    //Normally we would update model position here, however doing this:
    //btTransform b;
    //mesh.getMotionState()->getWorldTransform(b);
    //modelsTransform.position = hh::toGlmVec3(b.getOrigin()) + playerController.offsetFromCollider;
    //will result in lag. https://gamedev.stackexchange.com/questions/140865/graphical-mesh-lags-behind-collision-shape-in-bulletphysics-debug-drawing

    /////////////////Transform Model Orientation according to collision mesh and camera.
    applyNewRotation(camera.getForward(), playerController, modelsTransform);
    ///////////////////
}
