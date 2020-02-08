#include "GameInfo.h"

float PrivateGameInfo::deltaTime = 0.0f;
float GameInfo::getDeltaTime() {
    return PrivateGameInfo::deltaTime;
}
