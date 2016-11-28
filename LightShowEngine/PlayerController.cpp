#include "PlayerController.h"

float PlayerController::getResistance() const
{
	if (isTouchingGround)
	{
		return (friction + damping) * GameInfo::deltaTime;
	}

	if (jumping)
	{
		return damping * GameInfo::deltaTime;
	}

	return  (damping + fallMovementDamping) * GameInfo::deltaTime;
}

bool PlayerController::canJump(const glm::vec3 & lastPosition) const
{
	return performJump == false && jumping == false && (isTouchingGround || glm::distance(lastTouchedPosition, lastPosition) < jumpEnabledWithinMagnitude);
}
