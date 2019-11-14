#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H
#include "Component.h"
#include "GameInfo.h"
#include <SDL_keycode.h>
#include <glm/geometric.hpp>

//The state resembles the players movement and the current animation that should play along with it
//PA_XXX 2 - where XXX is the movment state and 2 is the animation that should play at that state.
enum class PLAYER_ANIMATION_STATE : uint8_t {
    Walking = 3,
    Jumping = 5,
    Idle    = 6
};

namespace PLR_CTRLR_NS {
struct SlopeIntensity {
    SlopeIntensity(float gravity, float depth) {
        slopeGravity = gravity;
        slopeAmount  = depth;
    }

    float slopeGravity = 0;
    float slopeAmount  = 0;
};
}

class PlayerController : public Component<PlayerController> {

public:
    ///////////////////////////////////////////////////////////////////////////////
    //Public constructors
    ///////////////////////////////////////////////////////////////////////////////
    PlayerController(const glm::vec3& offsetFromCollisionMesh) { offsetFromCollider = offsetFromCollisionMesh; }
    PlayerController() {}

    ///////////////////////////////////////////////////////////////////////////////
    //Public getters
    ///////////////////////////////////////////////////////////////////////////////
    PLAYER_ANIMATION_STATE getAnimationState() const { return currentState; }
    uint8_t getAnimationStateUint() const { return static_cast<uint8_t>(currentState); }

private:
    ///////////////////////////////////////////////////////////////////////////////
    //Const Members
    ///////////////////////////////////////////////////////////////////////////////
    static const unsigned int MAX_AMOUNT_SLOPE_CALCULATIONS = 3;

    const PLR_CTRLR_NS::SlopeIntensity slopes[MAX_AMOUNT_SLOPE_CALCULATIONS] = {
        PLR_CTRLR_NS::SlopeIntensity(10, .2f), //Must be in incrementing order.
        PLR_CTRLR_NS::SlopeIntensity(30, .6f), //+
        PLR_CTRLR_NS::SlopeIntensity(80, .8f) //++
    };

    const float friction                   = 2.0f;
    const float damping                    = 7.0f;
    const float fallMovementDamping        = 2;
    const float halfLengthOfRay            = 2.f;
    const float playerGravityImpulse       = -GameInfo::deltaTime * 19;
    const float speed                      = GameInfo::deltaTime * 100;
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
    bool isTouchingGround     = false;
    bool performJump          = false;
    bool jumping              = false;
    bool rotateAwayFromCamera = false;
    glm::vec3 lastTouchedPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 offsetFromCollider = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rayNormal = glm::vec3(0.0f, 0.0f, 0.0f);
    btVector3 currentSlopeIntensity = btVector3(0.0f, 0.0f, 0.0f);
    PLAYER_ANIMATION_STATE currentState = PLAYER_ANIMATION_STATE::Idle;

    //Friend the system for this component
    friend class PlayerControllerSystem;
};

#endif
