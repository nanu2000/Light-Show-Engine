#ifndef CONTROLLER_RAY_COLLISION
#define CONTROLLER_RAY_COLLISION
struct ControllerRayCollision
{
	ControllerRayCollision() : rayIndex(-1), lowestPointIndex(-1), lowestPointValue(0.0f) {}

	ControllerRayCollision(int rayInd, int lowestPointind, float lowestPointVal)
	{
		rayIndex = rayInd;
		lowestPointIndex = lowestPointind;
		lowestPointValue = lowestPointVal;
	}
	
	static const ControllerRayCollision INVALID_COLLISION;

	int rayIndex = -1;
	int lowestPointIndex = -1;
	float lowestPointValue = 0.0f;


	bool operator!=(const ControllerRayCollision& other)
	{
		return (other.lowestPointIndex != lowestPointIndex || other.lowestPointValue != lowestPointValue || other.rayIndex != rayIndex);
	}

};

#endif