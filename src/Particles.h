#ifndef PARTICLES_H
#define PARTICLES_H

#include "Component.h"
#include "Locator.h"
#include "Settings.h"
#include "ShaderBase.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

//Used to tell which system should be used for a Particles component.
//A system can be an overriden DefaultParticleSystem- the proper system to be called is set in the main systems file's.
enum class PARTICLE_TYPE {
    Default  = 0,
    Fountain = 1,
    Max      = 2
};

//Particle struct used for instanced rendering.
//      !!! Be careful changing it's contents !!!         : a VBO relies on it's format.
struct Particle {
    float lifeTime     = 0;
    float weight       = 0;
    float size         = 1;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 speed    = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec4 color    = glm::vec4(1, 1, 1, 1);
};

//Manages an array of particles and generated vao/vbos to be used by a particle system.
class Particles : public Component<Particles> {
public:
    Particles() {
        particles.resize(100);
    }

    //Size must not change throughout the lifetime of the system.
    Particles(unsigned int size) {
        particles.resize(size);
    }

    void initialize(const Settings& worldSettings);

    ~Particles();

    bool areVitalsNull();

    unsigned int getParticlesPerSecond() { return particlesPerSecond; }
    const Texture* const getTexture() { return particleTexture; }
    std::vector<Particle>* const getParticles() { return &particles; }
    PARTICLE_TYPE const getParticleType() { return particleType; }

    void setTexture(const Texture& pTexture) { particleTexture = &pTexture; }
    //Used for wind direction & such
    void Particles::setWorldSettings(const Settings& worldSettings) { currentWorldSettings = &worldSettings; }

    //Particles per second must be less than or equal to the amount of particles allocated.
    void setParticlesPerSecond(unsigned int pps) { particlesPerSecond = pps; }
    void setWeight(float dfltWeight) { defaultWeight = dfltWeight; }
    void setDefaultLifeTime(float dfltLifeTime) { defaultLifeTime = dfltLifeTime; }
    void setDefaultStartScale(float scl) { defaultScale = scl; }
    void setDefaultColor(const glm::vec4& color) { defaultColor = color; }
    void setPosition(const glm::vec3& position) { emmisionPosition = position; }
    void setParticleType(PARTICLE_TYPE t) { particleType = t; }

private:
    PARTICLE_TYPE particleType     = PARTICLE_TYPE::Default;
    const Texture* particleTexture = nullptr;
    std::vector<Particle> particles;

    const Settings* currentWorldSettings = nullptr;

    float defaultLifeTime = 1;
    float defaultWeight   = 1;
    float defaultScale    = .3f;

    glm::vec4 defaultColor     = glm::vec4(1, 1, 1, 1);
    glm::vec3 emmisionPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    unsigned int particlesPerSecond = 1;
    float newparticles              = 0;
    int renderingSize               = 0;

    GLuint vertexArrayObject    = 0;
    GLuint bufferObject         = 0;
    GLuint instanceBufferObject = 0;

    bool initialized = false;

    friend class DefaultParticleSystem;
    friend class FountainParticleSystem;
};

#endif
