#ifndef COLLISION_MESH_H
#define COLLISION_MESH_H
#include "AnimatedModel.h"
#include "CollisionTag.h"
#include "Component.h"
#include "HelpingHand.h"
#include "Model.h"
#include "Transform.h"
#include "btBulletDynamicsCommon.h"

class CollisionMesh : public Component<CollisionMesh> {

public:
    //typename T is expected to be inheriting from btCollisionShape
    template <typename T>
    void initialize(
        const btTransform& transformation,
        const T& shape,
        float mass,
        const CollisionTag usrTag = CollisionTag(),
        bool trigger              = false,
        btVector3 localInertia    = btVector3(0, 0, 0),
        float friction            = 0,
        float restitution         = 0,
        bool kinematic            = false) {

        if (hasInit) {
            DBG_LOG("CollisionMesh already initialized!\n");
            deInit();
        }

        thisShape = new T(shape);

        thisShape->calculateLocalInertia(mass, localInertia);

        thisMotionState = new btDefaultMotionState(transformation);

        btCI collisionInfo = btCI(mass, thisMotionState, thisShape, localInertia);

        collisionInfo.m_friction    = friction;
        collisionInfo.m_restitution = restitution;

        rigidBody = new btRigidBody(collisionInfo);

        thisTag = usrTag;

        rigidBody->setUserPointer(&thisTag);

        setTrigger(trigger);
        setKinematic(kinematic);

        hasInit = true;
    }

    const CollisionTag* const getTag() {
        return static_cast<CollisionTag*>(rigidBody->getUserPointer());
    }

    void setUserTag(const CollisionTag& tag) {
        thisTag = tag;
        rigidBody->setUserPointer(&thisTag);
    }

    void initializeMeshShape(
        const btTransform& transformation,
        _3DM::Model& model,
        unsigned int meshIndex,
        float mass,
        CollisionTag usrTag    = CollisionTag(),
        bool trigger           = false,
        btVector3 localInertia = btVector3(0, 0, 0),
        float friction         = 0,
        float restitution      = 0,
        bool kinematic         = false);

    void initializeModelShape(
        const btTransform& transformation,
        _3DM::Model& model,
        float mass,
        CollisionTag usrTag    = CollisionTag(),
        bool trigger           = false,
        btVector3 localInertia = btVector3(0, 0, 0),
        float friction         = 0,
        float restitution      = 0,
        bool kinematic         = false);

    void initializeMeshShape(
        const btTransform& transformation,
        _3DM::AnimatedModel& model,
        unsigned int meshIndex,
        float mass,
        CollisionTag usrTag    = CollisionTag(),
        bool trigger           = false,
        btVector3 localInertia = btVector3(0, 0, 0),
        float friction         = 0,
        float restitution      = 0,
        bool kinematic         = false);

    void initializeModelShape(
        const btTransform& transformation,
        _3DM::AnimatedModel& model,
        float mass,
        CollisionTag usrTag    = CollisionTag(),
        bool trigger           = false,
        btVector3 localInertia = btVector3(0, 0, 0),
        float friction         = 0,
        float restitution      = 0,
        bool kinematic         = false);

    void setTrigger(bool t);

    Transform getTransformation();

    ~CollisionMesh();

    btDefaultMotionState* const getMotionState() const { return thisMotionState; }
    btCollisionShape* const getShape() const { return thisShape; }
    btRigidBody* const getRigidBody() const { return rigidBody; }

    void setScale(const glm::vec3& scale) {
        assert(thisShape != nullptr);
        thisShape->setLocalScaling(hh::toBtVec3(scale));
    }

    glm::vec3 getScale() {
        assert(thisShape != nullptr);
        return hh::toGlmVec3(thisShape->getLocalScaling());
    }
    glm::vec3 getPosition() {
        assert(rigidBody != nullptr);
        return hh::toGlmVec3(rigidBody->getCenterOfMassPosition());
    }
    glm::quat getRotation() {
        assert(rigidBody != nullptr);
        return hh::toGlmQuat(rigidBody->getCenterOfMassTransform().getRotation());
    }

    void translate(const glm::vec3& addition) {
        rigidBody->translate(hh::toBtVec3(addition));
    }
    void translate(const btVector3& addition) {
        rigidBody->translate(addition);
    }

    void addVelocity(const btVector3& v) {
        assert(rigidBody != nullptr);

        rigidBody->setLinearVelocity(
            btVector3(
                rigidBody->getLinearVelocity().x() + v.x(),
                rigidBody->getLinearVelocity().y() + v.y(),
                rigidBody->getLinearVelocity().z() + v.z()));
    }

    void addVelocity(const glm::vec3& v) {
        assert(rigidBody != nullptr);

        rigidBody->setLinearVelocity(
            btVector3(
                rigidBody->getLinearVelocity().x() + v.x,
                rigidBody->getLinearVelocity().y() + v.y,
                rigidBody->getLinearVelocity().z() + v.z));
    }

    void setVelocity(const btVector3& v) {
        assert(rigidBody != nullptr);
        rigidBody->setLinearVelocity(v);
    }

    void setVelocity(const glm::vec3& v) {
        assert(rigidBody != nullptr);
        rigidBody->setLinearVelocity(hh::toBtVec3(v));
    }

    glm::vec3 getVelocity() {
        assert(rigidBody != nullptr);
        return hh::toGlmVec3(rigidBody->getLinearVelocity());
    }
    btVector3 getVelocitybt() const {
        assert(rigidBody != nullptr);
        return rigidBody->getLinearVelocity();
    }
    void applyCentralImpulse(const glm::vec3& impulse) {
        assert(rigidBody != nullptr);
        rigidBody->applyCentralImpulse(hh::toBtVec3(impulse));
    }
    void applyCentralImpulse(const btVector3& impulse) {
        assert(rigidBody != nullptr);
        rigidBody->applyCentralImpulse(impulse);
    }
    void activateRigidBody(bool t) {
        assert(rigidBody != nullptr);
        rigidBody->activate(t);
    }

    bool isKinematic() const { return isKinematicBody; }
    bool isTrigger() const { return isTriggerBody; }

private:
    void deInit();

    bool isKinematicBody = false;
    bool isTriggerBody   = false;

    void setKinematic(bool t);

    typedef btRigidBody::btRigidBodyConstructionInfo btCI;

    bool isMesh  = false;
    bool hasInit = false;

    CollisionTag thisTag;

    btRigidBody* rigidBody;
    btCollisionShape* thisShape;
    btDefaultMotionState* thisMotionState;
    btTriangleMesh* thisMesh;

    //Will allocate memory when used, returns nullptr on error
    btTriangleMesh* createTriangleMeshCollider(_3DM::AnimatedModel* model, unsigned int meshIndex);
    btTriangleMesh* createTriangleModelCollider(_3DM::AnimatedModel* model);
    btTriangleMesh* createTriangleMeshCollider(_3DM::Model* model, unsigned int meshIndex);
    btTriangleMesh* createTriangleModelCollider(_3DM::Model* model);
};

#endif // !COLLISION_MESH_H
