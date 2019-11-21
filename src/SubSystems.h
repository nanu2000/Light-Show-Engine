#ifndef SUB_SYSTEMS_H
#define SUB_SYSTEMS_H

#include "BoneCollisionMeshSystem.h"
#include "DayNightCycleSystem.h"
#include "DisplayStatisticsSystem.h"
#include "EnemyControllerSystem.h"
#include "FirstPersonCameraSystem.h"
#include "PauseMenuSystem.h"
#include "PlayerCameraHandlingSystem.h"
#include "PlayerControllerSystem.h"
#include "SkyBoxSystem.h"
#include "ThirdPersonCameraSystem.h"

struct SubSystems {
    PauseMenuSystem pauseMenuSystem;
    DisplayStatisticsSystem displayStatisticsSystem;

    GUIResizingInformation guiResizingInfo;

    PlayerControllerSystem playerControllerSystem;
    EnemyControllerSystem enemyControllerSystem;

    PlayerCameraHandlingSystem playerCameraHandlingSystem;

    FirstPersonCameraSystem firstPersonCameraSystem;
    ThirdPersonCameraSystem thirdPersonCameraSystem;
    BoneCollisionMeshSystem boneCollisionMeshSystem;

    DayNightCycleSystem dayNightCycleSystem;
    SkyBoxSystem skyBoxSystem;
};

#endif
