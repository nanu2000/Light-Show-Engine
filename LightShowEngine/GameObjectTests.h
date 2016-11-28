#ifndef GAME_OBJECT_TESTS
#define GAME_OBJECT_TESTS
#include "Scene.h"
#include "Material.h"
#include "CollisionMesh.h"
#include "PlayerController.h"
#include "LitShader.h"
#include "Lights.h"
#include "DebuggingController.h"
#include "ThirdPersonCameraControllerTest.h"
#include "Settings.h"
#include "FountainParticleEmitter.h"
#include "TextMap.h"
#include "GuiString.h"
#include "DisplayStatistics.h"
#include "ThirdPersonCamera.h"
#include "UserControls.h"
#include "PlayerCameraHandler.h"
#include "BoneCollisionMesh.h"
#include "EnemyController.h"
#include "GlobalInformation.h"
#include "TestEnemyAI.h"
#include "EntityTransform.h"
#include "EntityStats.h"
#include "entityStatsDisplayer.h"
#include "GuiSprite.h"
#include "PauseMenu.h"

class EntityWrapper 
{
public:

	struct EntityVitals 
	{
		Settings		* currentSettings	= nullptr;
		Scene			* scene				= nullptr;
		PhysicsWorld	* thisWorld			= nullptr;
		WorldSettings	* worldSettings		= nullptr;
		TextMap			* map				= nullptr;

		bool checkForNulls() { return currentSettings && scene && thisWorld && worldSettings && map; }
	};

	virtual void initialize(EntityVitals& vitals) {}

	virtual ~EntityWrapper() {}
	
};

class LightTest : public EntityWrapper
{

public:

	void initialize(EntityWrapper::EntityVitals& vitals);

private:
	std::vector<PointLight> lights = std::vector<PointLight>(4);
	DirectionalLight	directionalLight;


};

class ParticleTest : public EntityWrapper
{

public:

	void initialize(EntityWrapper::EntityVitals& vitals);

private:

	Particles				particles = Particles(100);
	DefaultShader			particleShader;
	FountainParticleEmitter particleTest;

};

class Player : public EntityWrapper
{

public:

	void initialize(EntityWrapper::EntityVitals& vitals);
	
private:

	DisplayStatistics	statDisplayer;
	DefaultShader		textShader;

	ThirdPersonCamera	camera;
	DebuggingController dbgController;
	UserControls		userControls;
	ThirdPersonCameraControllerTest	cameraController;

};

class PlayerTestObject : public EntityWrapper
{

public:

	void initialize(EntityWrapper::EntityVitals& vitals);
	
private:

	_3DM::AnimatedModel model = _3DM::AnimatedModel("p/player.3DMA");

	DefaultShader		textShader;

	PauseMenu			menu;

	
	EntityStatsDisplayer statsDisplayer;
	EntityStats			stats;

	PlayerCameraHandler cameraHandler;
	CollisionMesh		collisionMesh;
	PlayerController	testController = PlayerController(glm::vec3(0,-2,0));
	SimpleMaterial		material;
	LitShader			shader;
	BoneCollisionMesh	bCollisionMesh;


};

class EnemyTestObject : public EntityWrapper
{

public:

	void initialize(EntityWrapper::EntityVitals& vitals);

private:

	_3DM::AnimatedModel model = _3DM::AnimatedModel("p/player.3DMA");

	EntityTransform		transform;
	TestEnemyAI			ai;
	GlobalInformation	info;
	EnemyController		controller = EnemyController(glm::vec3(0, -2, 0));
	CollisionMesh		collisionMesh;
	SimpleMaterial		material;
	LitShader			shader;


};


class CubeTrigger : public EntityWrapper
{
	
public:

	void initialize(EntityWrapper::EntityVitals& vitals);

private:

	_3DM::Model			model = _3DM::Model("assets/models/cube.3DM");
	CollisionMesh		collisionMesh;
	Material			material;
	LitShader			shader;

};

class FloorObject : public EntityWrapper
{
public:

	void initialize(EntityWrapper::EntityVitals& vitals);

private:

	_3DM::Model model = _3DM::Model("Assets/Models/testlevel.3DM");
	CollisionMesh collisionMesh;
	Material cubeMaterial;
	LitShader shader;

};

#endif