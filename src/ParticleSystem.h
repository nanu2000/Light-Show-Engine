#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Component.h"
#include "Locator.h"
#include "ShaderBase.h"
#include "WorldSettings.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

struct Particle {
    float lifeTime = 0;
    float weight   = 0;
    float size     = 1;
    glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 speed = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec4 color = glm::vec4(1, 1, 1, 1);
};

class Particles : public Component<Particles> {
public:
    Particles(unsigned int size) {
        particles.resize(size);
    }

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

private:
    const Texture* particleTexture = nullptr;
    std::vector<Particle> particles;
};

class ParticleEmitter {

public:
    virtual ~ParticleEmitter() {}

    glm::vec3 emmisionPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    void initialize(const WorldSettings& worldSettings);
    void renderParticles(ShaderBase& shader, Particles& particlesWrapper);
    void updateParticles(int amountPerSecond, Particles& particlesWrapper);
    void updateParticles(Particles& particlesWrapper);

    unsigned int getParticlesPerSecond() { return particlesPerSecond; }

    void setParticlesPerSecond(unsigned int pps) { particlesPerSecond = pps; }
    void setWeight(float dfltWeight) { defaultWeight = dfltWeight; }
    void setDefaultLifeTime(float dfltLifeTime) { defaultLifeTime = dfltLifeTime; }
    void setDefaultStartScale(float scl) { defaultScale = scl; }
    void setDefaultColor(const glm::vec4& color) { defaultColor = color; }

    void setWorldSettings(const WorldSettings& worldSettings);

    bool areVitalsNull();

protected:
    const WorldSettings* currentWorldSettings = nullptr;

    float defaultLifeTime  = 1;
    float defaultWeight    = 1;
    float defaultScale     = .3f;
    glm::vec4 defaultColor = glm::vec4(1, 1, 1, 1);

private:
    unsigned int particlesPerSecond = 1;
    float newparticles              = 0;
    int renderingSize               = 0;

    void mainUpdateParticles(Particles& particlesWrapper);
    virtual void setParticleToDefault(Particle& p);
    virtual void performParticleCalculations(Particle& p);

    GLuint vertexArrayObject    = 0;
    GLuint bufferObject         = 0;
    GLuint instanceBufferObject = 0;
};

#endif
