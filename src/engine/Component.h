#ifndef COMPONENT_H
#define COMPONENT_H
#include <stdint.h>

class Scene;

//Inside a namespace because there is a global static that is needed.
namespace Important {
extern int32_t nextType;
}

class ComponentInterface {
public:
    virtual ~ComponentInterface() {}
    virtual int32_t getType() const = 0;
    int32_t getEntityID() const { return entityID; }
    void setEntityID(int32_t id) { entityID = id; }

    inline bool isActive() const { return active; }
    inline void setActive(bool isActive) { active = isActive; }

private:
    bool active      = true;
    int32_t ID       = -1;
    int32_t entityID = -1;

    friend Scene;
};

template <class T>
class Component : public ComponentInterface {
public:
    virtual ~Component() {}
    int32_t getType() const { return T::type; }
    static int32_t type;

private:
    friend Scene;
};

template <class T>
int32_t Component<T>::type = (Important::nextType++);
#endif // !COMPONENT_H
