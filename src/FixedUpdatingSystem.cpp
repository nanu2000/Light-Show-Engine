#include "FixedUpdatingSystem.h"

void FixedUpdatingSystem::initialize(Scene& scene, Settings& settings, PhysicsWorld& world, SubSystems& ssystems) {
    currentScene    = &scene;
    currentSettings = &settings;
    physicsWorld    = &world;
    systems         = &ssystems;

    if (FirstPersonCamera* camera = currentScene->getFirstActiveComponentOfType<FirstPersonCamera>()) {
        systems->firstPersonCameraSystem.initializeCamera(*camera);
    }
    if (ThirdPersonCamera* camera = currentScene->getFirstActiveComponentOfType<ThirdPersonCamera>()) {
        systems->thirdPersonCameraSystem.initializeCamera(*camera);
    }

    systems->guiResizingInfo.updateInformation();
    systems->dayNightCycleSystem.initialize();
}

bool FixedUpdatingSystem::updateGUI(const std::vector<Scene::GameObject>& gameObjects, const Time& time, PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap) {

    bool isPauseMenuShowing = false;

    if (UserControls* userControls = currentScene->getFirstActiveComponentOfType<UserControls>()) {
        if (PauseMenu* menu = currentScene->getFirstActiveComponentOfType<PauseMenu>()) {

            systems->pauseMenuSystem.update(InputLocator::getService(), *menu, systems->guiResizingInfo, *userControls);
            isPauseMenuShowing = menu->showing();
        }
    }

    if (isPauseMenuShowing) {

        SDL_ShowCursor(SDL_ENABLE);

    } else {

        //Update Physics
        physicsWorld->update();

        SDL_ShowCursor(SDL_DISABLE);

        GameInfo::setMousePosition(GameInfo::getWindowWidth() / 2, GameInfo::getWindowHeight() / 2);
    }

    for (unsigned int i = 0; i < gameObjects.size(); i++) {
        const int32_t& entity = gameObjects[i].id;

        if (currentScene->isEntityActive(entity)) {
            if (EntityStats* stats = currentScene->getComponent<EntityStats>(entity)) {
                if (EntityStatsDisplayer* disp = currentScene->getComponent<EntityStatsDisplayer>(entity)) {
                    systems->entityStatsDisplayerSystem.update(*disp, *stats, systems->guiResizingInfo);
                }
            }

            if (DisplayStatistics* stats = currentScene->getComponent<DisplayStatistics>(entity)) {
                systems->displayStatisticsSystem.update(*stats, time, systems->guiResizingInfo);
            }
        }
    }

    return isPauseMenuShowing;
}

void FixedUpdatingSystem::fixedUpdate(const Time& time, PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap) {

    if (areVitalsNull()) {
        return;
    }

    const std::vector<Scene::GameObject>& gameObjects = *currentScene->getAllGameObjects();

    bool isPauseMenuShowing = updateGUI(gameObjects, time, pointLightDepthMap, directionalLightDepthMap);

    if (isPauseMenuShowing) {
        return;
    }

    currentScene->performOperationsOnAllOfType<DirectionalLight>(
        [&](DirectionalLight& light) {
            systems->dayNightCycleSystem.update(light, time);

            //light.direction = dir;
            return false;
        });

    currentScene->performOperationsOnAllOfType<CollisionMesh>(
        [&](CollisionMesh& mesh) {
            updateCollision(mesh.getEntityID(), mesh);
            return false;
        });

    currentScene->performOperationsOnAllOfType<BoneCollisionMesh>(
        [&](BoneCollisionMesh& mesh) {
            if (_3DM::AnimatedModel* animatedModel = currentScene->getComponent<_3DM::AnimatedModel>(mesh.getEntityID())) {
                systems->boneCollisionMeshSystem.update(mesh, *animatedModel);
            }

            const std::vector<const CollisionTag*>& collisionTags = *mesh.getCollisionTags();

            for (unsigned int i = 0; i < collisionTags.size(); i++) {
                findTriggers(*collisionTags[i]);
            }

            return false;
        });

    for (unsigned int i = 0; i < gameObjects.size(); i++) {
        const int32_t& entity = gameObjects[i].id;

        if (currentScene->isEntityActive(entity)) {

            if (TestEnemyAI* ai = currentScene->getComponent<TestEnemyAI>(entity)) {
                if (EntityTransform* et = currentScene->getComponent<EntityTransform>(entity)) {

                    if (GlobalInformation* info = currentScene->getComponent<GlobalInformation>(entity)) {
                        ai->update(*info, InputLocator::getService(), *et);
                    }
                }
            }

            if (ThirdPersonCamera* thisCamera = currentScene->getComponent<ThirdPersonCamera>(entity)) {

                if (thisCamera->isActive()) {
                    //only needs to update fixed- saves memory & barely makes a difference.
                    updateShadowMaps(pointLightDepthMap, directionalLightDepthMap, *thisCamera);
                }
            }

            if (Particles* p = currentScene->getComponent<Particles>(entity)) {
                if (ParticleEmitter* pe = currentScene->getComponent<FountainParticleEmitter>(entity)) {
                    pe->updateParticles(*p);
                }
            }

            if (DebuggingController* dbgCtrlr = currentScene->getComponent<DebuggingController>(entity)) {
                Input& thisInput = InputLocator::getService();

                dbgCtrlr->controlWireframeDebugDraw(thisInput);
                dbgCtrlr->controlPhysicsDebugDraw(thisInput, *physicsWorld);
            }

            if (_3DM::AnimatedModel* animatedModel = currentScene->getComponent<_3DM::AnimatedModel>(entity)) {
                animatedModel->updateAnimation();
            }
        }
    }
}

