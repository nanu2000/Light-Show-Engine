#include "GameState.h"
#include "Game.h"
GameState::GameState(Game& game) {
    currentGame = &game;
}

void GameState::loadScene(int sceneIndex) {
    if (currentGame == nullptr) {
        return;
    }
    currentGame->loadScene(sceneIndex);
}

int GameState::getCurrentSceneIndex() {
    return currentGame->getCurrentSceneIndex();
}

int GameState::getSceneCount() {
    return currentGame->getSceneCount();
}
