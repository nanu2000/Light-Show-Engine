#ifndef _TRANSFORM_H
#define _TRANSFORM_H
#include "glm/gtc/quaternion.hpp"
struct Transform
{
	Transform(const glm::vec3 & pstin, const glm::quat& rttn, const glm::vec3& scl)
	{
		position	= pstin;
		rotation	= rttn;
		scale		= scl;
	}
	Transform() {}

	glm::vec3 position	= glm::vec3(0.0f, 0.0f, 0.0f);				//Position of object

	glm::quat rotation	= glm::quat(1.0f, 1.0f, 1.0f, 1.0f);		//Rotation of object

	glm::vec3 scale		= glm::vec3(1.0f, 1.0f, 1.0f);				//Scale of object

};

#endif