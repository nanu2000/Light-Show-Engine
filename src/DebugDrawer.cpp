#include "DebugDrawer.h"

void DebugDrawer::render(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {

    thisShader.useProgram();

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, vertID);
    glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(glm::vec3), &lineVertices[0].x, GL_DYNAMIC_DRAW);

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glEnableVertexAttribArray(positionAttribute);

    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vertID);

    glDrawArrays(GL_LINES, 0, currentAmountOfLines);

    currentAmountOfLines = 0;

    glBindVertexArray(0);
}

void DebugDrawer::initialize() {
    thisShader = ShaderBase("assets/Shaders/Color.v", "assets/Shaders/Color.f", SHADER_TYPE::Default);

    thisShader.useProgram();

    glUniform4fv(Shaders::getUniformLocation(thisShader.getProgramID(), Shaders::UniformName::Color), 1, &GameInfo::DEBUG_COLOR[0]);
    glUniformMatrix4fv(Shaders::getUniformLocation(thisShader.getProgramID(), Shaders::UniformName::ModelMatrix), 1, GL_FALSE, glm::value_ptr(identitym));
    projectionLocation = Shaders::getUniformLocation(thisShader.getProgramID(), Shaders::UniformName::ProjectionMatrix);
    viewLocation       = Shaders::getUniformLocation(thisShader.getProgramID(), Shaders::UniformName::ViewMatrix);
    positionAttribute  = Shaders::getAttribLocation(Shaders::AttribName::Position);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &vertID);
    glBindVertexArray(0);

    lineVertices.resize(MAX_AMOUNT_DEBUG_LINES * 2);

    currentMaxAmountOfVertices = lineVertices.size();
    initialized                = true;
}

DebugDrawer::~DebugDrawer() {

    if (!initialized) {
        return;
    }

    DBG_LOG("Freeing memory for debug drawer.\n");

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vertID);
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {

    if (currentAmountOfLines + 1 < currentMaxAmountOfVertices) {
        lineVertices.at(currentAmountOfLines)     = (glm::vec3(from.getX(), from.getY(), from.getZ()));
        lineVertices.at(currentAmountOfLines + 1) = (glm::vec3(to.getX(), to.getY(), to.getZ()));
        currentAmountOfLines += 2;
    }
}

void DebugDrawer::setDebugMode(int debugMode) {
    currentDebugMode = debugMode;
}

void DebugDrawer::reportErrorWarning(const char* warningString) {
    printf("%s", warningString);
}
