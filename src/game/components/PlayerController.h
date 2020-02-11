#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H
#include "Component.h"
#include "GameInfo.h"
#include "RayCaster.h"
#include <SDL_keycode.h>
#include <glm/geometric.hpp>

//The state resembles the players movement and the current animation that should play along with it
//PA_XXX 2 - where XXX is the movment state and 2 is the animation that should play at that state.
enum class PLAYER_ANIMATION_STATE : uint8_t {
    All     = 0,
    Idle    = 1,
    Walking = 2
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
    static const unsigned int AMOUNT_OF_RAYS                = 8;

    float clampingVelocity     = -5;
    float rayDistanceCorrector = -0.1f;

    RayCaster rayCast[PlayerController::AMOUNT_OF_RAYS];

    PLR_CTRLR_NS::SlopeIntensity slopes[MAX_AMOUNT_SLOPE_CALCULATIONS] = {
        PLR_CTRLR_NS::SlopeIntensity(10, .2f), //Must be in incrementing order.
        PLR_CTRLR_NS::SlopeIntensity(30, .6f), //+
        PLR_CTRLR_NS::SlopeIntensity(80, .8f) //++
    };

    float friction                   = 2.0f;
    float damping                    = 7.0f;
    float fallMovementDamping        = 2;
    float halfLengthOfRay            = 2.f;
    float playerGravityImpulse       = -GameInfo::fixedDeltaTime * 19;
    float speed                      = GameInfo::fixedDeltaTime * 100;
    float jumpVelocity               = 20;
    float jumpEnabledWithinMagnitude = 1;
    float colliderWidthAndDepth      = .4f;

    ///////////////////////////////////////////////////////////////////////////////
    //Helper Functions
    ///////////////////////////////////////////////////////////////////////////////
    float getResistance() const;
    bool canJump(const glm::vec3& lastPosition) const;

    ///////////////////////////////////////////////////////////////////////////////
    //Normal members for logic performed in system.
    ///////////////////////////////////////////////////////////////////////////////
    bool isTouchingGround               = false;
    bool performJump                    = false;
    bool jumping                        = false;
    bool rotateAwayFromCamera           = false;
    glm::vec3 lastTouchedPosition       = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 offsetFromCollider        = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rayNormal                 = glm::vec3(0.0f, 0.0f, 0.0f);
    btVector3 currentSlopeIntensity     = btVector3(0.0f, 0.0f, 0.0f);
    PLAYER_ANIMATION_STATE currentState = PLAYER_ANIMATION_STATE::Idle;

    //Friend the system for this component
    friend class PlayerControllerSystem;
};

#endif
