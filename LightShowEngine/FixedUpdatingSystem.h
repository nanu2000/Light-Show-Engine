#ifndef FIXED_UPDATING_SYSTEM
#define FIXED_UPDATING_SYSTEM

#include "LitShader.h"
#include "Scene.h"
#include "PointLightShadowMap.h"
#include "DirectionalLightShadowMap.h"
#include "FountainParticleEmitter.h"
#include "DebuggingController.h"
#include "ThirdPersonCameraControllerTest.h"
#include "PlayerController.h"
#include "SystemBase.h"
#include "DisplayStatistics.h"
#include "ThirdPersonCamera.h"
#include "RayCaster.h"
#include "Messenger.h"
#include "GlobalInformation.h"
#include "entityStatsDisplayer.h"
#include "GUIResizingInfo.h"
#include "GuiButton.h"



class FixedUpdatingSystem : public SystemBase
{

public:

	void initialize	(Scene& scene, Settings& settings, PhysicsWorld& world, SubSystems& systems) override;

	void update		(const Time & time, PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap & directionalLightDepthMap);
	
	void handleBackEndMsg(BackEndMessages msg);

private:
	


	//Updates the shadow maps and sets the current shadows light position.
	void updateShadowMaps	(PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap & directionalLightDepthMap, Camera& currentCamera);

	//Updates any collision logic if necessery.
	void updateCollision	(const int32_t& entity, CollisionMesh& collisionMesh);

	//checks to see if an entity is colliding with another entity.
	void findTriggers(const CollisionTag& thisTag);

	//executes any logic regarding collisions.
	void executeTriggers	(const CollisionTag& thisTag, const CollisionTag& otherTag);


};

#endif