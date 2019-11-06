#ifndef SPHERE_SHAPE
#define SPHERE_SHAPE

#include "ShaderBase.h"
#include "glm/gtc/constants.hpp"
#include "glm/vec3.hpp"
#include <vector>
#include "glm/gtc/type_ptr.hpp"
#include "Shaders.h"


class SphereShape
{
public:
	void createSphere(int radius, int stacks, int slices);
	void drawSphere(ShaderBase& shader);
private:
	std::vector<glm::vec3> vertices;
	GLuint VAOID;
	GLuint vertID;

};

#endif