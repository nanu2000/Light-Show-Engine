#ifndef SYSTEM_BASE_H
#define SYSTEM_BASE_H
#include "Messages.h"
#include "SystemVitals.h"
class SystemBase {
public:
    virtual void recieveMessage(const BackEndMessages& msg, Scene& currentScene, Engine::SystemVitals& systemVitals) {}
    virtual void initialize(Scene& currentScene, Engine::SystemVitals& systemVitals) {}
};

#endif
