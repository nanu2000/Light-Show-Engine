#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld(const btVector3& gravity) {
    broadphase = new btDbvtBroadphase();

    collisionConfiguration = new btDefaultCollisionConfiguration();

    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    solver = new btSequentialImpulseConstraintSolver;

    thisWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    thisWorld->setGravity(gravity);
}

PhysicsWorld::~PhysicsWorld() {
    DBG_CHECK(thisWorld);
    DBG_CHECK(solver);
    DBG_CHECK(collisionConfiguration);
    DBG_CHECK(dispatcher);
    DBG_CHECK(broadphase);
    removeAllRigidBodies();
    delete thisWorld;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;
}

void PhysicsWorld::initialize() {
    initializeDebugDraw();
}

void PhysicsWorld::update() {

    if (!thisWorld) {
        DBG_LOG("The Physics World Has Not Been Initialized, Please Initialize Before Updating. (PhysicsWorld.cpp update())\n");
        return;
    }

    thisWorld->stepSimulation(GameInfo::getDeltaTime(), GameInfo::PHYSICS_MAX_SUBSTEPS, GameInfo::PHYSICS_TIME_STEP);

    for (int i = 0; i < thisWorld->getDispatcher()->getNumManifolds(); i++) {
        btPersistentManifold* contactManifold = thisWorld->getDispatcher()->getManifoldByIndexInternal(i);

        const btCollisionObject* obA = contactManifold->getBody0();
        const btCollisionObject* obB = contactManifold->getBody1();

        if (obA->getUserPointer() && obB->getUserPointer()) {
            CollisionTag& tag1 = *static_cast<CollisionTag*>(obA->getUserPointer());
            CollisionTag& tag2 = *static_cast<CollisionTag*>(obB->getUserPointer());

            for (unsigned int j = 0; j < GameInfo::MAX_COLLISION_TRIGGERS_AT_ONCE; j++) {
                tag1.collidingWith.at(j) = nullptr;
                tag2.collidingWith.at(j) = nullptr;
            }

            for (int j = 0; j < contactManifold->getNumContacts(); j++) {
                if (contactManifold->getContactPoint(j).getDistance() < 0.f) {
                    if (j < GameInfo::MAX_COLLISION_TRIGGERS_AT_ONCE) {
                        tag1.collidingWith.at(j) = &tag2;
                        tag2.collidingWith.at(j) = &tag1;
                        break;
                    }
                }
            }
        }
    }
}

void PhysicsWorld::setDebugMode(btIDebugDraw::DebugDrawModes mode) {
    debugDrawer.setDebugMode(mode);
}

void PhysicsWorld::debugDraw(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {
    thisWorld->debugDrawWorld();
    debugDrawer.render(projectionMatrix, viewMatrix);
}

void PhysicsWorld::initializeDebugDraw() {
    debugDrawer.initialize();

    thisWorld->setDebugDrawer(&debugDrawer);

    debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
}

void PhysicsWorld::addRigidBody(const CollisionMesh& collisionMesh) {
    DBG_CHECK(thisWorld);
    DBG_CHECK(collisionMesh.getRigidBody()->getUserPointer());

    thisWorld->addRigidBody(collisionMesh.getRigidBody());
    rigidBodies.push_back(collisionMesh.getRigidBody());

    if (collisionMesh.isKinematic() && collisionMesh.isTrigger()) {
        btBroadphaseProxy* bproxy = collisionMesh.getRigidBody()->getBroadphaseHandle();
        if (bproxy) {
            bproxy->m_collisionFilterGroup = short(btBroadphaseProxy::DefaultFilter);
            bproxy->m_collisionFilterMask  = short(btBroadphaseProxy::AllFilter);
        }
    }
}

void PhysicsWorld::removeRigidBody(const CollisionMesh& collisionMesh) {
    DBG_CHECK(thisWorld);

    for (unsigned int i = 0; i < rigidBodies.size(); i++) {
        if (rigidBodies[i] == collisionMesh.getRigidBody()) {
            thisWorld->removeRigidBody(rigidBodies[i]);
            rigidBodies.erase(rigidBodies.begin() + i);
        }
    }
}

void PhysicsWorld::removeAllRigidBodies() {
    DBG_CHECK(thisWorld);

    for (unsigned int i = 0; i < rigidBodies.size(); i++) {
        thisWorld->removeRigidBody(rigidBodies[i]);
    }

    rigidBodies.clear();
}
