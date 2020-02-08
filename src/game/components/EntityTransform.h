#ifndef ENTITY_TRANSFORM
#define ENTITY_TRANSFORM
#include "Component.h"
#include "Transform.h"

class EntityTransform : public Component<EntityTransform> {
public:
    Transform transform;
};

#endif
