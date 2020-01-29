#ifndef SCENE_H
#define SCENE_H
#include "Component.h"
#include "PhysicsWorld.h"
#include <algorithm>
#include <vector>

class Scene {
public:
    struct Entity {
        int32_t id;
        bool isActive = true;
    };

    bool isEntityActive(const int32_t& id);
    void setEntityActive(const int32_t& id, bool t);

    int32_t generateEntity();

    void addComponent(const int32_t& objectID, ComponentInterface& component);

    template <class T>
    T* getComponent(const int32_t& objectID) {

        if (binarySearchForEntity(objectID) == false) {
            return nullptr; //error, Entity does not exist
        }

        int index = binarySearchForComponent(T::type, objectID);

        if (index != -1) {
            return static_cast<T*>(components.at(index));
        }

        return nullptr;
    }

    template <typename T>
    T* getFirstComponentOfType() {
        std::vector<T*>& componentsOfType = getAllComponentsOfType<T>();
        int firstIndex                    = binarySearchForComponent(T::type);
        for (int i = 0; i < componentsOfType.size(); i++) {
            if (componentsOfType[i]->isActive()) {
                return componentsOfType[i];
            }
        }
        return nullptr;
    }
    template <typename T>
    T* getFirstActiveComponentOfType() {

        int firstIndex = binarySearchForComponent(T::type);

        if (firstIndex == -1) {
            return nullptr;
        } else {
            return static_cast<T*>(components.at(firstIndex));
        }
    }

    //Loop through all components of type and pass to a lambda.
    //returning false in lambda will result in a 'continue' like response.
    //returning true in lambda will end loop.
    template <typename T, typename func>
    void performOperationsOnAllOfType(func function) {
        const std::vector<T*>& type = getAllComponentsOfType<T>();
        for (unsigned int i = 0; i < type.size(); i++) {
            if (function(*type[i])) {
                return;
            }
        }
    }

    //Loop through all entities and pass to a lambda.
    //returning false in lambda will result in a 'continue' like response.
    //returning true in lambda will end loop.
    template <typename func>
    void loopEntities(func function) {

        for (unsigned int i = 0; i < entities.size(); i++) {
            if (function(entities.at(i))) {
                return;
            }
        }
    }

    template <typename T>
    std::vector<T*> getAllComponentsOfType() {
        std::vector<T*> returnVector;

        int firstIndex = binarySearchForComponent(T::type);

        if (firstIndex == -1) {
            return returnVector;
        }

        while (firstIndex < static_cast<int>(components.size()) && T::type == components.at(firstIndex)->getType()) {
            returnVector.push_back(static_cast<T*>(components.at(firstIndex)));
            firstIndex++;
        }

        return returnVector;
    }

    const std::vector<Entity>* const getAllEntities() const;

private:
    //Returns the first found assosiated component's index in an entity. Returns -1 on failure
    int binarySearchForComponent(int32_t type, int32_t entityID);

    //Returns the first found component's index (not assosiated with peticular entity). Returns -1 on failure
    int binarySearchForComponent(int32_t type);

    //Returns true if the entity exists.
    bool binarySearchForEntity(int32_t id);

    std::vector<ComponentInterface*> components;
    std::vector<Entity> entities;
};

#endif
