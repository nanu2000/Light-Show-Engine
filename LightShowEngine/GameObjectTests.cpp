#include "GameObjectTests.h"

void ParticleTest::initialize(EntityWrapper::EntityVitals& vitals)
{
	if (vitals.checkForNulls() == false)
	{
		DBG_LOG("Null Vitals! (EntityWrapper::EntityVitals::checkForNulls)\n");
		return;
	}

	int32_t id = vitals.scene->generateGameObject();

	particles.initializeTexture(TextureLocator::getService().getTexture("Assets/Particles/fluffParticle.png"));

	particleShader = DefaultShader("Assets/Shaders/ParticleShader.v", "Assets/Shaders/ParticleShader.f", SHADER_TYPE::Particle);

	particleTest.initialize	(*vitals.worldSettings);

	particleTest.setWeight(1);
	particleTest.setFountainPull(10);
	particleTest.setFountainSpread(3.f);
	particleTest.setDefaultLifeTime(1.f);
	particleTest.setDefaultEndScale(2.f);
	particleTest.setDefaultStartScale(.2f);
	particleTest.setParticlesPerSecond(100);
	particleTest.setFountainVerticalForce(15);

	particleTest.emmisionPosition =	 glm::vec3(15, 2, -2);
	particleTest.setDefaultColor(glm::vec4(.15f, .15f, .15f, 1.5f));

	vitals.scene->addComponent(id, particles);
	vitals.scene->addComponent(id, particleTest);
	vitals.scene->addComponent(id, particleShader);
}

void Player::initialize(EntityWrapper::EntityVitals& vitals)
{
	if (vitals.checkForNulls() == false)
	{
		DBG_LOG("Null Vitals! (EntityWrapper::EntityVitals::checkForNulls)\n");
		return;
	}

	int32_t id = vitals.scene->generateGameObject();

	textShader = DefaultShader("Assets/Shaders/GUIShader.v", "Assets/Shaders/GUIShader.f", SHADER_TYPE::GUI);


	statDisplayer.initialize(*vitals.map, TextureLocator::getService().getTexture("Assets/Fonts/CourierNew.png", GL_NEAREST));

	vitals.scene->addComponent(id, cameraController);
	vitals.scene->addComponent(id, camera);
	vitals.scene->addComponent(id, dbgController);
	vitals.scene->addComponent(id, textShader);
	vitals.scene->addComponent(id, userControls);
	vitals.scene->addComponent(id, statDisplayer);

}

void PlayerTestObject::initialize(EntityWrapper::EntityVitals& vitals)
{
	if (vitals.checkForNulls() == false)
	{
		DBG_LOG("Null Vitals! (EntityWrapper::EntityVitals::checkForNulls)\n");
		return;
	}

	int32_t id = vitals.scene->generateGameObject();

	shader = LitShader
		(
			*vitals.currentSettings,
			"Assets/Shaders/AnimatedLit.v",
			"Assets/Shaders/PhongDiffuseSpecular.f",
			SHADER_TYPE::Default
			);

	material.shininess = 64.f;

	model.setAnimationClip(0);

	collisionMesh.initialize
		(
			btTransform(btQuaternion(btVector3(1, 0, 0), glm::radians(0.0f)), btVector3(0, 10, 0)),
			btBoxShape(btVector3(.4f, 1.f, .4f)),
			1.f,
			CollisionTag(),
			false,
			btVector3(0, 0, 0),
			0.0f,
			0.f
			);

	textShader = DefaultShader("Assets/Shaders/GUIShader.v", "Assets/Shaders/GUIShader.f", SHADER_TYPE::GUI);



	for (unsigned int i = 0; i < 30; i++)
	{
		bCollisionMesh.addCollider(i, id, COLLISION_TAGS::Player);
	}



	collisionMesh.getRigidBody()->setInvInertiaDiagLocal(btVector3(0, 0, 0));
	collisionMesh.getRigidBody()->updateInertiaTensor();




	for (unsigned int i = 0; i < model.amountOfMeshes(); i++)
	{
		model.setMeshMatrix(i, glm::mat4());
	}


	menu.initialize(*vitals.map, TextureLocator::getService().getTexture("Assets/Fonts/CourierNew.png", GL_NEAREST));
	statsDisplayer.initialize(*vitals.map, TextureLocator::getService().getTexture("Assets/Fonts/CourierNew.png", GL_NEAREST));

	vitals.scene->addComponent(id, menu);
	

	vitals.scene->addComponent(id, textShader);
	vitals.scene->addComponent(id, statsDisplayer);
	vitals.scene->addComponent(id, stats);
	vitals.scene->addComponent(id, bCollisionMesh);
	vitals.scene->addComponent(id, cameraHandler);
	vitals.scene->addComponent(id, model);
	vitals.scene->addComponent(id, shader);
	vitals.scene->addComponent(id, material);
	vitals.scene->addComponent(id, collisionMesh);

	vitals.scene->addComponent(id, testController);

}

