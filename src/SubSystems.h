#ifndef SUB_SYSTEMS_H
#define SUB_SYSTEMS_H

#include "BoneCollisionMeshSystem.h"
#include "CameraSystem.h"
#include "DayNightCycleSystem.h"
#include "DisplayStatisticsSystem.h"
#include "EnemyControllerSystem.h"
#include "ParticleSystem.h"
#include "PauseMenuSystem.h"
#include "PlayerCameraHandlingSystem.h"
#include "PlayerControllerSystem.h"
#include "SkyBoxSystem.h"

struct SubSystems {
    PauseMenuSystem pauseMenuSystem;
    DisplayStatisticsSystem displayStatisticsSystem;

    // GUIResizingInformation guiResizingInfo;

    PlayerControllerSystem playerControllerSystem;
    EnemyControllerSystem enemyControllerSystem;

    PlayerCameraHandlingSystem playerCameraHandlingSystem;

    CameraSystem cameraSystem;
    BoneCollisionMeshSystem boneCollisionMeshSystem;

    //DayNightCycleSystem dayNightCycleSystem;
    SkyBoxSystem skyBoxSystem;

    DefaultParticleSystem defaultParticleSystem;
    FountainParticleSystem fountainParticleSystem;
};

#endif
