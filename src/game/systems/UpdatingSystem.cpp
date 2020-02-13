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

        if (camera) {

            systems->cameraSystem.update(*camera);
        }
        if (camera && animatedModel) {

            if (PlayerCameraHandler* playerCameraHandler = currentScene->getComponent<PlayerCameraHandler>(entity)) {
                systems->playerCameraHandlingSystem.setThirdPersonCameraTargetPosition(*playerCameraHandler, animatedModel->transform, *camera);
            }

            if (PlayerController* controller = currentScene->getComponent<PlayerController>(entity)) {

                if (CollisionMesh* cmesh = currentScene->getComponent<CollisionMesh>(entity)) {

                    systems->playerControllerSystem.update(animatedModel->transform, *controller, *camera, *cmesh);
                }
            }
        }
    }
}
