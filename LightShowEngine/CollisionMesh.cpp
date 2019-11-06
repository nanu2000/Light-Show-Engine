#include "CollisionMesh.h"

void CollisionMesh::initializeMeshShape(
    const btTransform& transformation,
    _3DM::Model& model,
    unsigned int meshIndex,
    float mass,
    CollisionTag& usrTag,
    bool trigger,
    btVector3& localInertia,
    float friction,
    float restitution,
    bool kinematic) {
    isMesh = true;

    //this allocates memory
    thisMesh = createTriangleMeshCollider(&model, meshIndex);

    if (!thisMesh) {
        DBG_LOG("Error creating triangle mesh. (CollisionMesh.cpp CollisionMesh::initialize())\n");

        isMesh = false;
        return;
    }

    thisShape = new btBvhTriangleMeshShape(thisMesh, true);

    thisShape->calculateLocalInertia(mass, localInertia);

    thisMotionState = new btDefaultMotionState(transformation);

    btCI collisionInfo = btCI(mass, thisMotionState, thisShape, localInertia);

    collisionInfo.m_friction = friction;

    collisionInfo.m_restitution = restitution;

    rigidBody = new btRigidBody(collisionInfo);

    thisTag = usrTag;

    rigidBody->setUserPointer(&thisTag);

    setTrigger(trigger);
    setKinematic(kinematic);
}

void CollisionMesh::initializeModelShape(
    const btTransform& transformation,
    _3DM::Model& model,
    float mass,
    CollisionTag& usrTag,
    bool trigger,
    btVector3& localInertia,
    float friction,
    float restitution,
    bool kinematic) {

    isMesh = true;

    //this allocates memory
    thisMesh = createTriangleModelCollider(&model);

    if (!thisMesh) {
        DBG_LOG("Error creating triangle mesh. (CollisionMesh.cpp CollisionMesh::initialize())\n");

        isMesh = false;
        return;
    }

    thisShape = new btBvhTriangleMeshShape(thisMesh, true);

    thisShape->calculateLocalInertia(mass, localInertia);

    thisMotionState = new btDefaultMotionState(transformation);

    btCI collisionInfo = btCI(mass, thisMotionState, thisShape, localInertia);

    collisionInfo.m_friction = friction;

    collisionInfo.m_restitution = restitution;

    rigidBody = new btRigidBody(collisionInfo);

    thisTag = usrTag;

    rigidBody->setUserPointer(&thisTag);

    setTrigger(trigger);
    setKinematic(kinematic);
}

void CollisionMesh::initializeMeshShape(const btTransform& transformation, _3DM::AnimatedModel& model, unsigned int meshIndex, float mass, CollisionTag& usrTag, bool trigger, btVector3& localInertia, float friction, float restitution, bool kinematic) {
    isMesh = true;

    //this allocates memory
    thisMesh = createTriangleMeshCollider(&model, meshIndex);

    if (!thisMesh) {
        DBG_LOG("Error creating triangle mesh. (CollisionMesh.cpp CollisionMesh::initialize())\n");

        isMesh = false;
        return;
    }

    thisShape = new btBvhTriangleMeshShape(thisMesh, true);

    thisShape->calculateLocalInertia(mass, localInertia);

    thisMotionState = new btDefaultMotionState(transformation);

    btCI collisionInfo = btCI(mass, thisMotionState, thisShape, localInertia);

    collisionInfo.m_friction = friction;

    collisionInfo.m_restitution = restitution;

    rigidBody = new btRigidBody(collisionInfo);

    thisTag = usrTag;

    rigidBody->setUserPointer(&thisTag);

    setTrigger(trigger);
    setKinematic(kinematic);
}

void CollisionMesh::initializeModelShape(const btTransform& transformation, _3DM::AnimatedModel& model, float mass, CollisionTag& usrTag, bool trigger, btVector3& localInertia, float friction, float restitution, bool kinematic) {
    isMesh = true;

    //this allocates memory
    thisMesh = createTriangleModelCollider(&model);

    if (!thisMesh) {
        DBG_LOG("Error creating triangle mesh. (CollisionMesh.cpp CollisionMesh::initialize())\n");

        isMesh = false;
        return;
    }

    thisShape = new btBvhTriangleMeshShape(thisMesh, true);

    thisShape->calculateLocalInertia(mass, localInertia);

    thisMotionState = new btDefaultMotionState(transformation);

    btCI collisionInfo = btCI(mass, thisMotionState, thisShape, localInertia);

    collisionInfo.m_friction = friction;

    collisionInfo.m_restitution = restitution;

    rigidBody = new btRigidBody(collisionInfo);

    thisTag = usrTag;

    rigidBody->setUserPointer(&thisTag);

    setTrigger(trigger);
    setKinematic(kinematic);
}

