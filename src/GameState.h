#ifndef GAME_STATE_H
#define GAME_STATE_H

namespace Engine {
    class Game;
}

/*! 
The GameState class is used as a layer to provide Game functions such as Game::loadScene to classes such as 
FixedUpdatingSystem without exposing sensitive functions like Game::uninitialize or Game::resetVitals. 

Essentially: FixedUpdatingSystem, RenderingSystem, and UpdatingSystem should not know the Game class exists.
*/
class GameState {
public:
    GameState(Engine::Game& game);

    //!Loads a scene via index of the scene to be loaded.
    void loadScene(int sceneIndex);

    //!Gets the index of the current scene
    int getCurrentSceneIndex();

    //!Gets amount of scenes loaded into game
    int getSceneCount();

private:
    Engine::Game* currentGame;
};
#endif
