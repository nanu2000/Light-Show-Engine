#ifndef RAY_CASTER
#define RAY_CASTER
#include "Component.h"
#include "HelpingHand.h"
#include "glm/vec3.hpp"
#include <btBulletCollisionCommon.h>
typedef btCollisionWorld::AllHitsRayResultCallback RayResultCallback;

class RayCaster {

public:
    RayCaster(const glm::vec3& from, const glm::vec3& to) {
        rayFrom = hh::toBtVec3(from);
        rayTo   = hh::toBtVec3(to);
        resetCallback();
    }

    RayCaster() {}

    bool hasHit() {
        return rayCallback.hasHit();
    }

    const RayResultCallback* getCallback() {
        return &rayCallback;
    }

    void rayTest(const btCollisionWorld& collisionWorld) {
        collisionWorld.rayTest(rayFrom, rayTo, rayCallback);
    }

    void rayTest(const btCollisionWorld& collisionWorld, const btVector3& from, const btVector3& to) {
        rayFrom = from;
        rayTo   = to;
        resetCallback();

        collisionWorld.rayTest(rayFrom, rayTo, rayCallback);
    }
    void rayTest(const btCollisionWorld& collisionWorld, const glm::vec3& from, const glm::vec3& to) {
        rayFrom = hh::toBtVec3(from);
        rayTo   = hh::toBtVec3(to);
        resetCallback();

        collisionWorld.rayTest(rayFrom, rayTo, rayCallback);
    }

    void resetCallback(const glm::vec3& from, const glm::vec3& to) {
        rayCallback = RayResultCallback(hh::toBtVec3(from), hh::toBtVec3(to));
        length      = glm::distance(hh::toGlmVec3(rayFrom), hh::toGlmVec3(rayTo));
    }
    void resetCallback(const btVector3& from, const btVector3& to) {
        rayCallback = RayResultCallback(from, to);
        length      = glm::distance(hh::toGlmVec3(rayFrom), hh::toGlmVec3(rayTo));
    }
    void resetCallback() {
        rayCallback = RayResultCallback(rayFrom, rayTo);
        length      = glm::distance(hh::toGlmVec3(rayFrom), hh::toGlmVec3(rayTo));
    }

    float getHitFraction(int index) const {
        return rayCallback.m_hitFractions.at(index);
    }

    float getLength() const { return length; }

    float getHitFractionRaySpace(int index) const {
        return rayCallback.m_hitFractions.at(index) * length;
    }

    const btCollisionObject* getCollisionObject(int index) const { return rayCallback.m_collisionObjects.at(index); }

    int size() const { return rayCallback.m_collisionObjects.size(); }

    btVector3 getHitNormal(int index) { return rayCallback.m_hitNormalWorld.at(index); }

private:
    float length = 0;

    btVector3 rayFrom;
    btVector3 rayTo;
    RayResultCallback rayCallback = RayResultCallback(btVector3(0, 0, 0), btVector3(0, 0, 0));
};

#endif