void CollisionMesh::setTrigger(bool t) {
    assert(rigidBody != nullptr);
    if (t) {
        rigidBody->setCollisionFlags(
            rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    } else {
        rigidBody->setCollisionFlags(
            rigidBody->getCollisionFlags()
            & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
    }

    isTriggerBody = t;
}

void CollisionMesh::setKinematic(bool t) {
    assert(rigidBody != nullptr);
    if (t) {
        rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    } else {
        rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
    }

    isKinematicBody = t;
}

Transform CollisionMesh::getTransformation() {
    assert(rigidBody != nullptr && thisShape != nullptr);
    return Transform(
        hh::toGlmVec3(rigidBody->getCenterOfMassPosition()),
        hh::toGlmQuat(rigidBody->getCenterOfMassTransform().getRotation()),
        hh::toGlmVec3(thisShape->getLocalScaling()));
}

CollisionMesh::~CollisionMesh() {
    assert(rigidBody != nullptr && thisMotionState != nullptr && thisShape != nullptr);

    delete rigidBody;
    delete thisMotionState;
    delete thisShape;
    if (isMesh) {
        delete thisMesh;
    }
}

btTriangleMesh* CollisionMesh::createTriangleMeshCollider(_3DM::AnimatedModel* model, unsigned int meshIndex) {
    if (meshIndex >= model->amountOfMeshes()) {
        return nullptr;
    }

    //Allocate Memory
    btTriangleMesh* tempMesh = new btTriangleMesh(true, false);

    std::vector<uint32_t>* indices = model->getMeshIndices(meshIndex);
    std::vector<glm::vec3>* verts  = model->getMeshVertices(meshIndex);

    btVector3 vertexPos[3];

    for (unsigned int i = 0; i < indices->size(); i += 3) {
        for (unsigned int j = 0; j < 3; j++) {

            int index      = indices->at(i + j);
            glm::vec3& vec = verts->at(index);
            vertexPos[j].setX(vec.x);
            vertexPos[j].setY(vec.y);
            vertexPos[j].setZ(vec.z);
        }

        tempMesh->addTriangle(vertexPos[0], vertexPos[1], vertexPos[2], true);
    }

    return tempMesh;
}

btTriangleMesh* CollisionMesh::createTriangleModelCollider(_3DM::AnimatedModel* model) {
    if (model->amountOfMeshes() <= 0) {
        DBG_LOG("No Meshes in Supplied Model!! (CollisionMesh.cpp)\n");
        return nullptr;
    }

    //Allocate Memory
    btTriangleMesh* tempMesh = new btTriangleMesh(true, false);

    btVector3 vertexPos[3];

    for (unsigned int i = 0; i < model->amountOfMeshes(); i++) {
        std::vector<uint32_t>* indices = model->getMeshIndices(i);
        std::vector<glm::vec3>* verts  = model->getMeshVertices(i);

        for (unsigned int i = 0; i < indices->size(); i += 3) {
            for (unsigned int j = 0; j < 3; j++) {

                int index      = indices->at(i + j);
                glm::vec3& vec = verts->at(index);
                vertexPos[j].setX(vec.x);
                vertexPos[j].setY(vec.y);
                vertexPos[j].setZ(vec.z);
            }

            tempMesh->addTriangle(vertexPos[0], vertexPos[1], vertexPos[2], true);
        }
    }

    return tempMesh;
}

btTriangleMesh* CollisionMesh::createTriangleMeshCollider(_3DM::Model* model, unsigned int meshIndex) {

    if (meshIndex >= model->amountOfMeshes()) {
        return nullptr;
    }

    //Allocate Memory
    btTriangleMesh* tempMesh = new btTriangleMesh(true, false);

    std::vector<uint32_t>* indices = model->getMeshIndices(meshIndex);
    std::vector<glm::vec3>* verts  = model->getMeshVertices(meshIndex);

    btVector3 vertexPos[3];

    for (unsigned int i = 0; i < indices->size(); i += 3) {
        for (unsigned int j = 0; j < 3; j++) {

            int index      = indices->at(i + j);
            glm::vec3& vec = verts->at(index);
            vertexPos[j].setX(vec.x);
            vertexPos[j].setY(vec.y);
            vertexPos[j].setZ(vec.z);
        }

        tempMesh->addTriangle(vertexPos[0], vertexPos[1], vertexPos[2], true);
    }

    return tempMesh;
}

btTriangleMesh* CollisionMesh::createTriangleModelCollider(_3DM::Model* model) {
    if (model->amountOfMeshes() <= 0) {
        DBG_LOG("No Meshes in Supplied Model!! (CollisionMesh.cpp)\n");
        return nullptr;
    }

    //Allocate Memory
    btTriangleMesh* tempMesh = new btTriangleMesh(true, false);

    btVector3 vertexPos[3];

    for (unsigned int i = 0; i < model->amountOfMeshes(); i++) {
        std::vector<uint32_t>* indices = model->getMeshIndices(i);
        std::vector<glm::vec3>* verts  = model->getMeshVertices(i);

        for (unsigned int i = 0; i < indices->size(); i += 3) {
            for (unsigned int j = 0; j < 3; j++) {

                int index      = indices->at(i + j);
                glm::vec3& vec = verts->at(index);
                vertexPos[j].setX(vec.x);
                vertexPos[j].setY(vec.y);
                vertexPos[j].setZ(vec.z);
            }

            tempMesh->addTriangle(vertexPos[0], vertexPos[1], vertexPos[2], true);
        }
    }

    return tempMesh;
}
