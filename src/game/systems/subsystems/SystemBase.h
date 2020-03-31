#ifndef SYSTEM_BASE_H
#define SYSTEM_BASE_H
#include "Messages.h"
class SystemBase {
public:
    virtual void recieveMessage(const BackEndMessages& msg, Scene& currentScene) {}
};

#endif
