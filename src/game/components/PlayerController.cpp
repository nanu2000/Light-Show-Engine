#include "PlayerController.h"

float PlayerController::getResistance() const {
    if (isTouchingGround) {
        return (friction + damping) * GameInfo::fixedDeltaTime;
    }

    if (jumping) {
        return damping * GameInfo::fixedDeltaTime;
    }

    return (damping + fallMovementDamping) * GameInfo::fixedDeltaTime;
}

bool PlayerController::canJump(const glm::vec3& lastPosition) const {
    return performJump == false && jumping == false && (isTouchingGround || glm::distance(lastTouchedPosition, lastPosition) < jumpEnabledWithinMagnitude);
}
