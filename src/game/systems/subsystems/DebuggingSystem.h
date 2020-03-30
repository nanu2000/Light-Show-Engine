#ifndef DEBUGGING_SYSTEM_H
#define DEBUGGING_SYSTEM_H

#include "Input.h"
#include "PhysicsWorld.h"
#include "SystemBase.h"

//!Controls the PhysicsWorld DebugDrawer.
class DebuggingSystem : public SystemBase {
public:
    //!Updates the DebugDrawer- such as toggling if it should be rendering or not. Should be called in FixedUpdatingSystem
    void controlPhysicsDebugDraw(Input& inputHandler, PhysicsWorld& world);

    //!Renders the DebugDrawer if it is set to render. Should be called in RenderingSystem
    void executeDebugRendering(PhysicsWorld& world, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
};
#endif
