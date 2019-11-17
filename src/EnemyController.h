#ifndef ENEMY_CONTROLLER_H
#define ENEMY_CONTROLLER_H

#include "Component.h"
#include "GameInfo.h"
#include <SDL_keycode.h>
#include <glm/geometric.hpp>

enum class ENEMY_ANIMATION_STATE : uint8_t {
    Walking = 3,
    Jumping = 5,
    Idle    = 6
};

enum class ENEMY_ROTATION : uint8_t {
    Towards_Target,
    Away_From_Target,
    Away_From_Last_Target,
    Random_Changing_Direction,
    Unknown
};

namespace ENEMY_CTRLR_NS {
struct SlopeIntensity {
    SlopeIntensity(float gravity, float depth) {
        slopeGravity = gravity;
        slopeAmount  = depth;
    }

    float slopeGravity = 0;
    float slopeAmount  = 0;
};
}

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
    glm::vec3 lastKnownTargetPosition;

    ///////////////////////////////////////////////////////////////////////////////
    //Const Members
    ///////////////////////////////////////////////////////////////////////////////
    static const unsigned int MAX_AMOUNT_SLOPE_CALCULATIONS = 3;

    const ENEMY_CTRLR_NS::SlopeIntensity slopes[MAX_AMOUNT_SLOPE_CALCULATIONS] = {
        ENEMY_CTRLR_NS::SlopeIntensity(10, .2f), //Must be in incrementing order.
        ENEMY_CTRLR_NS::SlopeIntensity(30, .6f), //+
        ENEMY_CTRLR_NS::SlopeIntensity(80, .8f) //++
    };

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
    //Helper Functions
    ///////////////////////////////////////////////////////////////////////////////
    float getResistance() const;
    bool canJump(const glm::vec3& lastPosition) const;

    ///////////////////////////////////////////////////////////////////////////////
    //Normal members for logic performed in system.
    ///////////////////////////////////////////////////////////////////////////////

    float changeDirectionCooldown = 0;
    float targetRotation          = 0;

    bool isTouchingGround = false;
    bool performJump      = false;
    bool jumping          = false;
    glm::vec3 lastTouchedPosition;
    glm::vec3 offsetFromCollider;
    glm::vec3 rayNormal;
    btVector3 currentSlopeIntensity;
    ENEMY_ANIMATION_STATE currentState = ENEMY_ANIMATION_STATE::Idle;
    ENEMY_ROTATION currentRotation = ENEMY_ROTATION::Towards_Target;

    //Friend the system for this component
    friend class EnemyControllerSystem;
};

#endif
