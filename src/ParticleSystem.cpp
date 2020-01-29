#include "ParticleSystem.h"

//Rendering logic
void DefaultParticleSystem::renderParticles(ShaderBase& shader, Particles& particles) {

    if (particles.renderingSize <= 0) {
        return;
    }
    if (particles.areVitalsNull()) {
        DBG_LOG("No vitals provided for particles! (ParticleSystem::renderParticleBase in ParticleSystem.cpp) \n");
        return;
    }
    if (!particles.getTexture()) {
        DBG_LOG("Particle Texture Is Null! Cannot Render Particles! (ParticleSystem::renderParticleBase in ParticleSystem.cpp) \n");
        return;
    }

    std::vector<Particle>& particleV = particles.particles;

    glBindVertexArray(particles.vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, particles.instanceBufferObject);
    glBufferData(GL_ARRAY_BUFFER, particles.renderingSize * sizeof(Particle), &particleV[0], GL_DYNAMIC_DRAW);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, particles.getTexture()->getTextureData());
    glUniform1i(Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::DiffuseTexture), 0);

    GLboolean currentDepth;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &currentDepth);

    glDepthMask(GL_FALSE);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles.renderingSize);
    glDepthMask(currentDepth);

    glBindVertexArray(0);
}

//Ran every fixed frame
void DefaultParticleSystem::fixedUpdateParticles(Particles& particles) {
    std::vector<Particle>& particlesV = particles.particles;

    //Lets say we want 60 particles per second - pps

    //~.1 seconds lets say is every frame - dt

    //every frame we need pps * dt particles. 60 * .1 = 6 per frame to spawn

    particles.newparticles += GameInfo::fixedDeltaTime * particles.particlesPerSecond;

    int particlesToSpawn = static_cast<int>(particles.newparticles);

    if (particlesToSpawn > 0) {

        while (particlesToSpawn > 0) {

            if (particles.renderingSize < static_cast<int>(particlesV.size())) {

                //Get the last particle
                setParticleToDefault(particlesV[particles.renderingSize], particles);
                particlesV[particles.renderingSize].lifeTime -= GameInfo::fixedDeltaTime;

                particles.renderingSize++;

                particlesToSpawn--;

            } else {
                break;
            }
        }
        particles.newparticles = 0;
    }
    assert(particles.renderingSize <= particles.particles.size());

    for (int i = 0; i < particles.renderingSize; i++) {
        particlesV[i].lifeTime -= GameInfo::fixedDeltaTime;

        if (particlesV[i].lifeTime <= 0) {
            hh::swapVal(particlesV[i], particlesV[particles.renderingSize - 1]);
            particles.renderingSize--;
        }
    }
}

//Rand every frame
void DefaultParticleSystem::updateParticles(Particles& particles) {
    std::vector<Particle>& particlesV = particles.particles;

    for (int i = 0; i < particles.renderingSize; i++) {
        performParticleCalculations(particlesV[i], particles);
    }
}

//Sets the default position of the particle. Used when a particles is instantiated.
void DefaultParticleSystem::setParticleToDefault(Particle& particle, Particles& particles) {

    float dt = GameInfo::getDeltaTime();

    particle.speed = glm::vec3(std::rand() % 10 - 5, std::rand() % 10 - 5, std::rand() % 10 - 5);

    particle.position = particles.emmisionPosition;
    particle.color    = particles.defaultColor;
    particle.size     = particles.defaultScale;
    particle.lifeTime = particles.defaultLifeTime;
}

//Generally calculations are overridden
void DefaultParticleSystem::performParticleCalculations(Particle& particle, Particles& particles) {
    particle.speed += GameInfo::getDeltaTime();
    particle.position += particle.speed * GameInfo::getDeltaTime();
}

//Fountain particle stuff:
//------------------------
void FountainParticleSystem::setParticleToDefault(Particle& particle, Particles& particles) {

    particle.speed = glm::vec3(
        hh::getRandomRange(-5, 5),
        hh::getRandomRange(10 - 2, 10),
        hh::getRandomRange(-5, 5));

    particle.position = particles.emmisionPosition;
    particle.color    = particles.defaultColor;
    particle.size     = particles.defaultScale;
    particle.lifeTime = particles.defaultLifeTime;
}

void FountainParticleSystem::performParticleCalculations(Particle& particle, Particles& particles) {

    float dt = GameInfo::getDeltaTime();
    particle.speed += (particles.currentWorldSettings->getWind() + particles.currentWorldSettings->getGravity() + glm::vec3(0, -particle.weight, 0)) * dt * 0.5f;
    particle.position += particle.speed * dt;
    particle.speed.y -= dt * 5;
    particle.color.a -= dt / particles.defaultLifeTime * particles.defaultColor.a;
    if (particle.color.a < 0) {
        particle.color.a = 0;
    }
    particle.size += dt * .4;
}
