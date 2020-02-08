#ifndef GAME_H
#define GAME_H
#include "EntityTests.h"
#include "FixedUpdatingSystem.h"
#include "GameState.h"
#include "Messenger.h"
#include "RenderingSystem.h"
#include "Scenes.h"
#include "UpdatingSystem.h"

namespace Engine {

    //!Handles the Game's physics, scene loading, rendering, and updating. Managed by Application.
    class Game {

    public:
        void initialize(Time* time, Messenger<BackEndMessages>* backEndMessagingSystem);

        void fixedUpdate();
        void render();
        void update();

        void uninitialize();

        //!Loads a scene by index- should be called in fixedUpdate.
        void loadScene(int scene);

        inline int getCurrentSceneIndex() const {
            return currentScene;
        }
        inline int getSceneCount() const {
            return scenes.size();
        }

    private:
        //!Messenger to recieve messages from Application.
        Messenger<BackEndMessages>* backEndMessages = nullptr;
        Time* currentTime                           = nullptr;

        //!Used to provide Game info to UpdatingSystem, FixedUpdatingSystem, and RenderingSystem without exposing the Game
        GameState gameState = GameState(*this);

        //!Checks for null vitals
        bool areVitalsNull();

        void initializeShaders();
        void readBackendEventQueue();

        void freeEntities();

        Settings settings = Settings(GameInfo::DEFAULT_GRAVITY, glm::vec3(-20, 0, 10));

        //Todo: Components?
        PointLightShadowMap pointLightDepthMap;
        DirectionalLightShadowMap directionalLightDepthMap;
        RenderTexture renderTexture;

        //!The main system used for updating the game based on the Scene's components
        UpdatingSystem updatingSystem;

        //!The main system used for fixed updating the game based on the Scene's components
        FixedUpdatingSystem fixedUpdatingSystem;

        //!The main system used for rendering game the game based on the Scene's components
        RenderingSystem renderingSystem;

        //!Small systems that are used inside of the UpdatingSystem, FixedUpdatingSystem, and RenderingSystem.
        SubSystems subSystems;

        //!Current scene
        Scene* scene = new Scene();

        //!Current PhysicsWorld used along with the Scene
        PhysicsWorld* physicsWorld = new PhysicsWorld(hh::toBtVec3(GameInfo::DEFAULT_GRAVITY));

        std::vector<EntityWrapper*> sceneEntities;

        //!The current scene index
        int currentScene = 0;

        //Todo: Move to component?
        //!Font atlas to be used in the game.
        TextMap map;
    };
}
#endif
