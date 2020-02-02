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

    vitals.currentSettings = &settings;
    vitals.map             = &map;
    vitals.scene           = scene;
    vitals.thisWorld       = physicsWorld;

    for (unsigned int i = 0; i < sceneOneEntities.size(); i++) {
        sceneOneEntities[i]->initialize(vitals);
    }

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

    fixedUpdatingSystem.initialize(*scene, settings, *physicsWorld, subSystems);
    updatingSystem.initialize(*scene, settings, *physicsWorld, subSystems);
    renderingSystem.initialize(*scene, settings, *physicsWorld, subSystems);
}

void Game::initializeShaders() {

    directionalLightDepthMap.initialize();
    pointLightDepthMap.initialize();
    renderTexture.initialize(GameInfo::getWindowWidth(), GameInfo::getWindowHeight());

    Shader::setShaderTask(SHADER_TASK::Normal_Render_Task);
    Shader::setShaderTaskShader(SHADER_TASK::Directional_Depth_Task, directionalLightDepthMap.getDepthMapShader());
    Shader::setShaderTaskShader(SHADER_TASK::Omnidirectional_Depth_Task, pointLightDepthMap.getDepthMapShader());
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

    if (InputLocator::getService().keyPressedOnce(SDLK_8)) {

        Quad x[100];
        for (int i = 0; i < 100; i++) {
            x[i].render3D(
                ShaderLocator::getService().getShader("ui", settings, "assets/shaders/gui-shader.vert", "assets/shaders/gui-shader.frag", SHADER_TYPE::GUI),
                TextureLocator::getService().getTexture("assets/Fonts/CourierNew.png", GL_NEAREST).getTextureData(),
                glm::mat4(1));
        }
    }
    if (InputLocator::getService().keyPressedOnce(SDLK_7)) {

        delete scene;
        delete physicsWorld;

        scene        = new Scene();
        physicsWorld = new PhysicsWorld(hh::toBtVec3(GameInfo::DEFAULT_GRAVITY));

        if (!hasInit) {

            sceneTwoEntities[0] = new Player();
            sceneTwoEntities[1] = new Player();
            sceneTwoEntities[2] = new Player();
            sceneTwoEntities[3] = new Player();

            hasInit = true;
        } else {

            delete sceneTwoEntities[0];
            delete sceneTwoEntities[1];
            delete sceneTwoEntities[2];
            delete sceneTwoEntities[3];

            sceneTwoEntities[0] = new Player();
            sceneTwoEntities[1] = new PlayerTestObject();
            sceneTwoEntities[2] = new FloorObject();
            sceneTwoEntities[3] = new LightTest();
        }

        EntityWrapper::EntityVitals vitals;
        vitals.currentSettings = &settings;
        vitals.map             = &map;
        vitals.scene           = scene;
        vitals.thisWorld       = physicsWorld;

        for (unsigned int i = 0; i < sceneTwoEntities.size(); i++) {
            sceneTwoEntities[i]->initialize(vitals);
        }

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

        fixedUpdatingSystem.initialize(*scene, settings, *physicsWorld, subSystems);
        updatingSystem.initialize(*scene, settings, *physicsWorld, subSystems);
        renderingSystem.initialize(*scene, settings, *physicsWorld, subSystems);
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
    for (unsigned int i = 0; i < sceneOneEntities.size(); i++) {
        delete sceneOneEntities[i];
        sceneOneEntities[i] = nullptr;
    }
    for (unsigned int i = 0; i < sceneTwoEntities.size(); i++) {
        delete sceneTwoEntities[i];
        sceneTwoEntities[i] = nullptr;
    }
}