void EnemyTestObject::initialize(EntityWrapper::EntityVitals& vitals)
{
	if (vitals.checkForNulls() == false)
	{
		DBG_LOG("Null Vitals! (EntityWrapper::EntityVitals::checkForNulls)\n");
		return;
	}

	int32_t id = vitals.scene->generateGameObject();

	shader = LitShader
	(
		*vitals.currentSettings,
		"Assets/Shaders/AnimatedLit.v",
		"Assets/Shaders/PhongDiffuseSpecular.f",
		SHADER_TYPE::Default
	);

	material.shininess = 16.f;

	model.setAnimationClip(0);

	collisionMesh.initialize
	(
		btTransform(btQuaternion(btVector3(1, 0, 0), glm::radians(0.0f)), btVector3(5, 10, -5)),
		btBoxShape(btVector3(.4f, 1.f, .4f)),
		1.f,
		CollisionTag(),
		false,
		btVector3(0, 0, 0),
		0.0f,
		0.f
	);
	
	collisionMesh.getRigidBody()->setInvInertiaDiagLocal(btVector3(0, 0, 0));
	collisionMesh.getRigidBody()->updateInertiaTensor();
	
	for (unsigned int i = 0; i < model.amountOfMeshes(); i++)
	{
		model.setMeshMatrix(i, glm::mat4());
	}

	vitals.scene->addComponent(id, transform);
	vitals.scene->addComponent(id, ai);
	vitals.scene->addComponent(id, info);
	vitals.scene->addComponent(id, controller);
	vitals.scene->addComponent(id, model);
	vitals.scene->addComponent(id, shader);
	vitals.scene->addComponent(id, material);
	vitals.scene->addComponent(id, collisionMesh);


}

void CubeTrigger::initialize(EntityWrapper::EntityVitals& vitals)
{
	if (vitals.checkForNulls() == false)
	{
		DBG_LOG("Null Vitals! (EntityWrapper::EntityVitals::checkForNulls)\n");
		return;
	}

	int32_t id = vitals.scene->generateGameObject();

	shader = LitShader
	(
		*vitals.currentSettings,
		"Assets/Shaders/lit.v",
		"Assets/Shaders/PhongColor.f",
		SHADER_TYPE::TransparentModel
	);
						
	collisionMesh.initialize
	(
		btTransform(btQuaternion(btVector3(0, 0, 1), 0), btVector3(2, 4, -2)),
		btBoxShape(btVector3(1, 1, 1)),
		0.f,
		CollisionTag(COLLISION_TAGS::TestCollisionTag, id),
		true,
		btVector3(0, 0, 0),
		.1f
	);
	
	vitals.scene->addComponent(id, material);
	vitals.scene->addComponent(id, model);
	vitals.scene->addComponent(id, shader);
	vitals.scene->addComponent(id, collisionMesh);

}

void FloorObject::initialize(EntityWrapper::EntityVitals& vitals)
{
	if (vitals.checkForNulls() == false)
	{
		DBG_LOG("Null Vitals! (EntityWrapper::EntityVitals::checkForNulls)\n");
		return;
	}

	int32_t id = vitals.scene->generateGameObject();

	shader = LitShader
	(
		*vitals.currentSettings,
		"Assets/Shaders/lit.v",
		"Assets/Shaders/FloorShader.f",
		SHADER_TYPE::Default
	);

	floorMaterial.shininess = 128.f;

	collisionMesh.initializeModelShape
	(
		btTransform(btQuaternion(btVector3(1,0,0),glm::radians(0.0f)), btVector3(0, 0, 0)),
		model,
		0,
		CollisionTag(),
		false,
		btVector3(0,0,0),
		.4f,
		1
	);		

	collisionMesh.getShape()->setLocalScaling(btVector3(1,1,1));
	model.setMeshMatrix(0, glm::mat4());
	
	vitals.scene->addComponent(id, floorMaterial);
	vitals.scene->addComponent(id, model);
	vitals.scene->addComponent(id, shader);
	vitals.scene->addComponent(id, collisionMesh);

}

void LightTest::initialize(EntityWrapper::EntityVitals& vitals)
{
	if (vitals.checkForNulls() == false)
	{
		DBG_LOG("Null Vitals! (EntityWrapper::EntityVitals::checkForNulls)\n");
		return;
	}

	int32_t id = vitals.scene->generateGameObject();


	directionalLight.setActive(true);
	directionalLight.diffuse	= glm::vec3(.8f,	.83f,	.86f);	//diffuse set in daynightcycle
	directionalLight.specular	= glm::vec3(1.f,	1.f,	1.f);
	directionalLight.direction	= glm::vec3(0.f,	-1.f,	0.f);
	directionalLight.ambient	= glm::vec3(0.1f,	0.1f,	0.1f);

	const int lightRange = 10;
	const int lightRangeMinY = 5;
	const int lightRangeMaxY = 10;

	for (unsigned int i = 0; i < vitals.currentSettings->getLightsPerEntity() && i < lights.size(); i++)
	{
		lights[i].setActive (true);

		lights[i].linear = .45f;
		lights[i].constant = 1.f;
		lights[i].quadratic = .0075f;


		lights[i].ambient = glm::vec3(.45f, 0.45f, 0.5f);
		lights[i].diffuse = glm::vec3(.55f, 0.55f, 0.6f);
		lights[i].specular = glm::vec3(.7, .6f, .5f);


		lights[i].position = glm::vec3(hh::getRandomRange(-lightRange, lightRange), hh::getRandomRange(lightRangeMinY, lightRangeMaxY), hh::getRandomRange(-lightRange, lightRange));

		vitals.scene->addComponent(id, lights[i]);
	}

	GLuint m = TextureLocator::getService().createCubeMap(std::vector<std::string>{
		"Assets/Images/cubeMaps/skybox/right.png",
			"Assets/Images/cubeMaps/skybox/left.png",
			"Assets/Images/cubeMaps/skybox/top.png",
			"Assets/Images/cubeMaps/skybox/bottom.png",
			"Assets/Images/cubeMaps/skybox/front.png",
			"Assets/Images/cubeMaps/skybox/back.png"
	});

	skyBox.supplyMap(m);

	skyBoxShader = DefaultShader("Assets/Shaders/SkyBox.v", "Assets/Shaders/SkyBox.f", SHADER_TYPE::Default);


	vitals.scene->addComponent(id, skyBoxShader);
	vitals.scene->addComponent(id, skyBox);
	vitals.scene->addComponent(id, directionalLight);
}