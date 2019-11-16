#ifndef LIGHTS_H
#define LIGHTS_H
#include <glm/vec3.hpp>

struct PointLight : Component<PointLight> {
    float constant  = 1;
    float linear    = .09f;
    float quadratic = .032f;

    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 ambient  = glm::vec3(.4f, 0.4f, 0.4f);
    glm::vec3 diffuse  = glm::vec3(.4f, 0.4f, 0.45f);
    glm::vec3 specular = glm::vec3(1, .9f, .8f);
};

struct DirectionalLight : Component<DirectionalLight> {
    glm::vec3 direction = glm::vec3(0, -1, 0);
    glm::vec3 ambient   = glm::vec3(.05f, 0.05f, 0.05f);
    glm::vec3 diffuse   = glm::vec3(.4f, 0.4f, 0.4f);
    glm::vec3 specular  = glm::vec3(.6f, .54f, .5f);
};

#endif // !LIGHTS_H
