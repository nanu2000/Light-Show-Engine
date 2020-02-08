#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H

#include "CollisionMesh.h"
#include "CollisionTag.h"
#include "DebugDrawer.h"
#include "btBulletDynamicsCommon.h"
#include <LinearMath/btIDebugDraw.h>

class PhysicsWorld {

public:
    PhysicsWorld(const btVector3& gravity);

    btDiscreteDynamicsWorld* getWorld() const { return thisWorld; }

    ~PhysicsWorld();

    void initialize();

    void fixedUpdate();

    void setDebugMode(btIDebugDraw::DebugDrawModes mode);

    void debugDraw(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);

    void addRigidBody(const CollisionMesh& collisionMesh);

    void removeRigidBody(const CollisionMesh& collisionMesh);

    void removeAllRigidBodies();

    void setDebugDrawing(bool b) {
        shouldDebugDraw = b;
    }
    bool isDebugDrawing() {
        return shouldDebugDraw;
    }

    Engine::DebugDrawer& getDebugDrawer() {
        return debugDrawer;
    }

private:
    void initializeDebugDraw();

    bool shouldDebugDraw = false;

    std::vector<btRigidBody*> rigidBodies;
    std::vector<btCollisionShape*> shapes;

    Engine::DebugDrawer debugDrawer;
    btBroadphaseInterface* broadphase                       = nullptr;
    btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
    btCollisionDispatcher* dispatcher                       = nullptr;
    btSequentialImpulseConstraintSolver* solver             = nullptr;
    btDiscreteDynamicsWorld* thisWorld                      = nullptr;
};

#endif // !PHYSICS_WORLD_H
