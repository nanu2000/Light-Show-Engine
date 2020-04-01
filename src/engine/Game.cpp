#include "Game.h"

bool Engine::Game::areVitalsNull() {
    if (!currentTime || !backEndMessages) {
        DBG_LOG("ONE OR MORE VITAL OBJECTS IS NULL\n (Game::isVitalsNull())\n");
        return true;
    }

    return false;
}

void Engine::Game::initialize(Time& time, Messenger<BackEndMessages>& backEndMessagingSystem) {

    Entities::registerEntities();

    currentTime     = &time;
    backEndMessages = &backEndMessagingSystem;

    systemVitals = new SystemVitals(*currentTime, *physicsWorld, gameState);

    if (areVitalsNull()) {
        return;
    }

    initializeShaders();

    systemVitals->initializeTextMaps();

    loadScene(0);
}

void Engine::Game::initializeShaders() {

    systemVitals->initializeDepthMaps();

    //Start game with normal shader draw call..
    Shader::setShaderTask(SHADER_TASK::Normal_Render_Task);
}

void Engine::Game::loadScene(int index) {

    if (index >= scenes.size()) {
        DBG_LOG("Cannot load scene, index out of bounds\n");
        return;
    }

    currentScene = index;

    //Todo: Possibly move this logic to SystemVitals
    //Free old scene and entities.
    delete scene;
    delete physicsWorld;
    freeEntities();

    //Allocate new scene and entities.
    scene        = new Scene();
    physicsWorld = new PhysicsWorld(hh::toBtVec3(GameInfo::DEFAULT_GRAVITY));
    physicsWorld->initialize();

    //Provide new references to SystemVitals.
    systemVitals->resupply(*currentTime, *physicsWorld, gameState);

    //TODO: replace EntityVitals with SystemVitals.
    EntityVitals vitals = EntityVitals(&systemVitals->getSettings(), scene, physicsWorld, &systemVitals->getTextMap());

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
    //TODO: remove after adding init function to systembase.

    std::vector<SystemBase*> subSystemsAsBase = subSystems.getAllSubSystems();

    BackEndMessages msg;
    while (backEndMessages->getMessagesThenRemove(msg)) {
        for (unsigned int i = 0; i < subSystemsAsBase.size(); i++) {
            subSystemsAsBase.at(i)->recieveMessage(msg, *scene, *systemVitals);
        }
    }

    //Re-init systems.
    fixedUpdatingSystem.initialize(*scene, *systemVitals, subSystems);
    updatingSystem.initialize(*scene, *systemVitals, subSystems);
    renderingSystem.initialize(*scene, *systemVitals, subSystems);

    for (unsigned int i = 0; i < subSystemsAsBase.size(); i++) {
        subSystemsAsBase.at(i)->initialize(*scene, *systemVitals);
    }

    /*
    TODO: Add system base initializing for subsystems.
    Camera and daynight init should be added into initializing hook
    Provide scene and container holding:
    pointLightDepthMap
    directionalLightDepthMap
    currentTime
    gameState
    renderTexture.

    Provide the stame container to:
    fixedUpdatingSystem.fixedUpdate,
    updatingSystem.update
    renderingSystem.render
    to lessen function args required.



    */
}

void Engine::Game::readBackendEventQueue() {
    BackEndMessages msg;
    while (backEndMessages->getMessagesThenRemove(msg)) {
        updatingSystem.recieveSystemMessage(msg, *systemVitals);
    }
}

void Engine::Game::fixedUpdate() {
    if (areVitalsNull()) {
        return;
    }

    fixedUpdatingSystem.fixedUpdate(*systemVitals);
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

    renderingSystem.render(*systemVitals);
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
    delete systemVitals;
    freeEntities();
}
