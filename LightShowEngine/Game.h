#ifndef GAME_H
#define GAME_H
#include "FixedUpdatingSystem.h"
#include "GameObjectTests.h"
#include "Messenger.h"
#include "RenderingSystem.h"

class Game {

public:
    void initialize(Time* time, Messenger<BackEndMessages>* backEndMessagingSystem);
    void resetVitals(Time* time, Messenger<BackEndMessages>* backEndMessagingSystem);

    void fixedUpdate();
    void render();
    void uninitialize();

private:
    Messenger<BackEndMessages>* backEndMessages = nullptr;
    Time* currentTime                           = nullptr;

    bool areVitalsNull();

    void initializeShaders();
    void renderOmnidirectionalDepthMap();
    void renderDirectionalDepthMap();
    void readBackendEventQueue();

    WorldSettings worldSettings = WorldSettings(GameInfo::DEFAULT_GRAVITY, glm::vec3(-5, 0, 0));
    PhysicsWorld physicsWorld   = PhysicsWorld(hh::toBtVec3(GameInfo::DEFAULT_GRAVITY));

    Scene currentScene;
    Settings currentSettings;

    PointLightShadowMap pointLightDepthMap;
    DirectionalLightShadowMap directionalLightDepthMap;

    FixedUpdatingSystem updatingSystem;
    RenderingSystem renderingSystem;
    SubSystems subSystems;

    const static unsigned int AMOUNT_OF_ENTITIES = 6;

    Player playerTest             = Player();
    LightTest lightTest           = LightTest();
    PlayerTestObject playerObject = PlayerTestObject();
    EnemyTestObject enemyObject   = EnemyTestObject();
    FloorObject floor             = FloorObject();
    CubeTrigger cube              = CubeTrigger();
    ParticleTest particles        = ParticleTest();

    TextMap map;

    const std::vector<Scene::GameObject>* AllGameObjects;
};

#endif
