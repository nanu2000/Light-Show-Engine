#include "Scene.h"

bool Scene::isEntityActive(const int32_t& id) {

    if (binarySearchForGameObject(id) == false) {
        DBG_LOG("Entity not found (Scene.h isEntityActive())\n");
        return false;
    }
    return gameObjects.at(id).isActive;
}

void Scene::setEntityActive(const int32_t& id, bool t) {

    if (binarySearchForGameObject(id) == false) {
        DBG_LOG("Entity not found (Scene.h setEntityActive())\n");
        return;
    }

    gameObjects.at(id).isActive = t;
}

int32_t Scene::generateGameObject() {
    GameObject newObject;

    newObject.id       = static_cast<int32_t>(gameObjects.size());
    newObject.isActive = true;

    gameObjects.push_back(newObject);

    std::sort(gameObjects.begin(), gameObjects.end(), [](Scene::GameObject& g1, Scene::GameObject& g2) { return g1.id < g2.id; });

    return newObject.id;
}

void Scene::addComponent(const int32_t& objectID, ComponentInterface& component) {

    if (binarySearchForGameObject(objectID) == false) {
        return; //error
    }

    component.ID       = components.size();
    component.entityID = objectID;

    components.push_back(&component); //Add component pointer to vector of components

    std::sort(components.begin(), components.end(), [](ComponentInterface* g1, ComponentInterface* g2) { return g1->getType() < g2->getType(); });
}

const std::vector<Scene::GameObject>* const Scene::getAllGameObjects() const {
    return &gameObjects;
}

int Scene::binarySearchForComponent(int32_t type, int32_t entityID) {

    unsigned int mid   = 0;
    unsigned int left  = 0;
    unsigned int right = components.size();

    while (left < right) {
        mid = left + (right - left) / 2;

        if (type > components.at(mid)->getType()) {
            left = mid + 1;
        } else if (type < components.at(mid)->getType()) {
            right = mid;
        } else {

            unsigned int origin = mid;

            if (components.at(mid)->getEntityID() == entityID) {
                return mid;
            }

            while (mid != 0 && type == components.at(mid - 1)->getType()) {
                mid--;

                if (components.at(mid)->getEntityID() == entityID) {
                    return mid;
                }
            }

            mid = origin;

            while (mid < components.size() - 1 && type == components.at(mid + 1)->getType()) {
                mid++;

                if (components.at(mid)->getEntityID() == entityID) {
                    return mid;
                }
            }

            return -1;
        }
    }

    return -1;
}

int Scene::binarySearchForComponent(int32_t type) {

    unsigned int mid   = 0;
    unsigned int left  = 0;
    unsigned int right = components.size();

    while (left < right) {
        mid = left + (right - left) / 2;

        if (type > components.at(mid)->getType()) {
            left = mid + 1;
        } else if (type < components.at(mid)->getType()) {
            right = mid;
        } else {
            while (mid != 0 && type == components.at(mid - 1)->getType()) {
                mid--;
            }
            return mid;
        }
    }

    return -1;
}

//Checks if an entity with the id given exists
bool Scene::binarySearchForGameObject(int32_t id) {

    //Binary search the sorted gameObject Vector
    unsigned int mid   = 0;
    unsigned int left  = 0;
    unsigned int right = gameObjects.size();

    while (left < right) {
        mid = left + (right - left) / 2;

        if (id > gameObjects.at(mid).id) {
            left = mid + 1;
        } else if (id < gameObjects.at(mid).id) {
            right = mid;
        } else {
            return true;
        }
    }

    return false;
}
