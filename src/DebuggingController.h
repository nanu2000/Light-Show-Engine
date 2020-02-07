#ifndef DEBUGGING_CONTROLLER_H
#define DEBUGGING_CONTROLLER_H

#include "Component.h"
#include "Input.h"
#include "PhysicsWorld.h"

class DebuggingController : public Component<DebuggingController> {
public:
    static const SDL_Keycode PHYSICS_DRAW_TOGGLE = SDLK_F1;

    void controlPhysicsDebugDraw(Input& inputHandler, PhysicsWorld& world);

    void executeDebugRendering(PhysicsWorld& world, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
};

#endif
