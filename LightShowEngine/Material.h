#ifndef MATERIAL_H
#define MATERIAL_H
#include "glm/vec3.hpp"
#include "Component.h"
struct Material : Component<Material>
{
	glm::vec3 ambient	= glm::vec3(.6f, .6f, .65f);
	glm::vec3 diffuse	= glm::vec3(.6f, .6f, .65f);
	glm::vec3 specular	= glm::vec3(.9f, .9f, .9f);
	float shininess		= 8.0f;
};
struct SimpleMaterial : Component<SimpleMaterial>
{
	//For other parameters in the future, use a nested union.
	float shininess = 8.0f;
};
#endif
