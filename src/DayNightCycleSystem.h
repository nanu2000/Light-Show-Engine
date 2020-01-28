#ifndef DAY_NIGHT_CYCLE_SYSTEM_H
#define DAY_NIGHT_CYCLE_SYSTEM_H

#include "DebugDrawer.h"
#include "HelpingHand.h"
#include "Lights.h"
#include "Time.h"

class DayNightCycleSystem {

public:
    void initialize() {
        debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
        debugDrawer.initialize();

        nightLightDiffuse = glm::vec3(.1f, .13f, .16f);
        dayLightDiffuse   = glm::vec3(.7f, .73f, .74f);
    }

    void fixedUpdate(DirectionalLight& light, const Time& time) {

        float speed = 1.0f;

        if (InputLocator::getService().getMouseButton(MOUSE_BUTTON::LeftButton)) {
            speed = 20;
        }

        currentRotation = currentRotation + GameInfo::fixedDeltaTime * speed;
        currentRotation = glm::mod(currentRotation, 360.f);

        if (currentRotation >= 90 && currentRotation < 270) {

            sunDown         = !sunDown;
            currentRotation = 270;
            DBG_LOG("sundown %i\n", sunDown);

            if (sunDown) {
                light.diffuse = nightLightDiffuse;

            } else {
                light.diffuse = dayLightDiffuse;
            }
        }

        float rotRadians = glm::radians(currentRotation);

        glm::vec3 dir = hh::sphericalToCartisean(rotRadians, glm::radians(170.f), 1);

        currentDirection = glm::vec3(dir.y, dir.x, dir.z);

        light.direction = currentDirection;
    }

    void debugRender(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {

        btVector3 linePos = btVector3(0, 10, 0);
        float distance    = 5;

        debugDrawer.drawLine(linePos, (btVector3(currentDirection.x, currentDirection.y, currentDirection.z) * distance) + linePos, btVector3(255, 255, 255));

        debugDrawer.render(projectionMatrix, viewMatrix);
    }

private:
    DebugDrawer debugDrawer;

    glm::vec3 currentDirection;

    float currentRotation = 270;

    bool sunDown = false;

    glm::vec3 nightLightDiffuse;
    glm::vec3 dayLightDiffuse;
};

#endif
