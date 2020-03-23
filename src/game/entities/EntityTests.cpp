#include "EntityTests.h"

void ParticleTest::initialize(EntityVitals& vitals) {
    if (vitals.checkForNulls() == false) {
        DBG_LOG("Null Vitals! (EntityVitals::checkForNulls)\n");
        return;
    }

    int32_t id = vitals.scene->generateEntity();

    particles.setTexture(TextureLocator::getService().getTexture("assets/particles/fluff.png"));

    particles.setPosition(glm::vec3(42, 8, -30));
    particles.setParticlesPerSecond(1000);
    particles.setDefaultStartScale(.1);
    particles.setWeight(1);
    particles.setDefaultLifeTime(2);
    particles.setDefaultColor(glm::vec4(.4, .6, 1, .4));

    particles.setParticleType(PARTICLE_TYPE::Fountain);

    particles.initialize(*vitals.currentSettings);

    particleShader
        = ShaderLocator::getService().getShader("particle", *vitals.currentSettings, "assets/shaders/particle-shader.vert", "assets/shaders/particle-shader.frag", SHADER_TYPE::Particle);

    vitals.scene->addComponent(id, particles);
    vitals.scene->addComponent(id, particleShader);
}

void Player::initialize(EntityVitals& vitals) {
    if (vitals.checkForNulls() == false) {
        DBG_LOG("Null Vitals! (EntityVitals::checkForNulls)\n");
        return;
    }

    int32_t id = vitals.scene->generateEntity();

    textShader = ShaderLocator::getService().getShader("ui", *vitals.currentSettings, "assets/shaders/gui-shader.vert", "assets/shaders/gui-shader.frag", SHADER_TYPE::GUI);

    statDisplayer.initialize(*vitals.map, TextureLocator::getService().getTexture("assets/fonts/courier-new.png", GL_NEAREST));

    menu.initialize(*vitals.map, TextureLocator::getService().getTexture("assets/fonts/courier-new.png", GL_NEAREST));

    //Matrix for shadow map debugging quad
    glm::mat4 dirShadowDbgrm = glm::mat4(1.0) * glm::translate(glm::vec3(0.8f, 0.8f, 0.0f)) * glm::scale(glm::vec3(0.2, 0.2, 0.2));

    dirShadowDbgr.initialize(dirShadowDbgrm);

    vitals.scene->addComponent(id, dirShadowDbgr);
    vitals.scene->addComponent(id, menu);
    vitals.scene->addComponent(id, camera);
    vitals.scene->addComponent(id, textShader);
    vitals.scene->addComponent(id, userControls);
    vitals.scene->addComponent(id, statDisplayer);
}

void PlayerTestObject::initialize(EntityVitals& vitals) {
    if (vitals.checkForNulls() == false) {
        DBG_LOG("Null Vitals! (EntityVitals::checkForNulls)\n");
        return;
    }

    int32_t id = vitals.scene->generateEntity();

    shader = ShaderLocator::getService().getShader("playerobj",
                                                   *vitals.currentSettings,
                                                   "assets/shaders/animated-lit.vert",
                                                   "assets/shaders/phong-diffuse-specular.frag",
                                                   SHADER_TYPE::Lit);

    material.shininess = 16.f;

    model.setAnimationClip(0);

    collisionMesh.initialize(
        btTransform(btQuaternion(btVector3(1, 0, 0), glm::radians(0.0f)), btVector3(0, 20, 0)),
        btCapsuleShape(.6f, 1.5f),
        1.f,
        CollisionTag(),
        false,
        btVector3(0, 0, 0),
        0.0f,
        0.1f);

    textShader = ShaderLocator::getService().getShader("ui", *vitals.currentSettings, "assets/shaders/gui-shader.vert", "assets/shaders/gui-shader.frag", SHADER_TYPE::GUI);

    bCollisionMesh.addCollider(model.getBoneID("head"), id, COLLISION_TAGS::Player);
    bCollisionMesh.addCollider(model.getBoneID("body"), id, COLLISION_TAGS::Player);
    bCollisionMesh.addCollider(model.getBoneID("arm.r"), id, COLLISION_TAGS::Player);
    bCollisionMesh.addCollider(model.getBoneID("arm.l"), id, COLLISION_TAGS::Player);
    bCollisionMesh.addCollider(model.getBoneID("leg.r"), id, COLLISION_TAGS::Player);
    bCollisionMesh.addCollider(model.getBoneID("leg.l"), id, COLLISION_TAGS::Player);

    collisionMesh.getRigidBody()->setInvInertiaDiagLocal(btVector3(0, 0, 0));
    collisionMesh.getRigidBody()->updateInertiaTensor();

    for (unsigned int i = 0; i < model.amountOfMeshes(); i++) {
        model.setMeshMatrix(i, glm::mat4(1.f));
    }

    vitals.scene->addComponent(id, bCollisionMesh);
    vitals.scene->addComponent(id, cameraHandler);
    vitals.scene->addComponent(id, model);
    vitals.scene->addComponent(id, shader);
    vitals.scene->addComponent(id, material);
    vitals.scene->addComponent(id, collisionMesh);

    vitals.scene->addComponent(id, testController);
}

