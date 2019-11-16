#ifndef TEST_ENEMY_AI
#define TEST_ENEMY_AI
#include "Component.h"
#include "EntityTransform.h"
#include "GlobalInformation.h"
#include "Input.h"

enum class ENEMY_MOVEMENT_TYPE : uint8_t {
    Walk_Towards_Player                       = 0,
    Walk_Away_From_Player                     = 1,
    Walk_Away_From_Last_Known_Player_Position = 2,
    Strafe_Right_From_Player                  = 3,
    Strafe_Left_From_Player                   = 4,
    Idle_Facing_Player                        = 5,
    Idle                                      = 6,
    Run_Towards_Player                        = 7,
    Run_Away_From_Player                      = 8,

    Strafe_Right_And_Move_Towards_Player = 9,
    Strafe_Left_And_Move_Towards_Player  = 10,

    Walk_Random_Direction = 11,
    MAX                   = 12
};

class TestEnemyAI : public Component<TestEnemyAI> {

public:
    ENEMY_MOVEMENT_TYPE getMovementType() const { return currentMovement; }

    TestEnemyAI() {
        fieldOfViewPreCos = cos(glm::radians(fieldOfView));
    }

    void update(GlobalInformation& globalInfo, Input& input, EntityTransform& entityTransform) {

        glm::vec3 rot = glm::normalize(entityTransform.transform.rotation * glm::vec3(0, 1, 0));

        rot = glm::vec3(-rot.x, 0, -rot.z);

        if (hh::isPointWithinConePreCos(entityTransform.transform.position, rot, globalInfo.getPlayersPosition(), fieldOfViewPreCos, fieldOfViewDistance)) {
            //printf("hi!");
        }

        if (input.keyPressedOnce(SDLK_0)) {
            currentMovement = static_cast<ENEMY_MOVEMENT_TYPE>((uint8_t)currentMovement + 1);
        }
        if (input.isKeyDown(SDLK_9)) {
            currentMovement = ENEMY_MOVEMENT_TYPE::Idle_Facing_Player;
        }

        if (currentMovement > ENEMY_MOVEMENT_TYPE::MAX) {
            currentMovement = static_cast<ENEMY_MOVEMENT_TYPE>(0);
        }
    }

    float getChangeInDirectionForEveryXSeconds() const { return changeDirectionEveryXSeconds; }

private:
    float changeDirectionEveryXSeconds = .75f;
    const float fieldOfViewDistance    = 10.0f;
    const float fieldOfView            = 90.0f;
    float fieldOfViewPreCos            = 0;

    ENEMY_MOVEMENT_TYPE currentMovement = ENEMY_MOVEMENT_TYPE::Idle_Facing_Player;
};

#endif
