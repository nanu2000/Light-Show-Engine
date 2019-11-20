#ifndef ENTITY_TESTS
#define ENTITY_TESTS
#include "BoneCollisionMesh.h"
#include "CollisionMesh.h"
#include "DebuggingController.h"
#include "DisplayStatistics.h"
#include "EnemyController.h"
#include "EntityStats.h"
#include "EntityStatsDisplayer.h"
#include "EntityTransform.h"
#include "FountainParticleEmitter.h"
#include "GlobalInformation.h"
#include "GuiSprite.h"
#include "GuiString.h"
#include "Lights.h"
#include "Material.h"
#include "PauseMenu.h"
#include "PlayerCameraHandler.h"
#include "PlayerController.h"
#include "Scene.h"
#include "Settings.h"
#include "Shader.h"
#include "SkyBox.h"
#include "TextMap.h"
#include "ThirdPersonCamera.h"
#include "ThirdPersonCameraControllerTest.h"
#include "UserControls.h"

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

    virtual void initialize(EntityVitals& vitals) = 0;

    virtual ~EntityWrapper() {}
};

class LightTest : public EntityWrapper {

public:
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    std::vector<PointLight> lights = std::vector<PointLight>(4);
    DirectionalLight directionalLight;
    SkyBox skyBox;
    Shader skyBoxShader;
};

class ParticleTest : public EntityWrapper {

public:
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    Particles particles = Particles(10000);
    Shader particleShader;
    FountainParticleEmitter particleTest;
};

class Player : public EntityWrapper {

public:
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    DisplayStatistics statDisplayer;
    Shader textShader;

    ThirdPersonCamera camera;
    DebuggingController dbgController;
    UserControls userControls;
    ThirdPersonCameraControllerTest cameraController;
};

class PlayerTestObject : public EntityWrapper {

public:
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    _3DM::AnimatedModel model = _3DM::AnimatedModel("assets/Models/player/player.3DMA");

    Shader textShader;

    PauseMenu menu;

    EntityStatsDisplayer statsDisplayer;
    EntityStats stats;

    PlayerCameraHandler cameraHandler;
    CollisionMesh collisionMesh;
    PlayerController testController = PlayerController(glm::vec3(0, -2, 0));
    SimpleMaterial material;
    Shader shader;
    BoneCollisionMesh bCollisionMesh;
};

class EnemyTestObject : public EntityWrapper {

public:
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    _3DM::AnimatedModel model = _3DM::AnimatedModel("assets/Models/player/player.3DMA");

    EntityTransform transform;
    GlobalInformation info;
    EnemyController controller = EnemyController(glm::vec3(0, -2, 0));
    CollisionMesh collisionMesh;
    SimpleMaterial material;
    Shader shader;
};

class CubeTrigger : public EntityWrapper {

public:
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    _3DM::Model model = _3DM::Model("assets/Models/cube.3DM");
    CollisionMesh collisionMesh;
    Material material;
    Shader shader;
};

class FloorObject : public EntityWrapper {
public:
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    _3DM::Model model = _3DM::Model("assets/Models/testlevel.3DM");
    _3DM::Model cm    = _3DM::Model("assets/Models/testlevel_cm.3DM");
    CollisionMesh collisionMesh;
    Material floorMaterial;
    Shader shader;
};

#endif
