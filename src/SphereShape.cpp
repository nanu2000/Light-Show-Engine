#include "SphereShape.h"

void SphereShape::createSphere(int radius, int stacks, int slices) {
    int ind = 0;
    int i, j;
    for (i = 0; i <= stacks; i++) {
        double lat0 = glm::pi<float>() * (-0.5 + (double)(i - 1) / stacks);
        double z0   = sin(lat0);
        double zr0  = cos(lat0);

        double lat1 = glm::pi<float>() * (-0.5 + (double)i / stacks);
        double z1   = sin(lat1);
        double zr1  = cos(lat1);

        for (j = 0; j <= slices; j++) {
            double lng = 2 * glm::pi<float>() * (double)(j - 1) / slices;
            double x   = cos(lng);
            double y   = sin(lng);

            vertices.push_back(glm::vec3(radius * x * zr1, radius * y * zr1, radius * z1));
            vertices.push_back(glm::vec3(radius * x * zr0, radius * y * zr0, radius * z0));
        }
    }

    GLuint posAttrib = Shaders::getAttribLocation(Shaders::AttribName::Position);

    glGenVertexArrays(1, &VAOID);
    glGenBuffers(1, &vertID);

    glBindVertexArray(VAOID);
    glBindBuffer(GL_ARRAY_BUFFER, vertID);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    initialized = true;
}

void SphereShape::drawSphere(Shader& shader) {
    DBG_LOG("rendering");
    GLuint transformLoc = Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::ModelMatrix);
    GLuint posAttrib    = Shaders::getAttribLocation(Shaders::AttribName::Position);

    glBindVertexArray(VAOID);

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4()));

    glBindBuffer(GL_ARRAY_BUFFER, vertID);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());

    glBindVertexArray(0);
}

SphereShape::~SphereShape() {
    if (!initialized) {
        return;
    }

    DBG_LOG("Freeing memory for sphereshape.\n");

    glDeleteVertexArrays(1, &VAOID);
    glDeleteBuffers(1, &vertID);
}
