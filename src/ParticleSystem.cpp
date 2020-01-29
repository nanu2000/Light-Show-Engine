#include "ParticleSystem.h"

void Particles::initialize(const Settings& worldSettings) {

    currentWorldSettings = &worldSettings;

    // Vertex Buffer Object
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    glGenBuffers(1, &bufferObject);
    glGenBuffers(1, &instanceBufferObject);

    glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(hh::quadVertices), &hh::quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(Shaders::getAttribLocation(Shaders::AttribName::Position));
    glEnableVertexAttribArray(Shaders::getAttribLocation(Shaders::AttribName::TextureCoordinates));

    glVertexAttribPointer(
        Shaders::getAttribLocation(Shaders::AttribName::Position),
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(GLfloat),
        (GLvoid*)0);

    glVertexAttribPointer(
        Shaders::getAttribLocation(Shaders::AttribName::TextureCoordinates),
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(GLfloat),
        (GLvoid*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);

    for (int i = 0; i < particles.size(); i++) {

        setParticleToDefault(particles[i]);
    }

    initialized = true;
}

bool Particles::areVitalsNull() {
    if (!currentWorldSettings) {
        DBG_LOG("Please initialize particle emitter properly\n");
        return true;
    }
    return false;
}
Particles::~Particles() {
    if (!initialized) {
        return;
    }

    DBG_LOG("Freeing memory for particle emitter.\n");

    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &bufferObject);
    glDeleteBuffers(1, &instanceBufferObject);
}

void Particles::setWorldSettings(const Settings& worldSettings) {
    currentWorldSettings = &worldSettings;
}

void Particles::setParticleToDefault(Particle& particle) {

    particle.speed = glm::vec3(std::rand() % 10 - 5, std::rand() % 10 - 5, std::rand() % 10 - 5);

    particle.position = emmisionPosition;
    particle.color    = defaultColor;
    particle.size     = defaultScale;
    particle.lifeTime = defaultLifeTime;
}

void ParticleSystem::renderParticles(ShaderBase& shader, Particles& particles) {
    if (particles.renderingSize > 0 && !particles.areVitalsNull()) {
        if (!particles.getTexture()) {
            DBG_LOG("Particle Texture Is Null! Cannot Render Particles! (ParticleSystem::renderParticles in ParticleSystem.cpp) \n");
            return;
        }

        std::vector<Particle>& particleV = *particles.getParticles();

        glBindVertexArray(particles.vertexArrayObject);

        glBindBuffer(GL_ARRAY_BUFFER, particles.instanceBufferObject);
        glBufferData(GL_ARRAY_BUFFER, particles.renderingSize * sizeof(Particle), &particleV[0], GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(Shaders::getAttribLocation(Shaders::AttribName::ParticlePosition));
        glEnableVertexAttribArray(Shaders::getAttribLocation(Shaders::AttribName::ParticleScale));
        glEnableVertexAttribArray(Shaders::getAttribLocation(Shaders::AttribName::ParticleColor));

        glVertexAttribPointer(
            Shaders::getAttribLocation(Shaders::AttribName::ParticlePosition),
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Particle),
            (GLvoid*)(offsetof(Particle, Particle::position)));
        glVertexAttribPointer(
            Shaders::getAttribLocation(Shaders::AttribName::ParticleScale),
            1,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Particle),
            (GLvoid*)(offsetof(Particle, Particle::size)));
        glVertexAttribPointer(
            Shaders::getAttribLocation(Shaders::AttribName::ParticleColor),
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Particle),
            (GLvoid*)(offsetof(Particle, Particle::color)));

        glVertexAttribDivisor(Shaders::getAttribLocation(Shaders::AttribName::ParticleColor), 1);
        glVertexAttribDivisor(Shaders::getAttribLocation(Shaders::AttribName::ParticleScale), 1);
        glVertexAttribDivisor(Shaders::getAttribLocation(Shaders::AttribName::ParticlePosition), 1);

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
}

void ParticleSystem::fixedUpdateParticles(Particles& particles) {
    std::vector<Particle>& particlesV = *particles.getParticles();

    //Lets say we want 60 particles per second - pps

    //~.1 seconds lets say is every frame - dt

    //every frame we need pps * dt particles. 60 * .1 = 6 per frame to spawn

    particles.newparticles += GameInfo::fixedDeltaTime * particles.particlesPerSecond;

    int particlesToSpawn = static_cast<int>(particles.newparticles);

    if (particlesToSpawn > 0) {

        while (particlesToSpawn > 0) {

            if (particles.renderingSize <= static_cast<int>(particlesV.size())) {

                //Get the last particle
                particles.setParticleToDefault(particlesV[particles.renderingSize]);

                particlesV[particles.renderingSize].lifeTime -= GameInfo::fixedDeltaTime;

                particles.renderingSize++;

                particlesToSpawn--;

            } else {
                break;
            }
        }
        particles.newparticles = 0;
    }

    assert(particles.renderingSize <= particles.particlesPerSecond);

    for (int i = 0; i < particles.renderingSize; i++) {
        particlesV[i].lifeTime -= GameInfo::fixedDeltaTime;

        if (particlesV[i].lifeTime <= 0) {
            hh::swapVal(particlesV[i], particlesV[particles.renderingSize - 1]);
            particles.renderingSize--;
        }
    }
}

void ParticleSystem::updateParticles(Particles& particles) {
    std::vector<Particle>& particlesV = *particles.getParticles();

    for (int i = 0; i < particles.renderingSize; i++) {
        performParticleCalculations(particlesV[i], particles);
    }
}

//Generally calculations are overridden
void ParticleSystem::performParticleCalculations(Particle& particle, Particles& particles) {
    particle.speed += particles.currentWorldSettings->getGravity() * GameInfo::getDeltaTime() * 0.5f;
    particle.position += particle.speed * GameInfo::getDeltaTime();
}
