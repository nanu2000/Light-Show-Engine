#ifndef GAME_H
#define GAME_H
#include "EntityTests.h"
#include "FixedUpdatingSystem.h"
#include "GameState.h"
#include "Messenger.h"
#include "RenderingSystem.h"
#include "Scenes.h"
#include "UpdatingSystem.h"

class Game {

public:
    void initialize(Time* time, Messenger<BackEndMessages>* backEndMessagingSystem);
    void resetVitals(Time* time, Messenger<BackEndMessages>* backEndMessagingSystem);

    void fixedUpdate();
    void render();
    void update();
    void uninitialize();

    void loadScene(int scene);

    inline int getCurrentSceneIndex() const {
        return currentScene;
    }
    inline int getSceneCount() const {
        return scenes.size();
    }

private:
    Messenger<BackEndMessages>* backEndMessages = nullptr;
    Time* currentTime                           = nullptr;
    GameState gameState                         = GameState(*this);

    bool
    areVitalsNull();

    void initializeShaders();
    void readBackendEventQueue();

    void freeEntities();

    Settings settings = Settings(GameInfo::DEFAULT_GRAVITY, glm::vec3(-20, 0, 10));

    PointLightShadowMap pointLightDepthMap;
    DirectionalLightShadowMap directionalLightDepthMap;
    RenderTexture renderTexture;

    UpdatingSystem updatingSystem;
    FixedUpdatingSystem fixedUpdatingSystem;
    RenderingSystem renderingSystem;
    SubSystems subSystems;

    Scene* scene               = new Scene();
    PhysicsWorld* physicsWorld = new PhysicsWorld(hh::toBtVec3(GameInfo::DEFAULT_GRAVITY));

    std::vector<EntityWrapper*> sceneEntities;

    int currentScene = 0;

    TextMap map;
};

#endif
