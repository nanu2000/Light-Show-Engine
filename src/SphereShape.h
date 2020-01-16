#ifndef SPHERE_SHAPE
#define SPHERE_SHAPE

#include "ShaderBase.h"
#include "Shaders.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/vec3.hpp"
#include <vector>

class SphereShape {
public:
    void createSphere(int radius, int stacks, int slices);
    void drawSphere(ShaderBase& shader);
    ~SphereShape();

private:
    std::vector<glm::vec3> vertices;
    GLuint VAOID;
    GLuint vertID;
    bool initialized = false;
};

#endif
