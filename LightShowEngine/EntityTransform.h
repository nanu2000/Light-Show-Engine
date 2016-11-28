#ifndef ENTITY_TRANSFORM
#define ENTITY_TRANSFORM
#include "Transform.h"
#include "Component.h"

class EntityTransform : public Component<EntityTransform>
{
public:
	Transform transform;
};

#endif