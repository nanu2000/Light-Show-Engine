#include "FixedUpdatingSystem.h"

//Called in Game.cpp
void FixedUpdatingSystem::initialize(Scene& scene, Settings& settings, PhysicsWorld& world, SubSystems& ssystems) {
    currentScene    = &scene;
    currentSettings = &settings;
    physicsWorld    = &world;
    systems         = &ssystems;

    if (ThirdPersonCamera* camera = currentScene->getFirstActiveComponentOfType<ThirdPersonCamera>()) {
        systems->thirdPersonCameraSystem.initializeCamera(*camera);
    }

    systems->guiResizingInfo.updateInformation();
    systems->dayNightCycleSystem.initialize();
}

//Called in Game.cpp
void FixedUpdatingSystem::handleBackEndMessage(BackEndMessages msg) {

    switch (msg) {

    case BackEndMessages::REFRESH_CAMERA:

        if (ThirdPersonCamera* camera = currentScene->getFirstActiveComponentOfType<ThirdPersonCamera>()) {
            systems->thirdPersonCameraSystem.refreshCamera(*camera);
            DBG_LOG("Camera Refreshed Succesfully\n");
        }

        systems->guiResizingInfo.updateInformation();

        break;

    case BackEndMessages::NULL_MESSAGE:
    case BackEndMessages::MESSAGES_END:
    default:
        DBG_LOG("NULL or Unknown Message Passed as Argument (FixedUpdatingSystem::handleBackEndMsg)\n");
        break;
    }
}