void EnemyTestObject::initialize(EntityVitals& vitals) {
    if (vitals.checkForNulls() == false) {
        DBG_LOG("Null Vitals! (EntityVitals::checkForNulls)\n");
        return;
    }

    int32_t id = vitals.scene->generateEntity();

    shader = ShaderLocator::getService().getShader("enemyobj",
                                                   *vitals.currentSettings,
                                                   "assets/shaders/animated-lit.vert",
                                                   "assets/shaders/phong-diffuse-specular.frag",
                                                   SHADER_TYPE::Lit);

    for (unsigned int i = 0; i < model.amountOfMeshes(); i++) {
        model.addTexture(
            TextureLocator::getService().getTexture("assets/models/player/gltftestimg.png"),
            i,
            _3DM::TextureType::Diffuse);
    }
    material.shininess = 16.f;

    model.setAnimationClip(0);

    collisionMesh.initialize(
        btTransform(btQuaternion(btVector3(1, 0, 0), glm::radians(0.0f)), btVector3(5, 10, -5)),
        btBoxShape(btVector3(.4f, 1.f, .4f)),
        1.f,
        CollisionTag(),
        false,
        btVector3(0, 0, 0),
        0.0f,
        0.f);

    collisionMesh.getRigidBody()->setInvInertiaDiagLocal(btVector3(0, 0, 0));
    collisionMesh.getRigidBody()->updateInertiaTensor();

    for (unsigned int i = 0; i < model.amountOfMeshes(); i++) {
        model.setMeshMatrix(i, glm::mat4(1.f));
    }

    vitals.scene->addComponent(id, transform);
    vitals.scene->addComponent(id, info);
    vitals.scene->addComponent(id, controller);
    vitals.scene->addComponent(id, model);
    vitals.scene->addComponent(id, shader);
    vitals.scene->addComponent(id, material);
    vitals.scene->addComponent(id, collisionMesh);
}

void CubeTrigger::initialize(EntityVitals& vitals) {
    if (vitals.checkForNulls() == false) {
        DBG_LOG("Null Vitals! (EntityVitals::checkForNulls)\n");
        return;
    }

    int32_t id = vitals.scene->generateEntity();

    shader = ShaderLocator::getService().getShader("cubeobj",
                                                   *vitals.currentSettings,
                                                   "assets/shaders/lit.vert",
                                                   "assets/shaders/phong-color.frag",
                                                   SHADER_TYPE::Lit);

    collisionMesh.initialize(
        btTransform(btQuaternion(btVector3(0, 0, 1), 0), btVector3(2, 4, -2)),
        btBoxShape(btVector3(1, 1, 1)),
        0.f,
        CollisionTag(COLLISION_TAGS::TestCollisionTag, id),
        true,
        btVector3(0, 0, 0),
        .1f);

    vitals.scene->addComponent(id, material);

    //model.addTexture will not work on this model because there are no uv coords.
    vitals.scene->addComponent(id, model);
    vitals.scene->addComponent(id, shader);
    vitals.scene->addComponent(id, collisionMesh);
}

