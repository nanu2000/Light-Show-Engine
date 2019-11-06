#ifndef ANIMATION_KEYS_H
#define ANIMATION_KEYS_H
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
namespace _3DM {
struct Vector3Key {
    Vector3Key(float tme, glm::vec3 orgn) {
        time   = tme;
        origin = orgn;
    }
    Vector3Key() {
    }
    float time;
    glm::vec3 origin;
};

struct QuatKey {
    QuatKey(float tme, glm::quat orgn) {
        time   = tme;
        origin = orgn;
    }

    QuatKey() {
    }
    float time;
    glm::quat origin;
};
}

#endif
