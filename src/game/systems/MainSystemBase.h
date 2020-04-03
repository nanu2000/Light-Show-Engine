#ifndef SYSTEM_BASE
#define SYSTEM_BASE

#include "SubSystems.h"
#include "SystemVitals.h"

class MainSystemBase {

public:
    virtual ~MainSystemBase() {}

    virtual void initialize(Scene& scene, Engine::SystemVitals& systemVitals, SubSystems& systems) = 0;

    bool areVitalsNull() {
        if (!currentScene || !systemVitals || !systems) {
            DBG_LOG("Please Initialize Before Executing System! MainSystemBase::areVitalsNull\n");
            return true;
        }
        return false;
    }

protected:
    //Vitals
    SubSystems* systems;
    Engine::SystemVitals* systemVitals = nullptr;
    Scene* currentScene                = nullptr;
};

#endif // !SYSTEM_BASE
