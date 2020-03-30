#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Particles.h"
#include "SystemBase.h"

//The base particle system.
class DefaultParticleSystem : public SystemBase {

public:
    virtual void renderParticles(Shader& shader, Particles& particles);
    virtual void fixedUpdateParticles(Particles& particlesWrapper);
    virtual void updateParticles(Particles& particlesWrapper);

protected:
    virtual void setParticleToDefault(Particle& particle, Particles& particles);
    virtual void performParticleCalculations(Particle& particle, Particles& particles);
};

//A fountain particle system.
class FountainParticleSystem : public DefaultParticleSystem {

private:
    void setParticleToDefault(Particle& particle, Particles& particles) override;
    void performParticleCalculations(Particle& particle, Particles& particles) override;
};

#endif
