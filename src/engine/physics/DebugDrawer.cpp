#include "DebugDrawer.h"

void Engine::DebugDrawer::render(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {

    if (!initialized) {
        DBG_LOG("Engine::DebugDrawer::render Must initialize before rendering! Note: this object should only be used and initialized with the current physics world.\n");
        return;
    }

    thisShader.useProgram();
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, vertID);
    glBufferData(GL_ARRAY_BUFFER, currentMaxAmountOfVertices * sizeof(glm::vec3), &lineVertices[0].x, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(positionAttribute);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, colorID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * currentMaxAmountOfVertices, &lineColors[0].x, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(colorAttribute);
    glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glLineWidth(DBG_DRAWER::DEBUG_LINE_WIDTH);

    glDrawArrays(GL_LINES, 0, currentAmountOfLines);

    currentAmountOfLines = 0;

    glBindVertexArray(0);
}

void Engine::DebugDrawer::initialize() {
    thisShader = ShaderLocator::getService().getShader("dbg", "assets/shaders/color.vert", "assets/shaders/color.frag", SHADER_TYPE::Default);

    thisShader.useProgram();

    glUniformMatrix4fv(Shaders::getUniformLocation(thisShader.getProgramID(), Shaders::UniformName::ModelMatrix), 1, GL_FALSE, glm::value_ptr(identitym));
    projectionLocation = Shaders::getUniformLocation(thisShader.getProgramID(), Shaders::UniformName::ProjectionMatrix);
    viewLocation       = Shaders::getUniformLocation(thisShader.getProgramID(), Shaders::UniformName::ViewMatrix);
    positionAttribute  = Shaders::getAttribLocation(Shaders::AttribName::Position);
    colorAttribute     = Shaders::getAttribLocation(Shaders::AttribName::Color);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &vertID);
    glGenBuffers(1, &colorID);
    glBindVertexArray(0);

    currentMaxAmountOfVertices = DBG_DRAWER::MAX_AMOUNT_DEBUG_LINES;
    initialized                = true;
}

Engine::DebugDrawer::~DebugDrawer() {

    if (!initialized) {
        return;
    }

    DBG_LOG("Freeing memory for debug drawer.\n");

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vertID);
    glDeleteBuffers(1, &colorID);
}

void Engine::DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    drawLine(from, to, color, color);
}

void Engine::DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& colorStart, const btVector3& colorEnd) {
    if (currentAmountOfLines + 1 < DBG_DRAWER::MAX_AMOUNT_DEBUG_LINES) {
        lineVertices[(currentAmountOfLines)]     = (glm::vec3(from.getX(), from.getY(), from.getZ()));
        lineVertices[(currentAmountOfLines + 1)] = (glm::vec3(to.getX(), to.getY(), to.getZ()));
        lineColors[currentAmountOfLines]         = hh::toGlmVec3(colorStart);
        lineColors[currentAmountOfLines + 1]     = hh::toGlmVec3(colorEnd);
        currentAmountOfLines += 2;
    }
}

void Engine::DebugDrawer::setDebugMode(int debugMode) {
    currentDebugMode = debugMode;
}

void Engine::DebugDrawer::reportErrorWarning(const char* warningString) {
    printf("%s", warningString);
}
