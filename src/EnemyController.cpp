#include "EnemyController.h"

float EnemyController::getResistance() const {
    if (isTouchingGround) {
        return (friction + damping) * GameInfo::fixedDeltaTime;
    }

    if (jumping) {
        return damping * GameInfo::fixedDeltaTime;
    }

    return (damping + fallMovementDamping) * GameInfo::fixedDeltaTime;
}

bool EnemyController::canJump(const glm::vec3& lastPosition) const {
    return performJump == false && jumping == false && (isTouchingGround || glm::distance(lastTouchedPosition, lastPosition) < jumpEnabledWithinMagnitude);
}
