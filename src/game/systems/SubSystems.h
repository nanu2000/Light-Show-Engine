#ifndef SUB_SYSTEMS_H
#define SUB_SYSTEMS_H

#include "BoneCollisionMeshSystem.h"
#include "CameraSystem.h"
#include "DayNightCycleSystem.h"
#include "DebuggingSystem.h"
#include "DirectionalShadowDebuggerSystem.h"
#include "DisplayStatisticsSystem.h"
#include "EnemyControllerSystem.h"
#include "ParticleSystem.h"
#include "PauseMenuSystem.h"
#include "PlayerCameraHandlingSystem.h"
#include "PlayerControllerSystem.h"
#include "SkyBoxSystem.h"

struct SubSystems {

    std::vector<SystemBase*> getAllSubSystems() {
        return systems;
    }

    PauseMenuSystem pauseMenuSystem;
    DisplayStatisticsSystem displayStatisticsSystem;
    GUIResizingInformation guiResizingInfo;
    PlayerControllerSystem playerControllerSystem;
    EnemyControllerSystem enemyControllerSystem;
    PlayerCameraHandlingSystem playerCameraHandlingSystem;
    CameraSystem cameraSystem;
    BoneCollisionMeshSystem boneCollisionMeshSystem;
    DayNightCycleSystem dayNightCycleSystem;
    SkyBoxSystem skyBoxSystem;
    DebuggingSystem debuggingSystem;
    DefaultParticleSystem defaultParticleSystem;
    FountainParticleSystem fountainParticleSystem;
    DirectionalShadowDebuggerSystem directionalShadowDebuggerSystem;

private:
    std::vector<SystemBase*> systems {
        &pauseMenuSystem,
        &displayStatisticsSystem,
        &guiResizingInfo,
        &playerControllerSystem,
        &enemyControllerSystem,
        &playerCameraHandlingSystem,
        &cameraSystem,
        &boneCollisionMeshSystem,
        &dayNightCycleSystem,
        &skyBoxSystem,
        &debuggingSystem,
        &defaultParticleSystem,
        &fountainParticleSystem,
        &directionalShadowDebuggerSystem
    };
};

#endif
