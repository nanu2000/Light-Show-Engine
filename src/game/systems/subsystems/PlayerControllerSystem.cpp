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
            playerController.clampingVelocity,
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

    for (unsigned int i = 0; i < PlayerController::AMOUNT_OF_RAYS; i++) {
        if (playerController.rayCast[i].hasHit()) {
            int index = getLowestRayHitIndex(playerController.rayCast[i], playerController);

            if (index != -1) {
                float amount = playerController.rayCast[i].getLength() - playerController.rayCast[i].getHitFractionRaySpace(index);

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
    float rayDistance = playerController.colliderWidthAndDepth + playerController.rayDistanceCorrector;

    //We want to have an equal space per ray. The rays are drawn in a circle so we calculate the radians per space each ray should have.
    float spacePerRays = glm::radians(360.f / PlayerController::AMOUNT_OF_RAYS);
    float radius       = rayDistance;

    /*
    formula:
    x = originX + cos(angle) * radius;
    y = originY + sin(angle) * radius;
    */

    for (unsigned int i = 0; i < PlayerController::AMOUNT_OF_RAYS; i++) {
        glm::vec3 position = glm::vec3(
            meshTransform.position.x + glm::cos(spacePerRays * (i + 1)) * radius,
            meshTransform.position.y,
            meshTransform.position.z + glm::sin(spacePerRays * (i + 1)) * radius);

        glm::vec3 from = glm::vec3(position.x, position.y + playerController.halfLengthOfRay, position.z);
        glm::vec3 to   = glm::vec3(position.x, position.y - playerController.halfLengthOfRay, position.z);

        playerController.rayCast[i].rayTest(*world.getWorld(), from, to);
    }

    float amountAverage = 0;
    int itr             = 0;
    for (unsigned int i = 0; i < PlayerController::AMOUNT_OF_RAYS; i++) {
        int index = getLowestRayHitIndex(playerController.rayCast[i], playerController);

        if (index != -1) {
            float amount = playerController.rayCast[i].getLength() - playerController.rayCast[i].getHitFractionRaySpace(index);
            amountAverage += amount;
            itr++;
        }
    }
    if (amountAverage != 0 && itr != 0) {
        amountAverage /= itr;
    }

    ControllerRayCollision collision = getLowestRayHitForAllRays(playerController);

    static bool useAverage = true;
    if (InputLocator::getService().keyPressedOnce(SDLK_5)) {
        useAverage = !useAverage;
        DBG_LOG("Using %s\n", useAverage ? "average" : "highest point");
    }
    if (!useAverage) {
        amountAverage = collision.lowestPointValue;
    }

    if (collision != ControllerRayCollision::INVALID_COLLISION) {

        handleRayHit(playerController, collisionMesh, amountAverage);

        if (collision.lowestPointIndex < playerController.rayCast[collision.rayIndex].size()) {

            playerController.rayNormal = hh::toGlmVec3(playerController.rayCast[collision.rayIndex].getHitNormal(collision.lowestPointIndex));

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

void PlayerControllerSystem::debugRender(PhysicsWorld& w, PlayerController& p) {

    if (!w.isDebugDrawing()) {
        return;
    }

    for (int i = 0; i < PlayerController::AMOUNT_OF_RAYS; i++) {

        w.getDebugDrawer().drawLine(p.rayCast[i].getRayFrom(), p.rayCast[i].getRayTo(), btVector3(0, 0, 0));
    }
}
