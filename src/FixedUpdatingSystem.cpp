#include "FixedUpdatingSystem.h"

//Called in Game.cpp
void FixedUpdatingSystem::initialize(Scene& scene, Settings& settings, PhysicsWorld& world, SubSystems& ssystems) {
    currentScene    = &scene;
    currentSettings = &settings;
    physicsWorld    = &world;
    systems         = &ssystems;

    if (Camera* camera = currentScene->getFirstActiveComponentOfType<Camera>()) {
        systems->cameraSystem.initializeCamera(*camera);
    }

    systems->guiResizingInfo.updateInformation();
    systems->dayNightCycleSystem.initialize();
}

//Called in Game.cpp
void FixedUpdatingSystem::handleBackEndMessage(BackEndMessages msg, RenderTexture& renderTexture) {

    switch (msg) {

    case BackEndMessages::REFRESH_CAMERA:

        if (Camera* camera = currentScene->getFirstActiveComponentOfType<Camera>()) {
            systems->cameraSystem.refreshCamera(*camera);
            DBG_LOG("Camera Refreshed Succesfully\n");
        }

        systems->guiResizingInfo.updateInformation();
        renderTexture.resize(GameInfo::getWindowWidth(), GameInfo::getWindowHeight());

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

    bool isPauseMenuShowing = updateGUI(time, pointLightDepthMap, directionalLightDepthMap);

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

    currentScene->loopEntities([&](const Scene::Entity& entity) {
        if (!entity.isActive) {
            return false;
        }

        if (Camera* camera = currentScene->getComponent<Camera>(entity.id)) {

            if (camera->isActive()) {
                //only needs to update fixed- saves memory & barely makes a difference.
                updateShadowMaps(pointLightDepthMap, directionalLightDepthMap, *camera);
            }
        }

        if (Particles* particles = currentScene->getComponent<Particles>(entity.id)) {
            switch (particles->getParticleType()) {
            case PARTICLE_TYPE::Default:
                systems->defaultParticleSystem.fixedUpdateParticles(*particles);
            default:
                break;
            case PARTICLE_TYPE::Fountain:
                systems->fountainParticleSystem.fixedUpdateParticles(*particles);
                break;
            }
        }

        if (DebuggingController* dbgCtrlr = currentScene->getComponent<DebuggingController>(entity.id)) {
            Input& thisInput = InputLocator::getService();

            dbgCtrlr->controlWireframeDebugDraw(thisInput);
            dbgCtrlr->controlPhysicsDebugDraw(thisInput, *physicsWorld);
        }

        if (_3DM::AnimatedModel* animatedModel = currentScene->getComponent<_3DM::AnimatedModel>(entity.id)) {
            animatedModel->fixedUpdateAnimation();
        }

        return false;
    });
}

//Updates GUI.
//returns true if the pause menu is showing.
bool FixedUpdatingSystem::updateGUI(const Time& time, PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap) {

    bool isPauseMenuShowing = false;

    if (UserControls* userControls = currentScene->getFirstActiveComponentOfType<UserControls>()) {
        if (PauseMenu* menu = currentScene->getFirstActiveComponentOfType<PauseMenu>()) {

            isPauseMenuShowing = menu->showing();
        }
    }

    currentScene->loopEntities([&](const Scene::Entity& entity) {
        if (!entity.isActive) {
            return false;
        }

        if (DisplayStatistics* stats = currentScene->getComponent<DisplayStatistics>(entity.id)) {
            systems->displayStatisticsSystem.fixedUpdate(*stats, time, systems->guiResizingInfo);
        }

        return false;
    });

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
                directionalLightDepthMap.updateDepthMap(currentCamera);
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
void FixedUpdatingSystem::updateCollision(const int32_t entity, CollisionMesh& collisionMesh) {

    _3DM::AnimatedModel* animatedModel = currentScene->getComponent<_3DM::AnimatedModel>(entity);
    UserControls* userControls         = currentScene->getFirstActiveComponentOfType<UserControls>();

    if (_3DM::Model* model = currentScene->getComponent<_3DM::Model>(entity)) {
        model->transform = collisionMesh.getTransformation();
    }

    if (animatedModel) {

        PlayerController* controller = currentScene->getComponent<PlayerController>(entity);
        //run player controller and update models animation
        if (controller && userControls) {

            if (Camera* camera = currentScene->getFirstActiveComponentOfType<Camera>()) {

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
            EntityTransform* et           = currentScene->getComponent<EntityTransform>(entity);

            if (globalInfo && et) {

                systems->enemyControllerSystem.update(InputLocator::getService(), *et, collisionMesh, *physicsWorld, *globalInfo, *eController);

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
