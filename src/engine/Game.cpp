#include "Game.h"

bool Engine::Game::areVitalsNull() {
    if (!currentTime || !backEndMessages) {
        DBG_LOG("ONE OR MORE VITAL OBJECTS IS NULL\n (Game::isVitalsNull())\n");
        return true;
    }

    return false;
}

void Engine::Game::initialize(Time* time, Messenger<BackEndMessages>* backEndMessagingSystem) {

    Entities::registerEntities();

    currentTime     = time;
    backEndMessages = backEndMessagingSystem;

    if (areVitalsNull()) {
        return;
    }

    initializeShaders();

    map.createMap("assets/fonts/courier-new.FontDat");

    loadScene(0);
}

void Engine::Game::initializeShaders() {

    directionalLightDepthMap.initialize();
    pointLightDepthMap.initialize();
    renderTexture.initialize(GameInfo::getWindowWidth(), GameInfo::getWindowHeight());

    Shader::setShaderTask(SHADER_TASK::Normal_Render_Task);
    Shader::setShaderTaskShader(SHADER_TASK::Directional_Depth_Task, directionalLightDepthMap.getDepthMapShader());
    Shader::setShaderTaskShader(SHADER_TASK::Omnidirectional_Depth_Task, pointLightDepthMap.getDepthMapShader());
}

void Engine::Game::loadScene(int index) {

    if (index >= scenes.size()) {
        DBG_LOG("Cannot load scene, index out of bounds\n");
        return;
    }

    currentScene = index;

    //Free old scene and entities.
    delete scene;
    delete physicsWorld;
    freeEntities();

    //Allocate new scene and entities.
    scene        = new Scene();
    physicsWorld = new PhysicsWorld(hh::toBtVec3(GameInfo::DEFAULT_GRAVITY));
    physicsWorld->initialize();

    EntityVitals vitals = EntityVitals(&settings, scene, physicsWorld, &map);

    for (unsigned int i = 0; i < scenes.at(currentScene).size(); i++) {

        EntityWrapper* newEntity = Entities::allocateEntity(scenes.at(currentScene).at(i));

        if (newEntity == nullptr) {
            continue; //Only add to scene if new entity is valid.
        }

        sceneEntities.push_back(newEntity);
    }

    //Initialize new entities
    for (unsigned int i = 0; i < sceneEntities.size(); i++) {
        sceneEntities.at(i)->initialize(vitals);
    }

    //Add rigid bodies to physics world.
    scene->performOperationsOnAllOfType<CollisionMesh>(
        [& world = *physicsWorld](const CollisionMesh& mesh) {
            world.addRigidBody(mesh);
            return false;
        });
    scene->performOperationsOnAllOfType<BoneCollisionMesh>(
        [& world = *physicsWorld](BoneCollisionMesh& bmesh) {
            bmesh.iterateThroughColliders(
                [&](CollisionMesh& mesh, int32_t id) {
                    world.addRigidBody(mesh);
                });

            return false;
        });

    //Lua is initialized before Game::Initialize, so we can safely send LUA_COMPILED to all subsystems.
    //This will replace all default variables in systems/components before they are initialized.
    backEndMessages->addMessage(BackEndMessages::LUA_COMPILED);

    //Send messages to subsystems such as LUA_COMPILED
    //Ran before initializing systems due to lua replacing default C++ variables.
    BackEndMessages msg;
    while (backEndMessages->getMessagesThenRemove(msg)) {
        std::vector<SystemBase*> s = subSystems.getAllSubSystems();
        for (unsigned int i = 0; i < s.size(); i++) {
            s.at(i)->recieveMessage(msg, *scene);
        }
    }

    //Re-init systems.
    fixedUpdatingSystem.initialize(*scene, settings, *physicsWorld, subSystems);
    updatingSystem.initialize(*scene, settings, *physicsWorld, subSystems);
    renderingSystem.initialize(*scene, settings, *physicsWorld, subSystems);
}

void Engine::Game::readBackendEventQueue() {
    BackEndMessages msg;
    while (backEndMessages->getMessagesThenRemove(msg)) {
        updatingSystem.recieveSystemMessage(msg, renderTexture);
    }
}

void Engine::Game::fixedUpdate() {
    if (areVitalsNull()) {
        return;
    }

    fixedUpdatingSystem.fixedUpdate(gameState, *currentTime, pointLightDepthMap, directionalLightDepthMap);
}

void Engine::Game::update() {

    //!Todo: Move- Needs messenger defined in game
    if (InputLocator::getService().isKeyPressedOnce(SDLK_F5)) {
        LuaLocator::getService().recompile();
        backEndMessages->addMessage(BackEndMessages::LUA_COMPILED);
    }

    readBackendEventQueue();
    updatingSystem.update();
}

void Engine::Game::render() {
    if (areVitalsNull()) {
        return;
    }

    renderingSystem.render(pointLightDepthMap, directionalLightDepthMap, *currentTime, renderTexture);
}

void Engine::Game::freeEntities() {

    for (unsigned int i = 0; i < sceneEntities.size(); i++) {
        if (sceneEntities.at(i) == nullptr) {
            continue;
        }
        delete sceneEntities.at(i);
    }
    sceneEntities.clear();
}

void Engine::Game::uninitialize() {
    delete scene;
    delete physicsWorld;
    freeEntities();
}
