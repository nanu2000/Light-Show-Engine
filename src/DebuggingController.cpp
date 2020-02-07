#include "DebuggingController.h"

void DebuggingController::controlPhysicsDebugDraw(Input& inputHandler, PhysicsWorld& world) {
    if (inputHandler.keyPressedOnce(PHYSICS_DRAW_TOGGLE)) {
        if (world.isDebugDrawing() == false) {
            DBG_LOG("Debug Draw Is Now On.\n");
            world.setDebugDrawing(true);
        } else {
            DBG_LOG("Debug Draw Is Now Off.\n");
            world.setDebugDrawing(false);
        }
    }
}

void DebuggingController::executeDebugRendering(PhysicsWorld& world, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    if (world.isDebugDrawing()) {
        world.debugDraw(projectionMatrix, viewMatrix);
    }
}
