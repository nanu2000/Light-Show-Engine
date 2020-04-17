#include "UpdatingSystem.h"

void UpdatingSystem::initialize(Scene& scene, Engine::SystemVitals& sv, SubSystems& ssystems) {

    currentScene = &scene;
    systemVitals = &sv;
    systems      = &ssystems;
}

void UpdatingSystem::recieveSystemMessage(const BackEndMessages& msg, Engine::SystemVitals& systemVitals) {

    if (!systems) {
        return;
    }

    std::vector<SystemBase*> s = systems->getAllSubSystems();
    for (unsigned int i = 0; i < s.size(); i++) {
        s.at(i)->recieveMessage(msg, *currentScene, systemVitals);
    }

    //!TODO: Move to some sort of system.
    switch (msg) {
    case BackEndMessages::REFRESH_CAMERA:
        systemVitals.getRenderTexture().resize(GameInfo::getWindowWidth(), GameInfo::getWindowHeight());
        break;
    default:
        break;
    }
}

void UpdatingSystem::update(Engine::SystemVitals& sv) {

    bool isPauseMenuShowing = false;

    PhysicsWorld& physicsWorld = sv.getPhysicsWorld();

    sv.getPhysicsWorld().getWorld()->stepSimulation(GameInfo::getDeltaTime(), 1, GameInfo::fixedDeltaTime);

    if (UserControls* userControls = currentScene->getFirstActiveComponentOfType<UserControls>()) {
        if (PauseMenu* menu = currentScene->getFirstActiveComponentOfType<PauseMenu>()) {

            systems->pauseMenuSystem.update(InputLocator::getService(), *menu, systems->guiResizingInfo, *userControls);
            isPauseMenuShowing = menu->showing();
        }
    }

    if (isPauseMenuShowing) {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        SDL_ShowCursor(SDL_ENABLE);
        return; //stop updating.

    } else {

        SDL_ShowCursor(SDL_DISABLE);
        SDL_SetRelativeMouseMode(SDL_TRUE);
        physicsWorld.update();
    }

    const std::vector<Scene::Entity>& entities = *currentScene->getAllEntities();

    for (unsigned int i = 0; i < entities.size(); i++) {
        if (!entities.at(i).isActive) {
            continue;
        }

        int32_t entity = entities.at(i).id;

        Camera* camera = currentScene->getFirstActiveComponentOfType<Camera>();

        _3DM::AnimatedModel* animatedModel = currentScene->getComponent<_3DM::AnimatedModel>(entity);

        if (Particles* particles = currentScene->getComponent<Particles>(entity)) {

            if (InputLocator::getService().isKeyPressedOnce(SDLK_0)) {
                particles->setParticleType(static_cast<PARTICLE_TYPE>((static_cast<int>(particles->getParticleType()) + 1) % static_cast<int>(PARTICLE_TYPE::Max)));

                DBG_LOG("Type set to %i\n", static_cast<int>(particles->getParticleType()));
            }
            switch (particles->getParticleType()) {
            case PARTICLE_TYPE::Default:
                systems->defaultParticleSystem.updateParticles(*particles);
            default:
                break;
            case PARTICLE_TYPE::Fountain:
                systems->fountainParticleSystem.updateParticles(*particles);
                break;
            }
        }
        if (camera && animatedModel) {

            if (PlayerController* controller = currentScene->getComponent<PlayerController>(entity)) {

                if (CollisionMesh* cmesh = currentScene->getComponent<CollisionMesh>(entity)) {
                    UserControls* userControls = currentScene->getFirstActiveComponentOfType<UserControls>();

                    systems->playerControllerSystem.update(animatedModel->transform, *controller, *camera, *cmesh, sv.getPhysicsWorld());
                }
            }

            if (PlayerCameraHandler* playerCameraHandler = currentScene->getComponent<PlayerCameraHandler>(entity)) {
                systems->playerCameraHandlingSystem.setThirdPersonCameraTargetPosition(*playerCameraHandler, animatedModel->transform, *camera);
            }
        }

        if (camera) {

            systems->cameraSystem.update(*camera);
        }
    }
}
