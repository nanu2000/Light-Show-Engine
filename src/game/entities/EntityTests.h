#ifndef ENTITY_TESTS
#define ENTITY_TESTS
#include "BoneCollisionMesh.h"
#include "Camera.h"
#include "CollisionMesh.h"
#include "DirectionalShadowDebugger.h"
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

//! Used to store vital information used to initialize entities.
class EntityVitals {
public:
    EntityVitals(Settings* set, Scene* scen, PhysicsWorld* phys, TextMap* text) {
        currentSettings = set;
        scene           = scen;
        thisWorld       = phys;
        map             = text;
    }

    Settings* currentSettings = nullptr;
    Scene* scene              = nullptr;
    PhysicsWorld* thisWorld   = nullptr;
    TextMap* map              = nullptr;

    bool checkForNulls() { return currentSettings && scene && thisWorld && map; }
};

//! Simple base class used for entities.
class EntityWrapper {
public:
    virtual void initialize(EntityVitals& vitals) = 0;

    virtual ~EntityWrapper() {};
};

//! Test entity class
class LightTest : public EntityWrapper {

public:
    void initialize(EntityVitals& vitals);
    ~LightTest() {}

private:
    std::vector<PointLight> lights = std::vector<PointLight>(4);
    DirectionalLight directionalLight;
    SkyBox skyBox;
    Shader skyBoxShader;
};

//! Test entity class
class ParticleTest : public EntityWrapper {

public:
    ~ParticleTest() {}
    void initialize(EntityVitals& vitals);

private:
    Particles particles = Particles(2000);
    Shader particleShader;
};

//! Test entity class
//! The Player contains all of the necessary ui and controls to move and view the PlayerTestObject entity.
class Player : public EntityWrapper {

public:
    ~Player() {}
    void initialize(EntityVitals& vitals);

private:
    DisplayStatistics statDisplayer;
    Shader textShader;
    PauseMenu menu;
    DirectionalShadowDebugger dirShadowDbgr;
    Camera camera;
    UserControls userControls;
};

//! Test entity class
class PlayerTestObject : public EntityWrapper {

public:
    ~PlayerTestObject() {}
    void initialize(EntityVitals& vitals);

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

//! Test entity class
class EnemyTestObject : public EntityWrapper {

public:
    ~EnemyTestObject() {}
    void initialize(EntityVitals& vitals);

private:
    _3DM::AnimatedModel model = _3DM::AnimatedModel("assets/Models/player/testplayer.3DMA");

    EntityTransform transform;
    GlobalInformation info;
    EnemyController controller = EnemyController(glm::vec3(0, .5f, 0));
    CollisionMesh collisionMesh;
    SimpleMaterial material;
    Shader shader;
};

//! Test entity class
class CubeTrigger : public EntityWrapper {

public:
    ~CubeTrigger() {}
    void initialize(EntityVitals& vitals);

private:
    _3DM::Model model = _3DM::Model("assets/Models/cube.3DM");
    CollisionMesh collisionMesh;
    Material material;
    Shader shader;
};

//! Test entity class
class FloorObject : public EntityWrapper {
public:
    ~FloorObject() {}
    void initialize(EntityVitals& vitals);

private:
    _3DM::Model model = _3DM::Model("assets/Models/testlevel.3DM");
    _3DM::Model cm    = _3DM::Model("assets/Models/testlevel_cm.3DM");
    CollisionMesh collisionMesh;
    Material floorMaterial;
    Shader shader;
};

namespace Entities {
    //! Used to map a string to an EntityWrapper type's constructor.
    template <typename T>
    static EntityWrapper* registerEntity() { return new T; }

    //! Map of entity constructors.
    static std::map<std::string, EntityWrapper* (*)()> entityTypes;

    //! Used to allocate a new entity via string registered with registerEntity. **Does not control lifetime of allocated entity.**
    static EntityWrapper* allocateEntity(const std::string& str) {

        std::map<std::string, EntityWrapper* (*)()>::iterator it = entityTypes.find(str);

        if (it != entityTypes.end()) {
            return entityTypes.at(str)();
        }

        DBG_LOG("Could not find entity with name `%s`.\n Perhaps this entity isn't registered in Entities::registerEntities(), or Entities::registerEntities() was never called in Game::init() ?\n", str.c_str());
        return nullptr;
    }

    //! Registers all entities so they can be created with Entities::allocateEntity via string.
    static void registerEntities() {
        entityTypes["LightTest"]        = &registerEntity<LightTest>;
        entityTypes["ParticleTest"]     = &registerEntity<ParticleTest>;
        entityTypes["Player"]           = &registerEntity<Player>;
        entityTypes["PlayerTestObject"] = &registerEntity<PlayerTestObject>;
        entityTypes["EnemyTestObject"]  = &registerEntity<EnemyTestObject>;
        entityTypes["CubeTrigger"]      = &registerEntity<CubeTrigger>;
        entityTypes["FloorObject"]      = &registerEntity<FloorObject>;
    }
}

#endif
