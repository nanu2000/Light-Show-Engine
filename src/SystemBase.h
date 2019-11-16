#ifndef SYSTEM_BASE
#define SYSTEM_BASE

#include "SubSystems.h"

class SystemBase {

public:
    virtual void initialize(Scene& scene, Settings& settings, PhysicsWorld& world, SubSystems& systems) = 0;

    void setScene(Scene& scene) { currentScene = &scene; }
    void setCurrentSettings(Settings& settings) { currentSettings = &settings; }
    void setPhysicsWorld(PhysicsWorld& world) { physicsWorld = &world; }

    bool areVitalsNull() {
        if (!currentScene || !physicsWorld || !currentSettings || !systems) {
            DBG_LOG("Please Initialize Before Executing System!\n");
            return true;
        }
        return false;
    }

protected:
    SubSystems* systems;

    //Vitals
    PhysicsWorld* physicsWorld = nullptr;
    Settings* currentSettings  = nullptr;
    Scene* currentScene        = nullptr;
};

#endif // !SYSTEM_BASE
