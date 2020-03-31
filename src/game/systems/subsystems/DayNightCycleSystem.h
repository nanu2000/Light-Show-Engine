#ifndef DAY_NIGHT_CYCLE_SYSTEM_H
#define DAY_NIGHT_CYCLE_SYSTEM_H

#include "DebugDrawer.h"
#include "HelpingHand.h"
#include "LTime.h"
#include "Lights.h"
#include "PhysicsWorld.h"
#include "SystemBase.h"

class DayNightCycleSystem : public SystemBase {

public:
    virtual void recieveMessage(const BackEndMessages& msg, Scene& currentScene) override {

        if (msg == BackEndMessages::LUA_COMPILED) {
            if (LuaLocator::getService().canUseFunction("day_night_cycle_init")) {
                sol::function day_night_cycle_init = LuaLocator::getService().getFunction("day_night_cycle_init");
                sol::table t                       = day_night_cycle_init();

                speed     = t["speed"];
                fastSpeed = t["fast_speed"];

                nightLightDiffuse = glm::vec3(t["night_light_diffuse"]["x"], t["night_light_diffuse"]["y"], t["night_light_diffuse"]["z"]);
                dayLightDiffuse   = glm::vec3(t["day_light_diffuse"]["x"], t["day_light_diffuse"]["y"], t["day_light_diffuse"]["z"]);
            }
        }
    }

    //TODO: where should init be called from???
    void initialize(DirectionalLight& light) {

        //Lua updates the color, but the light doesn't update until full rotation.
        light.diffuse = dayLightDiffuse;
    }

    void fixedUpdate(DirectionalLight& light, const Time& time) {

        float currentSpeed = speed;

        if (InputLocator::getService().isMouseButtonPressed(MOUSE_BUTTON::LeftButton)) {
            currentSpeed = fastSpeed;
        }

        currentRotation = currentRotation + GameInfo::fixedDeltaTime * currentSpeed;
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

        glm::vec3 dir = hh::sphericalToCartisean(rotRadians, glm::radians(180.f), 1);

        currentDirection = glm::vec3(dir.y, dir.x, dir.z);

        light.direction = currentDirection;
    }

    void debugRender(PhysicsWorld& physicsWorld) {

        if (physicsWorld.isDebugDrawing()) {

            btVector3 linePos = btVector3(0, 10, 0);
            float distance    = 5;

            glm::vec3 c  = glm::normalize(glm::cross(hh::toGlmVec3(linePos), currentDirection));
            glm::vec3 c2 = glm::normalize(glm::cross(c, currentDirection));

            physicsWorld.getDebugDrawer().drawLine(linePos, (btVector3(currentDirection.x, currentDirection.y, currentDirection.z) * distance) + linePos, btVector3(1, 1, 1));
            physicsWorld.getDebugDrawer().drawLine(linePos, hh::toBtVec3(c) + linePos, btVector3(1, 1, 1), btVector3(0, 0, 0));
            physicsWorld.getDebugDrawer().drawLine(linePos, hh::toBtVec3(c2) + linePos, btVector3(1, 1, 1), btVector3(0, 0, 0));

            //could also do:
            //physicsWorld.getDebugDrawer().drawTransform(btTransform);
        }
    }

private:
    glm::vec3 currentDirection;

    float currentRotation = 270;
    bool sunDown          = false;

    float speed     = 1;
    float fastSpeed = 50;

    glm::vec3 nightLightDiffuse = glm::vec3(.1f, .13f, .16f);
    glm::vec3 dayLightDiffuse   = glm::vec3(.7f, .73f, .74f);
};

#endif
