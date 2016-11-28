#ifndef ENTITY_STATS_H
#define ENTITY_STATS_H
#include "Component.h"

const uint8_t ENTITY_STATS_DEFAULT_MAX_HEALTH = 100;
const uint8_t ENTITY_STATS_DEFAULT_MAX_STAMINA = 100;

class EntityStats : public Component<EntityStats>
{

public:

	uint8_t health = ENTITY_STATS_DEFAULT_MAX_HEALTH;
	uint8_t stamina = ENTITY_STATS_DEFAULT_MAX_STAMINA;


};




#endif