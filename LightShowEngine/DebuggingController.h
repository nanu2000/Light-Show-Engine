#ifndef DEBUGGING_CONTROLLER_H
#define DEBUGGING_CONTROLLER_H

#include "Input.h"
#include "PhysicsWorld.h"
#include "Component.h"



class DebuggingController : public Component<DebuggingController>
{
public:

	const SDL_Keycode PHYSICS_DRAW_TOGGLE = SDLK_F1;
	const SDL_Keycode WIREFRAME_DRAW_TOGGLE = SDLK_F2;

	void controlPhysicsDebugDraw	(Input& inputHandler, PhysicsWorld & world);

	void executeDebugRendering		(PhysicsWorld & world, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

	void controlWireframeDebugDraw(Input& inputHandler);

private:

	bool isDebugDrawOn = false;
	bool isWireframeOn = false;

};


#endif
