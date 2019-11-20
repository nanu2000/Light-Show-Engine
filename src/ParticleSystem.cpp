#include "ParticleSystem.h"

void ParticleEmitter::initialize(const WorldSettings& worldSettings) {

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
}

void ParticleEmitter::renderParticles(ShaderBase& shader, Particles& particlesWrapper) {
    if (renderingSize > 0 && !areVitalsNull()) {
        if (!particlesWrapper.getTexture()) {
            DBG_LOG("Particle Texture Is Null! Cannot Render Particles! (ParticleEmitter::renderParticles in ParticleSystem.cpp) \n");
            return;
        }

        std::vector<Particle>& particles = *particlesWrapper.getParticles();

        glBindVertexArray(vertexArrayObject);

        glBindBuffer(GL_ARRAY_BUFFER, instanceBufferObject);
        glBufferData(GL_ARRAY_BUFFER, renderingSize * sizeof(Particle), &particles[0], GL_DYNAMIC_DRAW);

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
        glBindTexture(GL_TEXTURE_2D, particlesWrapper.getTexture()->getTextureData());
        glUniform1i(Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::DiffuseTexture), 0);

        GLboolean currentDepth;
        glGetBooleanv(GL_DEPTH_WRITEMASK, &currentDepth);

        glDepthMask(GL_FALSE);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, renderingSize);
        glDepthMask(currentDepth);

        glBindVertexArray(0);
    }
}

void ParticleEmitter::mainFixedUpdateParticles(Particles& particlesWrapper) {
    if (areVitalsNull()) {
        return;
    }

    std::vector<Particle>& particles = *particlesWrapper.getParticles();

    for (int i = 0; i < renderingSize; i++) {
        particles[i].lifeTime -= GameInfo::fixedDeltaTime;

        if (particles[i].lifeTime <= GameInfo::fixedDeltaTime) {
            hh::swapVal(particles[i], particles[renderingSize - 1]);
            renderingSize--;
        }
    }

    float pps = 1.0f / particlesPerSecond;

    newparticles += GameInfo::fixedDeltaTime;

    while (newparticles > pps) {
        if (renderingSize <= static_cast<int>(particles.size())) {
            setParticleToDefault(particles[renderingSize]);
            particles[renderingSize].lifeTime -= GameInfo::fixedDeltaTime;
            renderingSize++;
            newparticles -= pps;

        } else {
            break;
        }
    }
}

void ParticleEmitter::setParticleToDefault(Particle& p) {
    p.speed    = glm::vec3(std::rand() % 10 - 5, std::rand() % 10 - 5, std::rand() % 10 - 5);
    p.position = emmisionPosition;
    p.lifeTime = 1;
    p.size     = 1;
}

void ParticleEmitter::fixedUpdateParticles(int amountPerSecond, Particles& particlesWrapper) {
    particlesPerSecond = amountPerSecond;
    mainFixedUpdateParticles(particlesWrapper);
}

void ParticleEmitter::fixedUpdateParticles(Particles& particlesWrapper) {
    mainFixedUpdateParticles(particlesWrapper);
}

void ParticleEmitter::updateParticles(Particles& particlesWrapper) {
    std::vector<Particle>& particles = *particlesWrapper.getParticles();
    for (int i = 0; i < renderingSize; i++) {
        performParticleCalculations(particles[i]);
    }
}

//Generally calculations are overridden
void ParticleEmitter::performParticleCalculations(Particle& p) {
    p.speed += currentWorldSettings->getGravity() * GameInfo::getDeltaTime() * 0.5f;
    p.position += p.speed * GameInfo::getDeltaTime();
}

void ParticleEmitter::setWorldSettings(const WorldSettings& worldSettings) {
    currentWorldSettings = &worldSettings;
}

bool ParticleEmitter::areVitalsNull() {
    if (!currentWorldSettings) {
        DBG_LOG("Please initialize particle emitter properly\n");
        return true;
    }
    return false;
}
