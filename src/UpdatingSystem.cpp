#include "UpdatingSystem.h"

void UpdatingSystem::initialize(Scene& scene, Settings& settings, PhysicsWorld& world, SubSystems& ssystems) {

    //Vitals
    currentScene    = &scene;
    systems         = &ssystems;
    physicsWorld    = &world;
    currentSettings = &settings;
}

void UpdatingSystem::update() {

    bool isPauseMenuShowing = false;

    if (UserControls* userControls = currentScene->getFirstActiveComponentOfType<UserControls>()) {
        if (PauseMenu* menu = currentScene->getFirstActiveComponentOfType<PauseMenu>()) {

            systems->pauseMenuSystem.update(InputLocator::getService(), *menu, systems->guiResizingInfo, *userControls);
            isPauseMenuShowing = menu->showing();
        }
    }

    if (isPauseMenuShowing) {
        return;
    }

    const std::vector<Scene::Entity>& entities = *currentScene->getAllEntities();

    for (unsigned int i = 0; i < entities.size(); i++) {
        if (!entities.at(i).isActive) {
            continue;
        }

        int32_t entity = entities.at(i).id;

        ThirdPersonCamera* thirdPersonCamera = currentScene->getFirstActiveComponentOfType<ThirdPersonCamera>();

        _3DM::AnimatedModel* animatedModel = currentScene->getComponent<_3DM::AnimatedModel>(entity);

        if (Particles* p = currentScene->getComponent<Particles>(entity)) {
            if (ParticleEmitter* pe = currentScene->getComponent<FountainParticleEmitter>(entity)) {
                pe->updateParticles(*p);
            }
        }

        if (thirdPersonCamera) {

            systems->thirdPersonCameraSystem.update(*thirdPersonCamera);

            if (ThirdPersonCameraControllerTest* thisCameraController = currentScene->getComponent<ThirdPersonCameraControllerTest>(entity)) {
                thisCameraController->update(thirdPersonCamera);
            }
        }
        if (thirdPersonCamera && animatedModel) {

            if (PlayerCameraHandler* playerCameraHandler = currentScene->getComponent<PlayerCameraHandler>(entity)) {
                systems->playerCameraHandlingSystem.setThirdPersonCameraTargetPosition(*playerCameraHandler, animatedModel->transform, *thirdPersonCamera);
            }

            if (PlayerController* controller = currentScene->getComponent<PlayerController>(entity)) {

                systems->playerControllerSystem.update(animatedModel->transform, *controller, *thirdPersonCamera);
            }
        }
    }
}