//Called in Game.cpp
void FixedUpdatingSystem::fixedUpdate(const Time& time, PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap) {

    if (areVitalsNull()) {
        return;
    }

    const std::vector<Scene::Entity>& entities = *currentScene->getAllEntities();

    bool isPauseMenuShowing = updateGUI(entities, time, pointLightDepthMap, directionalLightDepthMap);

    if (isPauseMenuShowing) {

        SDL_ShowCursor(SDL_ENABLE);
        return; //stop updating.

    } else {

        //Update Physics
        physicsWorld->fixedUpdate();

        SDL_ShowCursor(SDL_DISABLE);

        GameInfo::setMousePosition(GameInfo::getWindowWidth() / 2, GameInfo::getWindowHeight() / 2);
    }

    //Run day night cycle
    currentScene->performOperationsOnAllOfType<DirectionalLight>(
        [&](DirectionalLight& light) {
            systems->dayNightCycleSystem.fixedUpdate(light, time);
            return false;
        });

    //Update collision for meshes
    currentScene->performOperationsOnAllOfType<CollisionMesh>(
        [&](CollisionMesh& mesh) {
            updateCollision(mesh.getEntityID(), mesh);
            //update triggers
            updateCollisionTriggers(*mesh.getTag());
            return false;
        });

    //updateBoneCollisionMeshes
    currentScene->performOperationsOnAllOfType<BoneCollisionMesh>(
        [&](BoneCollisionMesh& mesh) {
            //Update position of colliders
            if (_3DM::AnimatedModel* animatedModel = currentScene->getComponent<_3DM::AnimatedModel>(mesh.getEntityID())) {
                systems->boneCollisionMeshSystem.fixedUpdate(mesh, *animatedModel);
            }

            //Update collision for boneCollisionMeshes
            const std::vector<const CollisionTag*>& collisionTags = *mesh.getCollisionTags();
            for (unsigned int i = 0; i < collisionTags.size(); i++) {
                updateCollisionTriggers(*collisionTags[i]);
            }

            return false;
        });

    for (unsigned int i = 0; i < entities.size(); i++) {
        const int32_t& entity = entities[i].id;

        if (!currentScene->isEntityActive(entity)) {
            continue;
        }

        if (TestEnemyAI* ai = currentScene->getComponent<TestEnemyAI>(entity)) {
            if (EntityTransform* et = currentScene->getComponent<EntityTransform>(entity)) {
                if (GlobalInformation* info = currentScene->getComponent<GlobalInformation>(entity)) {
                    ai->fixedUpdate(*info, InputLocator::getService(), *et);
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
                pe->fixedUpdateParticles(*p);
            }
        }

        if (DebuggingController* dbgCtrlr = currentScene->getComponent<DebuggingController>(entity)) {
            Input& thisInput = InputLocator::getService();

            dbgCtrlr->controlWireframeDebugDraw(thisInput);
            dbgCtrlr->controlPhysicsDebugDraw(thisInput, *physicsWorld);
        }

        if (_3DM::AnimatedModel* animatedModel = currentScene->getComponent<_3DM::AnimatedModel>(entity)) {
            animatedModel->fixedUpdateAnimation();
        }
    }
}

//Updates GUI.
//returns true if the pause menu is showing.
bool FixedUpdatingSystem::updateGUI(const std::vector<Scene::Entity>& entities, const Time& time, PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap) {

    bool isPauseMenuShowing = false;

    if (UserControls* userControls = currentScene->getFirstActiveComponentOfType<UserControls>()) {
        if (PauseMenu* menu = currentScene->getFirstActiveComponentOfType<PauseMenu>()) {

            isPauseMenuShowing = menu->showing();
        }
    }

    for (unsigned int i = 0; i < entities.size(); i++) {
        const int32_t& entity = entities[i].id;

        if (!currentScene->isEntityActive(entity)) {
            continue;
        }

        if (EntityStats* stats = currentScene->getComponent<EntityStats>(entity)) {
            if (EntityStatsDisplayer* disp = currentScene->getComponent<EntityStatsDisplayer>(entity)) {
                systems->EntityStatsDisplayerSystem.fixedUpdate(*disp, *stats, systems->guiResizingInfo);
            }
        }

        if (DisplayStatistics* stats = currentScene->getComponent<DisplayStatistics>(entity)) {
            systems->displayStatisticsSystem.fixedUpdate(*stats, time, systems->guiResizingInfo);
        }
    }

    return isPauseMenuShowing;
}

//Provides info for the shadow maps such as the lights' positions
void FixedUpdatingSystem::updateShadowMaps(PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap, Camera& currentCamera) {

    //Assume no shadows are active
    pointLightDepthMap.setShadowActive(false);
    directionalLightDepthMap.setShadowActive(false);

    currentScene->performOperationsOnAllOfType<DirectionalLight>(
        [&](const DirectionalLight& light) {
            if (light.isActive()) {
                directionalLightDepthMap.setCurrentLightDirection(light.direction);
                directionalLightDepthMap.updateDepthMap(currentCamera.position);
                directionalLightDepthMap.setShadowActive(true);
                return true; //perform on first light
            }
            return false;
        });

    currentScene->performOperationsOnAllOfType<PointLight>(
        [&](const PointLight& light) {
            if (light.isActive()) {
                pointLightDepthMap.setCurrentLightPosition(light.position);
                pointLightDepthMap.setShadowActive(true);
                pointLightDepthMap.updateDepthMap();
                return true; //perform on first light
            }
            return false;
        });
}

//Set transforms of models to collision transforms
void FixedUpdatingSystem::updateCollision(const int32_t& entity, CollisionMesh& collisionMesh) {

    _3DM::AnimatedModel* animatedModel = currentScene->getComponent<_3DM::AnimatedModel>(entity);
    UserControls* userControls         = currentScene->getFirstActiveComponentOfType<UserControls>();

    if (_3DM::Model* model = currentScene->getComponent<_3DM::Model>(entity)) {
        model->transform = collisionMesh.getTransformation();
    }

    if (animatedModel) {

        PlayerController* controller = currentScene->getComponent<PlayerController>(entity);
        //run player controller and update models animation
        if (controller && userControls) {

            if (ThirdPersonCamera* camera = currentScene->getFirstActiveComponentOfType<ThirdPersonCamera>()) {

                systems->playerControllerSystem.fixedUpdate(InputLocator::getService(), animatedModel->transform, collisionMesh, *physicsWorld, *controller, *camera, *userControls);

                animatedModel->setAnimationClip(controller->getAnimationStateUint());

                const std::vector<GlobalInformation*>& GI = currentScene->getAllComponentsOfType<GlobalInformation>();

                //Update all globalinfo instances telling them where the players at.
                //This could be done in a better way but right not it's not a big deal.
                for (unsigned int i = 0; i < GI.size(); i++) {
                    GI[i]->setPlayersPosition(animatedModel->transform.position);
                }
            }

        } else if (EnemyController* eController = currentScene->getComponent<EnemyController>(entity)) {

            //run enemy controller and update models animation

            //3 prereqs for enemyctrlr sys
            GlobalInformation* globalInfo = currentScene->getComponent<GlobalInformation>(entity);
            TestEnemyAI* ai               = currentScene->getComponent<TestEnemyAI>(entity);
            EntityTransform* et           = currentScene->getComponent<EntityTransform>(entity);

            if (globalInfo && ai && et) {

                systems->enemyControllerSystem.update(InputLocator::getService(), *et, collisionMesh, *physicsWorld, *globalInfo, *eController, *ai);

                animatedModel->transform = et->transform;
                animatedModel->setAnimationClip(eController->getAnimationStateUint());
            }
        }
    }
}

//Checks for collision tag, if it exists then it will execute triggers on any objects colliding
void FixedUpdatingSystem::updateCollisionTriggers(const CollisionTag& thisTag) {
    if (thisTag.tagName == COLLISION_TAGS::TAG_EMPTY) {
        return;
    }

    const std::vector<CollisionTag*>& others = thisTag.collidingWith;
    const CollisionTag* other                = nullptr;

    for (unsigned int i = 0; i < thisTag.collidingWith.size() && (other = others[i]); i++) {
        handleCollisionTrigger(thisTag, *other);
    }
}

void FixedUpdatingSystem::handleCollisionTrigger(const CollisionTag& thisTag, const CollisionTag& otherTag) {

    if (otherTag.entity == TAG_ENTITY_UNDEFINED || !currentScene->isEntityActive(otherTag.entity)) {
        return;
    }

    //Just a test
    if (otherTag.tagName == COLLISION_TAGS::TestCollisionTag && thisTag.tagName == COLLISION_TAGS::Player) {
        DBG_LOG(
            "The %s entity collided with the %s Entity %i\n",
            GET_COLLISION_TAG_NAME(COLLISION_TAGS::Player),
            GET_COLLISION_TAG_NAME(COLLISION_TAGS::TestCollisionTag),
            SDL_GetTicks());

        currentScene->setEntityActive(otherTag.entity, false);
    }
}
