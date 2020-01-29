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

    if (InputLocator::getService().keyPressedOnce(SDLK_7)) {

        std::vector<EntityWrapper*> entitiestwo;
        entitiestwo.clear();

        if (hasInit) {
            //Must call init for these objs before deleting. If worldtest is alive, then init was called on these guys so we can re-create them.
            //If not, collision objects will never be inited, and an assert on nullptr will be thrown.

            testScene = Scene();

            playerTesttwo = Player();
            lightTesttwo  = LightTest();

            //must be called before other two.
            delete worldtest;

            delete floortwo;
            delete playerObjecttwo;

        } else {

            floortwo        = new FloorObject();
            playerObjecttwo = new PlayerTestObject();
            worldtest       = new PhysicsWorld(hh::toBtVec3(GameInfo::DEFAULT_GRAVITY));
        }

        floortwo        = new FloorObject();
        playerObjecttwo = new PlayerTestObject();
        worldtest       = new PhysicsWorld(hh::toBtVec3(GameInfo::DEFAULT_GRAVITY));

        entitiestwo.push_back(playerObjecttwo);
        entitiestwo.push_back(&lightTesttwo);
        entitiestwo.push_back(floortwo);
        entitiestwo.push_back(&playerTesttwo);

        EntityWrapper::EntityVitals vitals;
        vitals.currentSettings = &currentSettings;
        vitals.map             = &map;
        vitals.scene           = &testScene;
        vitals.thisWorld       = worldtest;

        for (unsigned int i = 0; i < entitiestwo.size(); i++) {

            entitiestwo.at(i)->initialize(vitals);
        }

        testScene.performOperationsOnAllOfType<CollisionMesh>(
            [& world = *worldtest](const CollisionMesh& mesh) {
                world.addRigidBody(mesh);
                return false;
            });
        testScene.performOperationsOnAllOfType<BoneCollisionMesh>(
            [& world = *worldtest](BoneCollisionMesh& bmesh) {
                bmesh.iterateThroughColliders(
                    [&](CollisionMesh& mesh, int32_t id) {
                        world.addRigidBody(mesh);
                    });

                return false;
            });

        fixedUpdatingSystem = FixedUpdatingSystem();
        renderingSystem     = RenderingSystem();
        updatingSystem      = UpdatingSystem();

        fixedUpdatingSystem.initialize(testScene, currentSettings, *worldtest, subSystems);
        updatingSystem.initialize(testScene, currentSettings, *worldtest, subSystems);
        renderingSystem.initialize(testScene, currentSettings, *worldtest, subSystems);

        hasInit = true;
    }

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
