#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H

#include "Bullet/btBulletDynamicsCommon.h"
#include <Bullet/LinearMath/btIDebugDraw.h>
#include "DebugDrawer.h"
#include "CollisionTag.h"
#include "CollisionMesh.h"

class PhysicsWorld
{

public:

	PhysicsWorld(const btVector3& gravity);

	btDiscreteDynamicsWorld* getWorld() const {	return thisWorld; }

	~PhysicsWorld();

	void update();

	void setDebugMode(btIDebugDraw::DebugDrawModes mode);

	void debugDraw(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);

	void initializeDebugDraw();

	DebugDrawer * const getDebugDrawer() { return &debugDrawer; }

	void addRigidBody(const CollisionMesh & collisionMesh);

	void removeRigidBody(const CollisionMesh & collisionMesh);

	void removeAllRigidBodies();

private:


	std::vector<btRigidBody*> rigidBodies;

	DebugDrawer								debugDrawer;
	btBroadphaseInterface				*	broadphase				= nullptr;
	btDefaultCollisionConfiguration		*	collisionConfiguration	= nullptr;
	btCollisionDispatcher				*	dispatcher				= nullptr;
	btSequentialImpulseConstraintSolver	*	solver					= nullptr;
	btDiscreteDynamicsWorld				*	thisWorld				= nullptr;
	
};


#endif // !PHYSICS_WORLD_H
