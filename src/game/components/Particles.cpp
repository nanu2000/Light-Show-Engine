#include "Particles.h"

Particles::~Particles() {
    if (!initialized) {
        return;
    }

    DBG_LOG("Freeing memory for particles.\n");

    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &bufferObject);
    glDeleteBuffers(1, &instanceBufferObject);
}

//Generates VAO & buffers
void Particles::initialize(const Settings& worldSettings) {

    currentWorldSettings = &worldSettings;

    //Generate vao
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    //Generate buffer for normal stuff, and other for instancing.
    glGenBuffers(1, &bufferObject);
    glGenBuffers(1, &instanceBufferObject);

    //Supply info about quad and texture coordinates to normal buffer.
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

    //Bind instance buffer and supply info about each particle.
    glBindBuffer(GL_ARRAY_BUFFER, instanceBufferObject);
    glEnableVertexAttribArray(Shaders::getAttribLocation(Shaders::AttribName::ParticlePosition));
    glEnableVertexAttribArray(Shaders::getAttribLocation(Shaders::AttribName::ParticleScale));
    glEnableVertexAttribArray(Shaders::getAttribLocation(Shaders::AttribName::ParticleColor));

    //Since we use a very simple struct using offsetof gets us the position of the attrib
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

    //Update data when rendering new instance- so we use the same quad dimensions and texture coordinates per draw, but not color, scale, or position.
    glVertexAttribDivisor(Shaders::getAttribLocation(Shaders::AttribName::ParticleColor), 1);
    glVertexAttribDivisor(Shaders::getAttribLocation(Shaders::AttribName::ParticleScale), 1);
    glVertexAttribDivisor(Shaders::getAttribLocation(Shaders::AttribName::ParticlePosition), 1);

    glBindVertexArray(0);

    initialized = true;
}

bool Particles::areVitalsNull() {
    if (!currentWorldSettings || !initialized) {
        DBG_LOG("Please initialize particles properly\n");
        return true;
    }
    return false;
}
