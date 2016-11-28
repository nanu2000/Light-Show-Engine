#ifndef SUB_SYSTEMS_H
#define SUB_SYSTEMS_H

#include "PauseMenuSystem.h"
#include "EntityStatsDisplayerSystem.h"
#include "DisplayStatisticsSystem.h"
#include "ThirdPersonCameraSystem.h"
#include "FirstPersonCameraSystem.h"
#include "PlayerCameraHandlingSystem.h"
#include "BoneCollisionMeshSystem.h"
#include "EnemyControllerSystem.h"
#include "PlayerControllerSystem.h"

struct SubSystems
{
	PauseMenuSystem				pauseMenuSystem;
	EntityStatsDisplayerSystem	entityStatsDisplayerSystem;
	DisplayStatisticsSystem		displayStatisticsSystem;

	GUIResizingInformation guiResizingInfo;

	PlayerControllerSystem playerControllerSystem;
	EnemyControllerSystem  enemyControllerSystem;

	PlayerCameraHandlingSystem playerCameraHandlingSystem;

	FirstPersonCameraSystem firstPersonCameraSystem;
	ThirdPersonCameraSystem thirdPersonCameraSystem;
	BoneCollisionMeshSystem boneCollisionMeshSystem;

};


#endif