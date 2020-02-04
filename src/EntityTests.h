#ifndef ENTITY_TESTS
#define ENTITY_TESTS
#include "BoneCollisionMesh.h"
#include "Camera.h"
#include "CollisionMesh.h"
#include "DebuggingController.h"
#include "DisplayStatistics.h"
#include "EnemyController.h"
#include "EntityTransform.h"
#include "GlobalInformation.h"
#include "GuiSprite.h"
#include "GuiString.h"
#include "Lights.h"
#include "Material.h"
#include "Particles.h"
#include "PauseMenu.h"
#include "PlayerCameraHandler.h"
#include "PlayerController.h"
#include "Scene.h"
#include "Settings.h"
#include "Shader.h"
#include "SkyBox.h"
#include "TextMap.h"
#include "UserControls.h"

enum class ENTITY_NAME {
    LightTest        = 0,
    ParticleTest     = 1,
    Player           = 2,
    PlayerTestObject = 3,
    EnemyTestObject  = 4,
    CubeTrigger      = 5,
    FloorObject      = 6,
    ENTITY_COUNT     = 7
};

static const char* EntityNames[7] = {
    "LightTest",
    "ParticleTest",
    "Player",
    "PlayerTestObject",
    "EnemyTestObject",
    "CubeTrigger",
    "FloorObject"
};

class EntityWrapper {
public:
    struct EntityVitals {
        Settings* currentSettings = nullptr;
        Scene* scene              = nullptr;
        PhysicsWorld* thisWorld   = nullptr;
        TextMap* map              = nullptr;

        bool checkForNulls() { return currentSettings && scene && thisWorld && map; }
    };

    virtual void initialize(EntityVitals& vitals) = 0;

    virtual ~EntityWrapper() {};
};

class LightTest : public EntityWrapper {

public:
    void initialize(EntityWrapper::EntityVitals& vitals);
    ~LightTest() {}

private:
    std::vector<PointLight> lights = std::vector<PointLight>(4);
    DirectionalLight directionalLight;
    SkyBox skyBox;
    Shader skyBoxShader;
};

class ParticleTest : public EntityWrapper {

public:
    ~ParticleTest() {}
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    Particles particles = Particles(2000);
    Shader particleShader;
};

class Player : public EntityWrapper {

public:
    ~Player() {}
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    DisplayStatistics statDisplayer;
    Shader textShader;
    PauseMenu menu;

    Camera camera;
    //DebuggingController dbgController;
    UserControls userControls;
};

class PlayerTestObject : public EntityWrapper {

public:
    ~PlayerTestObject() {}
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    _3DM::AnimatedModel model = _3DM::AnimatedModel("assets/Models/player/testplayer.3DMA");

    Shader textShader;

    PlayerCameraHandler cameraHandler;
    CollisionMesh collisionMesh;
    PlayerController testController = PlayerController(glm::vec3(0, .5f, 0));
    SimpleMaterial material;
    Shader shader;
    BoneCollisionMesh bCollisionMesh;
};

class EnemyTestObject : public EntityWrapper {

public:
    ~EnemyTestObject() {}
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    _3DM::AnimatedModel model = _3DM::AnimatedModel("assets/Models/player/testplayer.3DMA");

    EntityTransform transform;
    GlobalInformation info;
    EnemyController controller = EnemyController(glm::vec3(0, .5f, 0));
    CollisionMesh collisionMesh;
    SimpleMaterial material;
    Shader shader;
};

class CubeTrigger : public EntityWrapper {

public:
    ~CubeTrigger() {}
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    _3DM::Model model = _3DM::Model("assets/Models/cube.3DM");
    CollisionMesh collisionMesh;
    Material material;
    Shader shader;
};

class FloorObject : public EntityWrapper {
public:
    ~FloorObject() {}
    void initialize(EntityWrapper::EntityVitals& vitals);

private:
    _3DM::Model model = _3DM::Model("assets/Models/testlevel.3DM");
    _3DM::Model cm    = _3DM::Model("assets/Models/testlevel_cm.3DM");
    CollisionMesh collisionMesh;
    Material floorMaterial;
    Shader shader;
};

template <typename T>
static EntityWrapper* createInstance() { return new T; }

typedef std::map<std::string, EntityWrapper* (*)()> EntityMap;

static EntityMap entityMap;

static const char* getEntityName(const ENTITY_NAME& name) {
    if (name < ENTITY_NAME::ENTITY_COUNT) {
        return EntityNames[static_cast<int32_t>(name)];
    }
    return "NA";
}

static void initMap() {
    entityMap[getEntityName(ENTITY_NAME::LightTest)]        = &createInstance<LightTest>;
    entityMap[getEntityName(ENTITY_NAME::ParticleTest)]     = &createInstance<ParticleTest>;
    entityMap[getEntityName(ENTITY_NAME::Player)]           = &createInstance<Player>;
    entityMap[getEntityName(ENTITY_NAME::PlayerTestObject)] = &createInstance<PlayerTestObject>;
    entityMap[getEntityName(ENTITY_NAME::EnemyTestObject)]  = &createInstance<EnemyTestObject>;
    entityMap[getEntityName(ENTITY_NAME::CubeTrigger)]      = &createInstance<CubeTrigger>;
    entityMap[getEntityName(ENTITY_NAME::FloorObject)]      = &createInstance<FloorObject>;
}
static EntityWrapper* getEntity(const std::string& str) {
    return entityMap[str]();
}
#endif
