#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Component.h"
#include "Locator.h"
#include "Settings.h"
#include "ShaderBase.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

struct Particle {
    float lifeTime     = 0;
    float weight       = 0;
    float size         = 1;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 speed    = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec4 color    = glm::vec4(1, 1, 1, 1);
};

class Particles : public Component<Particles> {
public:
    void initialize(const Settings& worldSettings);

    glm::vec3 emmisionPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    Particles(unsigned int size) {
        particles.resize(size);
    }

    ~Particles();

    unsigned int getParticlesPerSecond() { return particlesPerSecond; }

    //Particles per second must be less than or equal to the amount of particles allocated.
    void setParticlesPerSecond(unsigned int pps) { particlesPerSecond = pps; }
    void setWeight(float dfltWeight) { defaultWeight = dfltWeight; }
    void setDefaultLifeTime(float dfltLifeTime) { defaultLifeTime = dfltLifeTime; }
    void setDefaultStartScale(float scl) { defaultScale = scl; }
    void setDefaultColor(const glm::vec4& color) { defaultColor = color; }
    void setWorldSettings(const Settings& worldSettings);

    void initializeTexture(const Texture& pTexture) {
        particleTexture = &pTexture;
    }

    const Texture* const getTexture() {
        return particleTexture;
    }

    void resize(unsigned int size) {
        particles.resize(size);
    }

    std::vector<Particle>* const getParticles() {
        return &particles;
    }

    bool areVitalsNull();

private:
    void setParticleToDefault(Particle& particle);

    const Texture* particleTexture = nullptr;
    std::vector<Particle> particles;

    const Settings* currentWorldSettings = nullptr;

    float defaultLifeTime  = 1;
    float defaultWeight    = 1;
    float defaultScale     = .3f;
    glm::vec4 defaultColor = glm::vec4(1, 1, 1, 1);

    unsigned int particlesPerSecond = 1;
    float newparticles              = 0;
    int renderingSize               = 0;

    GLuint vertexArrayObject    = 0;
    GLuint bufferObject         = 0;
    GLuint instanceBufferObject = 0;

    bool initialized = false;

    friend class ParticleSystem;
};

class ParticleSystem {

public:
    void renderParticles(ShaderBase& shader, Particles& particles);
    void fixedUpdateParticles(Particles& particlesWrapper);
    void updateParticles(Particles& particlesWrapper);

private:
    void performParticleCalculations(Particle& particle, Particles& particles);
};

#endif
