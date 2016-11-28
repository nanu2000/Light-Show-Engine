#ifndef WORLD_SETTINGS_H
#define WORLD_SETTINGS_H

#include "glm/vec3.hpp"

class WorldSettings
{
public:

	WorldSettings(glm::vec3 gravity, glm::vec3 wind)
	{
		worldGravity	= gravity;
		worldWind		= wind;
	}

	void setWorldWind		(glm::vec3 wind)	{ worldWind		= wind;		}
	void setWorldGravity	(glm::vec3 gravity) { worldGravity	= gravity;	}

	glm::vec3 getGravity() const { return worldGravity;	}
	glm::vec3 getWind	() const { return worldWind;	}

private:
	
	glm::vec3 worldWind;
	glm::vec3 worldGravity;

};

#endif
