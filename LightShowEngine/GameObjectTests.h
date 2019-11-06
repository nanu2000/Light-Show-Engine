#ifndef GAME_OBJECT_TESTS
#define GAME_OBJECT_TESTS
#include "BoneCollisionMesh.h"
#include "CollisionMesh.h"
#include "DebuggingController.h"
#include "DisplayStatistics.h"
#include "EnemyController.h"
#include "EntityStats.h"
#include "EntityTransform.h"
#include "FountainParticleEmitter.h"
#include "GlobalInformation.h"
#include "GuiSprite.h"
#include "GuiString.h"
#include "Lights.h"
#include "LitShader.h"
#include "Material.h"
#include "PauseMenu.h"
#include "PlayerCameraHandler.h"
#include "PlayerController.h"
#include "Scene.h"
#include "Settings.h"
#include "SkyBox.h"
#include "TestEnemyAI.h"
#include "TextMap.h"
#include "ThirdPersonCamera.h"
#include "ThirdPersonCameraControllerTest.h"
#include "UserControls.h"
#include "entityStatsDisplayer.h"

class EntityWrapper {
public:
    struct EntityVitals {
        Settings* currentSettings    = nullptr;
        Scene* scene                 = nullptr;
        PhysicsWorld* thisWorld      = nullptr;
        WorldSettings* worldSettings = nullptr;
        TextMap* map                 = nullptr;

        bool checkForNulls() { return currentSettings && scene && thisWorld && worldSettings && map; }
    };

    virtual void initialize(EntityVitals& vitals) {}

    virtual ~EntityWrapper() {}
};

class LightTest : public EntityWrapper {

public:
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    std::vector<PointLight> lights = std::vector<PointLight>(4);
    DirectionalLight directionalLight;
    SkyBox skyBox;
    DefaultShader skyBoxShader;
};

class ParticleTest : public EntityWrapper {

public:
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    Particles particles = Particles(100);
    DefaultShader particleShader;
    FountainParticleEmitter particleTest;
};

class Player : public EntityWrapper {

public:
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    DisplayStatistics statDisplayer;
    DefaultShader textShader;

    ThirdPersonCamera camera;
    DebuggingController dbgController;
    UserControls userControls;
    ThirdPersonCameraControllerTest cameraController;
};

class PlayerTestObject : public EntityWrapper {

public:
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    _3DM::AnimatedModel model = _3DM::AnimatedModel("Assets/Models/Player/player.3DMA");

    DefaultShader textShader;

    PauseMenu menu;

    EntityStatsDisplayer statsDisplayer;
    EntityStats stats;

    PlayerCameraHandler cameraHandler;
    CollisionMesh collisionMesh;
    PlayerController testController = PlayerController(glm::vec3(0, -2, 0));
    SimpleMaterial material;
    LitShader shader;
    BoneCollisionMesh bCollisionMesh;
};

class EnemyTestObject : public EntityWrapper {

public:
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    _3DM::AnimatedModel model = _3DM::AnimatedModel("Assets/Models/Player/player.3DMA");

    EntityTransform transform;
    TestEnemyAI ai;
    GlobalInformation info;
    EnemyController controller = EnemyController(glm::vec3(0, -2, 0));
    CollisionMesh collisionMesh;
    SimpleMaterial material;
    LitShader shader;
};

class CubeTrigger : public EntityWrapper {

public:
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    _3DM::Model model = _3DM::Model("Assets/models/cube.3DM");
    CollisionMesh collisionMesh;
    Material material;
    LitShader shader;
};

class FloorObject : public EntityWrapper {
public:
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    _3DM::Model model = _3DM::Model("Assets/Models/testlevel.3DM");
    CollisionMesh collisionMesh;
    Material floorMaterial;
    LitShader shader;
};

#endif
