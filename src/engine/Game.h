#ifndef GAME_H
#define GAME_H
#include "EntityTests.h"
#include "FixedUpdatingSystem.h"
#include "GameState.h"
#include "Messenger.h"
#include "RenderingSystem.h"
#include "SystemVitals.h"
#include "UpdatingSystem.h"

namespace Engine {

    //!Handles the Game's physics, scene loading, rendering, and updating. Managed by Application.
    class Game {

    public:
        void initialize(Time& time, Messenger<BackEndMessages>& backEndMessagingSystem);

        void fixedUpdate();
        void render();
        void update();

        void uninitialize();

        //!Loads a scene by index- should be called in fixedUpdate.
        void loadScene(unsigned int scene);

        inline int getCurrentSceneIndex() const {
            return currentScene;
        }
        inline int getSceneCount() const {
            return scenes.size();
        }

    private:
        void retrieveScenes() {

            if (LuaLocator::getService().canUseFunction("get_scenes")) {

                try {
                    sol::function f = LuaLocator::getService().getFunction("get_scenes");

                    sol::table t = f();

                    t.for_each([&](sol::object const& k, sol::table const& v) {
                        std::vector<std::string> scene;
                        v.for_each([&](sol::object const& key, sol::object const& value) {
                            scene.push_back(value.as<std::string>());
                        });
                        scenes.push_back(scene);
                    });

                } catch (const std::exception&) {
                }
            }
        }

        //!Messenger to recieve messages from Application.
        Messenger<BackEndMessages>* backEndMessages = nullptr;

        //!Checks for null vitals
        bool areVitalsNull();

        void initializeShaders();

        //Notifies systems of backend events such as camera refresh or Lua compile.
        void readBackendEventQueue();

        void freeEntities();

        //!Current scene
        Scene* scene = new Scene();

        //!Used to provide Game info to UpdatingSystem, FixedUpdatingSystem, and RenderingSystem without exposing the Game
        GameState gameState = GameState(*this);

        Time* currentTime = nullptr;

        //!Current PhysicsWorld used along with the Scene
        PhysicsWorld* physicsWorld = new PhysicsWorld(hh::toBtVec3(GameInfo::DEFAULT_GRAVITY));

        //!Used to wrap important objects that need to be provided to the systems.
        SystemVitals* systemVitals = nullptr;

        //!The main system used for updating the game based on the Scene's components
        UpdatingSystem updatingSystem;

        //!The main system used for fixed updating the game based on the Scene's components
        FixedUpdatingSystem fixedUpdatingSystem;

        //!The main system used for rendering game the game based on the Scene's components
        RenderingSystem renderingSystem;

        //!Small systems that are used inside of the UpdatingSystem, FixedUpdatingSystem, and RenderingSystem.
        SubSystems subSystems;

        std::vector<EntityWrapper*> sceneEntities;

        //!The current scene index
        int currentScene = 0;

        std::vector<std::vector<std::string>> scenes = {
            { "Player",
              "LightTest",
              "PlayerTestObject",
              "EnemyTestObject",
              "FloorObject",
              "ParticleTest" }
        };
    };
}
#endif
