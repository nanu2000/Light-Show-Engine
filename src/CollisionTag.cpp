#include "CollisionTag.h"

CollisionTag::CollisionTag() {
    tagName = COLLISION_TAGS::TAG_EMPTY;
    entity  = -1;
}

CollisionTag::CollisionTag(COLLISION_TAGS tag, int32_t tagEntity) {
    tagName = tag;
    entity  = tagEntity;
}