void FloorObject::initialize(EntityVitals& vitals) {
    if (vitals.checkForNulls() == false) {
        DBG_LOG("Null Vitals! (EntityVitals::checkForNulls)\n");
        return;
    }

    int32_t id = vitals.scene->generateEntity();

    shader = ShaderLocator::getService().getShader("floorobj",
                                                   *vitals.currentSettings,
                                                   "assets/shaders/lit.vert",
                                                   "assets/shaders/floor-shader.frag",
                                                   SHADER_TYPE::Lit);

    model.addTexture(
        TextureLocator::getService().getTexture("assets/models/scenes/test/FloorTexture.png"),
        0,
        _3DM::TextureType::Diffuse);

    model.addTexture(
        TextureLocator::getService().getTexture("assets/models/scenes/test/StairsTexture.png"),
        1,
        _3DM::TextureType::Diffuse);

    floorMaterial.shininess = 128.f;

    collisionMesh.initializeModelShape(
        btTransform(btQuaternion(btVector3(1, 0, 0), glm::radians(0.0f)), btVector3(0, 0, 0)),
        cm,
        0,
        CollisionTag(),
        false,
        btVector3(0, 0, 0),
        .4f,
        1);

    collisionMesh.getShape()->setLocalScaling(btVector3(1, 1, 1));

    vitals.scene->addComponent(id, floorMaterial);
    vitals.scene->addComponent(id, model);
    vitals.scene->addComponent(id, shader);
    vitals.scene->addComponent(id, collisionMesh);
}

void LightTest::initialize(EntityVitals& vitals) {
    if (vitals.checkForNulls() == false) {
        DBG_LOG("Null Vitals! (EntityVitals::checkForNulls)\n");
        return;
    }

    int32_t id = vitals.scene->generateEntity();

    directionalLight.setActive(true);
    directionalLight.diffuse   = glm::vec3(.8f, .83f, .86f); //diffuse set in daynightcycle
    directionalLight.specular  = glm::vec3(1.f, 1.f, 1.f);
    directionalLight.direction = glm::vec3(0.f, -1.f, 0.f);
    directionalLight.ambient   = glm::vec3(0.1f, 0.1f, 0.1f);

    const int lightRange     = 10;
    const int lightRangeMinY = 7;
    const int lightRangeMaxY = 10;

    for (unsigned int i = 0; i < vitals.currentSettings->getLightsPerEntity() && i < lights.size(); i++) {
        lights[i].setActive(true);

        lights[i].linear    = .45f;
        lights[i].constant  = 1.f;
        lights[i].quadratic = .0075f;

        lights[i].ambient  = glm::vec3(.45f, 0.45f, 0.5f);
        lights[i].diffuse  = glm::vec3(.55f, 0.55f, 0.6f);
        lights[i].specular = glm::vec3(.7, .6f, .5f);

        lights[i].position = glm::vec3(hh::getRandomRange(-lightRange, lightRange), hh::getRandomRange(lightRangeMinY, lightRangeMaxY), 25 + hh::getRandomRange(-lightRange, lightRange));

        vitals.scene->addComponent(id, lights[i]);
    }

    CubeMap& m = TextureLocator::getService().getCubeMap("skybox", std::vector<std::string> { "assets/images/cubemaps/skybox/right.png", "assets/images/cubemaps/skybox/left.png", "assets/images/cubemaps/skybox/top.png", "assets/images/cubemaps/skybox/bottom.png", "assets/images/cubemaps/skybox/front.png", "assets/images/cubemaps/skybox/back.png" });

    skyBox.supplyMap(m);

    skyBoxShader = ShaderLocator::getService().getShader("skybox", *vitals.currentSettings, "assets/shaders/skybox.vert", "assets/shaders/skybox.frag", SHADER_TYPE::Default);

    vitals.scene->addComponent(id, skyBoxShader);
    vitals.scene->addComponent(id, skyBox);
    vitals.scene->addComponent(id, directionalLight);
}
