#include "HelpingHand.h"

int hh::getRandomRange(int minInclusive, int maxExclusive)
{
	return minInclusive + rand() % (maxExclusive - minInclusive);
}

float hh::getRandomRange(float minInclusive, float maxExclusive)
{
	int randNum = static_cast<int>(minInclusive * 100) + rand() % static_cast<int>((maxExclusive - minInclusive) * 100);
	return static_cast<float>(randNum) / 100.0f;
}

btVector3 hh::toBtVec3(const glm::vec3 & v)
{
	return btVector3(v.x, v.y, v.z);
}

glm::quat hh::toQuaternion(const glm::vec3 & vec)
{
	return glm::quat(glm::radians(vec));
}

glm::vec3 hh::toGlmVec3(const btVector3 & vector)
{
	return glm::vec3(vector.getX(), vector.getY(), vector.getZ());
}

glm::quat hh::toGlmQuat(const btQuaternion & quat)
{
	return glm::quat
	(
		quat.getW(),
		quat.getX(),
		quat.getY(),
		quat.getZ()
	);
}

glm::quat hh::eularToQuat(const glm::vec3 & pitchYawRoll)
{

	glm::quat returnVal;

	glm::vec3 eulerAngle(pitchYawRoll.x * 0.5f, pitchYawRoll.y * 0.5f, pitchYawRoll.z * 0.5f);
	glm::vec3 c = glm::cos(eulerAngle * 0.5f);
	glm::vec3 s = glm::sin(eulerAngle * 0.5f);

	returnVal.w = c.x * c.y * c.z + s.x * s.y * s.z;
	returnVal.x = s.x * c.y * c.z - c.x * s.y * s.z;
	returnVal.y = c.x * s.y * c.z + s.x * c.y * s.z;
	returnVal.z = c.x * c.y * s.z - s.x * s.y * c.z;


	return returnVal;
}

float hh::signedRadiansBetweenVectors(glm::vec3 vec1, glm::vec3 vec2)
{
	vec1 = glm::normalize(vec1);
	vec2 = glm::normalize(vec2);

	return
		std::acos
		(
			glm::clamp
			(
				glm::dot(vec1, vec2), -1.0f, 1.0f
				)
			)
		*
		glm::sign(glm::cross(vec1, vec2).y);
}

glm::vec3 hh::sphericalToCartisean(float theta, float phi, float radius)
{
	//X = cos(theta) * cos(phi) * radius
	//Y = sin(phi)				* radius
	//Z = sin(theta) * cos(phi) * radius

	return glm::vec3
		(
			cos(theta) * cos(phi)	* radius,
			sin(phi)				* radius,
			sin(theta) * cos(phi)	* radius
			);
}

bool hh::isPointWithinCone(const glm::vec3 & coneTipPosition, const glm::vec3 & coneCenterLine, const glm::vec3 & point, const float fovRadians)
{

	return 
	(
		glm::dot	// Get the Dot product for the direction of the cone and the point minus the cones origin (normalized).
		(			// We do this to check how much the point is facing the same direction as the cones direction.
			glm::normalize(point - coneTipPosition),
			coneCenterLine
		)
		>= 
		cos(fovRadians)	//We then compare to see if the dot product result is bigger than or equal to the cosine of the given angle(in radians).
						//If it is, then the point is inside the angle givin.
	);
}

bool hh::isPointWithinConePreCos(const glm::vec3 & coneTipPosition, const glm::vec3 & coneCenterLine, const glm::vec3 & point, const float fovRadiansPreCos)
{
	return (glm::dot(glm::normalize(point - coneTipPosition), coneCenterLine) >= fovRadiansPreCos);
}

bool hh::isPointWithinCone(const glm::vec3 & coneTipPosition, const glm::vec3 & coneCenterLine, const glm::vec3 & point, const float fovRadians, const float withinDistance)
{

	if (glm::distance(point, coneTipPosition) > withinDistance)
	{
		return false;
	}

	return (glm::dot(glm::normalize(point - coneTipPosition), coneCenterLine) >= cos(fovRadians));
}

bool hh::isPointWithinConePreCos(const glm::vec3 & coneTipPosition, const glm::vec3 & coneCenterLine, const glm::vec3 & point, const float fovRadiansPreCos, const float withinDistance)
{
	if (glm::distance(point, coneTipPosition) > withinDistance)
	{
		return false;
	}

	return (glm::dot(glm::normalize(point - coneTipPosition), coneCenterLine) >= fovRadiansPreCos);
}



