#ifndef GLOBAL_INFORMATION_H
#define GLOBAL_INFORMATION_H
#include "Component.h"
#include <glm/vec3.hpp>

class GlobalInformation : public Component<GlobalInformation> 
{

public:

	glm::vec3 getPlayersPosition() const { return playersPosition; }
	

	//Make it so setters can only be accessed with player component
	void setPlayersPosition(const glm::vec3 & pos) { playersPosition = pos; }

private:

	glm::vec3 playersPosition;

};

#endif