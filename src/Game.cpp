#include "Game.h"

void Game::resetVitals(Time* time, Messenger<BackEndMessages>* backEndMessagingSystem) {
    currentTime     = time;
    backEndMessages = backEndMessagingSystem;
}

bool Game::areVitalsNull() {
    if (!currentTime || !backEndMessages) {
        DBG_LOG("ONE OR MORE VITAL OBJECTS IS NULL\n (Game::isVitalsNull())\n");
        return true;
    }

    return false;
}

void Game::initialize(Time* time, Messenger<BackEndMessages>* backEndMessagingSystem) {

    currentTime     = time;
    backEndMessages = backEndMessagingSystem;

    if (areVitalsNull()) {
        return;
    }

    initializeShaders();

    map.createMap("assets/Fonts/CourierNew.FontDat");

    EntityWrapper::EntityVitals vitals;

    vitals.currentSettings = &currentSettings;
    vitals.map             = &map;
    vitals.scene           = &currentScene;
    vitals.thisWorld       = &physicsWorld;
    vitals.worldSettings   = &worldSettings;

    std::vector<EntityWrapper*> entities;

    entities.push_back(&playerObject);
    entities.push_back(&lightTest);
    entities.push_back(&floor);
    entities.push_back(&cube);
    entities.push_back(&particles);
    entities.push_back(&enemyObject);
    entities.push_back(&playerTest);

    for (unsigned int i = 0; i < entities.size(); i++) {
        entities[i]->initialize(vitals);
    }

    Allentity = currentScene.getAllEntities();

    currentScene.performOperationsOnAllOfType<CollisionMesh>(
        [& world = physicsWorld](const CollisionMesh& mesh) {
            world.addRigidBody(mesh);
            return false;
        });
    currentScene.performOperationsOnAllOfType<BoneCollisionMesh>(
        [& world = physicsWorld](BoneCollisionMesh& bmesh) {
            bmesh.iterateThroughColliders(
                [&](CollisionMesh& mesh, int32_t id) {
                    world.addRigidBody(mesh);
                });

            return false;
        });

    fixedUpdatingSystem.initialize(currentScene, currentSettings, physicsWorld, subSystems);
    updatingSystem.initialize(currentScene, currentSettings, physicsWorld, subSystems);
    renderingSystem.initialize(currentScene, currentSettings, physicsWorld, subSystems);
}

void Game::initializeShaders() {

    directionalLightDepthMap.initialize();
    pointLightDepthMap.initialize();
    renderTexture.initialize(GameInfo::getWindowWidth(), GameInfo::getWindowHeight());

    ShaderBase::setShaderTask(SHADER_TASK::Normal_Render_Task);
    ShaderBase::setShaderTaskShader(SHADER_TASK::Directional_Depth_Task, directionalLightDepthMap.getDepthMapShader());
    ShaderBase::setShaderTaskShader(SHADER_TASK::Omnidirectional_Depth_Task, pointLightDepthMap.getDepthMapShader());
}

void Game::readBackendEventQueue() {
    BackEndMessages msg;
    while (backEndMessages->getMessagesThenRemove(msg)) {
        fixedUpdatingSystem.handleBackEndMessage(msg, renderTexture);
    }
}

void Game::fixedUpdate() {
    if (areVitalsNull()) {
        return;
    }

    //Event Queue
    readBackendEventQueue();

    fixedUpdatingSystem.fixedUpdate(*currentTime, pointLightDepthMap, directionalLightDepthMap);
}

void Game::update() {
    updatingSystem.update();
}

void Game::render() {
    if (areVitalsNull()) {
        return;
    }

    renderingSystem.render(pointLightDepthMap, directionalLightDepthMap, *currentTime, renderTexture);
}

void Game::uninitialize() {
    physicsWorld.removeAllRigidBodies();
}
