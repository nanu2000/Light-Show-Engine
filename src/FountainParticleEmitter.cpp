#include "FountainParticleEmitter.h"

void FountainParticleEmitter::setParticleToDefault(Particle& p) {
    p.speed = glm::vec3(
        hh::getRandomRange(-particleHorizontalSpread, particleHorizontalSpread),
        hh::getRandomRange(particleVerticalForce - 2, particleVerticalForce),
        hh::getRandomRange(-particleHorizontalSpread, particleHorizontalSpread));

    p.position = emmisionPosition;
    p.lifeTime = defaultLifeTime;
    p.weight   = defaultWeight;
    p.size     = defaultScale;
    p.color    = defaultColor;
}

void FountainParticleEmitter::performParticleCalculations(Particle& p) {

    float dt = GameInfo::getDeltaTime();
    p.speed += (currentWorldSettings->getWind() + currentWorldSettings->getGravity() + glm::vec3(0, -p.weight, 0)) * dt * 0.5f;
    p.position += p.speed * dt;
    p.speed.y -= dt * fountainPull;
    p.color.a -= dt / defaultLifeTime * defaultColor.a;
    p.size += dt * endScale;
}
