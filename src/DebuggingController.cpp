#include "DebuggingController.h"

void DebuggingController::controlPhysicsDebugDraw(Input& inputHandler, PhysicsWorld& world) {
    if (inputHandler.keyPressedOnce(PHYSICS_DRAW_TOGGLE)) {
        if (isDebugDrawOn == false) {
            DBG_LOG("Debug Draw Is Now On.\n");
            world.initializeDebugDraw();
            isDebugDrawOn = true;
        } else {
            DBG_LOG("Debug Draw Is Now Off.\n");
            isDebugDrawOn = false;
        }
    }
}

void DebuggingController::executeDebugRendering(PhysicsWorld& world, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    if (isDebugDrawOn) {
        world.debugDraw(projectionMatrix, viewMatrix);
    }
}

void DebuggingController::controlWireframeDebugDraw(Input& inputHandler) {
    if (inputHandler.keyPressedOnce(WIREFRAME_DRAW_TOGGLE)) {
        isWireframeOn = !isWireframeOn;
        if (isWireframeOn) {
            DBG_LOG("Wireframe mode is now turned on\n");
            glDisable(GL_BLEND);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            DBG_LOG("Wireframe mode is now turned off\n");
            glEnable(GL_BLEND);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
}
