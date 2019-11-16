#ifndef COLLISION_TAG_H
#define COLLISION_TAG_H
#include "Component.h"
#include "GameInfo.h"
#include <string>
#include <vector>

//COLLISION_TAGS is located in GameInfo.h

struct CollisionTag {

    CollisionTag();

    bool isColliding() { return (collidingWith.size() > 0); }

    CollisionTag(COLLISION_TAGS tag, int32_t tagEntity);

    COLLISION_TAGS tagName = COLLISION_TAGS::TAG_EMPTY;
    int32_t entity         = TAG_ENTITY_UNDEFINED;

    std::vector<CollisionTag*> collidingWith = std::vector<CollisionTag*>(GameInfo::MAX_COLLISION_TRIGGERS_AT_ONCE);
};

#endif // !COLLISION_TAG_H