void FixedUpdatingSystem::handleBackEndMsg(BackEndMessages msg) {
    switch (msg) {

    case REFRESH_CAMERA:

        if (FirstPersonCamera* camera = currentScene->getFirstActiveComponentOfType<FirstPersonCamera>()) {
            systems->firstPersonCameraSystem.refreshCamera(*camera);
            DBG_LOG("Camera Refreshed Succesfully\n");
        }
        if (ThirdPersonCamera* camera = currentScene->getFirstActiveComponentOfType<ThirdPersonCamera>()) {
            systems->thirdPersonCameraSystem.refreshCamera(*camera);
            DBG_LOG("Camera Refreshed Succesfully\n");
        }

        systems->guiResizingInfo.updateInformation();

        break;

    case NULL_MESSAGE:
    case MESSAGES_END:
    default:
        DBG_LOG("NULL or Unknown Message Passed as Argument (FixedUpdatingSystem::handleBackEndMsg)\n");
        break;
    }
}

void FixedUpdatingSystem::updateShadowMaps(PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap, Camera& currentCamera) {
    pointLightDepthMap.setShadowActive(false);
    directionalLightDepthMap.setShadowActive(false);

    currentScene->performOperationsOnAllOfType<DirectionalLight>(
        [&](const DirectionalLight& light) {
            if (light.isActive()) {
                directionalLightDepthMap.setCurrentLightDirection(light.direction);
                directionalLightDepthMap.updateDepthMap(currentCamera.position);
                directionalLightDepthMap.setShadowActive(true);
                return true;
            }
            return false;
        });

    currentScene->performOperationsOnAllOfType<PointLight>(
        [&](const PointLight& light) {
            if (light.isActive()) {
                pointLightDepthMap.setCurrentLightPosition(light.position);
                pointLightDepthMap.setShadowActive(true);
                pointLightDepthMap.updateDepthMap();
                return true;
            }
            return false;
        });
}
void FixedUpdatingSystem::updateCollision(const int32_t& entity, CollisionMesh& collisionMesh) {
    _3DM::AnimatedModel* animatedModel   = currentScene->getComponent<_3DM::AnimatedModel>(entity);
    ThirdPersonCamera* thirdPersonCamera = currentScene->getFirstActiveComponentOfType<ThirdPersonCamera>(); //currentScene->getFirstActiveComponentType is a semi costly function. Maybe move it and pass it as a function argument

    if (_3DM::Model* model = currentScene->getComponent<_3DM::Model>(entity)) {
        model->transform = collisionMesh.getTransformation();

        if (UserControls* userControls = currentScene->getFirstActiveComponentOfType<UserControls>()) {
            if (PlayerController* controller = currentScene->getComponent<PlayerController>(entity)) {
                if (thirdPersonCamera) {
                    systems->playerControllerSystem.fixedUpdate(InputLocator::getService(), model->transform, collisionMesh, *physicsWorld, *controller, *thirdPersonCamera, *userControls);
                } else if (FirstPersonCamera* currentCamera = currentScene->getFirstActiveComponentOfType<FirstPersonCamera>()) {
                    systems->playerControllerSystem.fixedUpdate(InputLocator::getService(), model->transform, collisionMesh, *physicsWorld, *controller, *currentCamera, *userControls);
                }

                const std::vector<GlobalInformation*>& GI = currentScene->getAllComponentsOfType<GlobalInformation>();

                for (unsigned int i = 0; i < GI.size(); i++) {
                    GI[i]->setPlayersPosition(model->transform.position);
                }
            }
        }
    }
    if (animatedModel) {

        if (UserControls* userControls = currentScene->getFirstActiveComponentOfType<UserControls>()) {
            if (PlayerController* controller = currentScene->getComponent<PlayerController>(entity)) {
                //currentScene->getFirstActiveComponentType is a semi costly function. Maybe move it and pass it as a function argument
                if (thirdPersonCamera) {
                    systems->playerControllerSystem.fixedUpdate(InputLocator::getService(), animatedModel->transform, collisionMesh, *physicsWorld, *controller, *thirdPersonCamera, *userControls);
                } else if (FirstPersonCamera* currentCamera = currentScene->getFirstActiveComponentOfType<FirstPersonCamera>()) {
                    systems->playerControllerSystem.fixedUpdate(InputLocator::getService(), animatedModel->transform, collisionMesh, *physicsWorld, *controller, *currentCamera, *userControls);
                }

                animatedModel->setAnimationClip(controller->getAnimationStateUint());

                const std::vector<GlobalInformation*>& GI = currentScene->getAllComponentsOfType<GlobalInformation>();

                for (unsigned int i = 0; i < GI.size(); i++) {
                    GI[i]->setPlayersPosition(animatedModel->transform.position);
                }
            } else if (EnemyController* eController = currentScene->getComponent<EnemyController>(entity)) {
                if (GlobalInformation* globalInfo = currentScene->getComponent<GlobalInformation>(entity)) {
                    if (TestEnemyAI* ai = currentScene->getComponent<TestEnemyAI>(entity)) {
                        if (EntityTransform* et = currentScene->getComponent<EntityTransform>(entity)) {

                            systems->enemyControllerSystem.update(InputLocator::getService(), *et, collisionMesh, *physicsWorld, *globalInfo, *eController, *userControls, *ai);

                            animatedModel->transform = et->transform;
                        }
                    }
                }

                animatedModel->setAnimationClip(eController->getAnimationStateUint());
            }
        }
    }

    findTriggers(*collisionMesh.getTag());
}

void FixedUpdatingSystem::executeTriggers(const CollisionTag& thisTag, const CollisionTag& otherTag) {
    if (otherTag.entity != TAG_ENTITY_UNDEFINED && currentScene->isEntityActive(otherTag.entity)) {
        if (otherTag.tagName == COLLISION_TAGS::TestCollisionTag && thisTag.tagName == COLLISION_TAGS::Player) {
            DBG_LOG(
                "The %s entity collided with the %s Entity %i\n",
                GET_COLLISION_TAG_NAME(COLLISION_TAGS::Player),
                GET_COLLISION_TAG_NAME(COLLISION_TAGS::TestCollisionTag),
                SDL_GetTicks());

            currentScene->setEntityActive(otherTag.entity, false);
        }
    }
}

void FixedUpdatingSystem::findTriggers(const CollisionTag& thisTag) {
    if (thisTag.tagName == COLLISION_TAGS::TAG_EMPTY) {
        return;
    }

    const std::vector<CollisionTag*>& others = thisTag.collidingWith;
    const CollisionTag* other                = nullptr;

    for (unsigned int i = 0; i < thisTag.collidingWith.size() && (other = others[i]); i++) {
        executeTriggers(thisTag, *other);
    }
}
