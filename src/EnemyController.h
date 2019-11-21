#ifndef ENEMY_CONTROLLER_H
#define ENEMY_CONTROLLER_H

#include "Component.h"
#include "GameInfo.h"
#include <SDL_keycode.h>
#include <glm/geometric.hpp>

enum class ENEMY_ANIMATION_STATE : uint8_t {
    Idle = 0
};

enum class ENEMY_ROTATION : uint8_t {
    Towards_Target,
    Unknown
};

enum class ENEMY_MOVEMENT_TYPE : uint8_t {
    Idle_Facing_Player    = 0,
    Idle                  = 1,
    Walk_Random_Direction = 2,
    MAX                   = 3
};

class EnemyController : public Component<EnemyController> {

public:
    ///////////////////////////////////////////////////////////////////////////////
    //Public constructors
    ///////////////////////////////////////////////////////////////////////////////
    EnemyController(const glm::vec3& offsetFromCollisionMesh) { offsetFromCollider = offsetFromCollisionMesh; }
    EnemyController() {}

    ///////////////////////////////////////////////////////////////////////////////
    //Public getters
    ///////////////////////////////////////////////////////////////////////////////
    ENEMY_ANIMATION_STATE getAnimationState() const { return currentState; }
    uint8_t getAnimationStateUint() const { return static_cast<uint8_t>(currentState); }

private:
    glm::vec3 lastKnownTargetPosition = glm::vec3(0, 0, 0);

    ///////////////////////////////////////////////////////////////////////////////
    //Const Members
    ///////////////////////////////////////////////////////////////////////////////

    const float friction                   = 2.0f;
    const float damping                    = 7.0f;
    const float fallMovementDamping        = 2;
    const float halfLengthOfRay            = 2.f;
    const float playerGravityImpulse       = -GameInfo::fixedDeltaTime * 19;
    const float speed                      = GameInfo::fixedDeltaTime * 65;
    const float jumpVelocity               = 11;
    const float jumpEnabledWithinMagnitude = 1;
    const float colliderWidthAndDepth      = .4f;

    ///////////////////////////////////////////////////////////////////////////////
    //Normal members for logic performed in system.
    ///////////////////////////////////////////////////////////////////////////////

    float changeDirectionCooldown = 0;
    float targetRotation          = 0;

    bool isTouchingGround = false;
    bool performJump      = false;
    bool jumping          = false;

    glm::vec3 offsetFromCollider = glm::vec3(0, 0, 0);
    glm::vec3 rayNormal          = glm::vec3(0, 0, 0);

    ENEMY_ANIMATION_STATE currentState   = ENEMY_ANIMATION_STATE::Idle;
    ENEMY_ROTATION currentRotation       = ENEMY_ROTATION::Towards_Target;
    ENEMY_MOVEMENT_TYPE enemyMovmentType = ENEMY_MOVEMENT_TYPE::Idle_Facing_Player;

    //Friend the system for this component
    friend class EnemyControllerSystem;
};

#endif
