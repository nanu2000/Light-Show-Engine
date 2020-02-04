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

    initMap();

    for (unsigned int i = 0; i < sceneOne.size(); i++) {
        sceneOneEntities.push_back(getEntity(sceneOne.at(i)));
    }

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

    if (InputLocator::getService().keyPressedOnce(SDLK_7)) {

        delete scene;
        delete physicsWorld;

        scene        = new Scene();
        physicsWorld = new PhysicsWorld(hh::toBtVec3(GameInfo::DEFAULT_GRAVITY));

        if (!hasInit) {

            for (unsigned int i = 0; i < sceneTwo.size(); i++) {
                sceneTwoEntities.push_back(getEntity(sceneTwo.at(i)));
            }

            hasInit = true;
        } else {

            for (unsigned int i = 0; i < sceneTwoEntities.size(); i++) {
                delete sceneTwoEntities.at(i);
            }
            sceneTwoEntities.clear();

            for (unsigned int i = 0; i < sceneTwo.size(); i++) {
                sceneTwoEntities.push_back(getEntity(sceneTwo.at(i)));
            }
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
        delete sceneOneEntities.at(i);
    }
    sceneOneEntities.clear();
    for (unsigned int i = 0; i < sceneTwoEntities.size(); i++) {
        delete sceneTwoEntities.at(i);
    }
    sceneTwoEntities.clear();
}
