#include "Scene.h"

bool Scene::isEntityActive(const int32_t& id) {

    if (binarySearchForEntity(id) == false) {
        DBG_LOG("Entity not found (Scene.h isEntityActive())\n");
        return false;
    }
    return entities.at(id).isActive;
}

void Scene::setEntityActive(const int32_t& id, bool t) {

    if (binarySearchForEntity(id) == false) {
        DBG_LOG("Entity not found (Scene.h setEntityActive())\n");
        return;
    }

    entities.at(id).isActive = t;
}

int32_t Scene::generateEntity() {
    Entity newObject;

    newObject.id       = static_cast<int32_t>(entities.size());
    newObject.isActive = true;

    entities.push_back(newObject);

    std::sort(entities.begin(), entities.end(), [](Scene::Entity& g1, Scene::Entity& g2) { return g1.id < g2.id; });

    return newObject.id;
}

void Scene::addComponent(const int32_t& objectID, ComponentInterface& component) {

    if (binarySearchForEntity(objectID) == false) {
        return; //error
    }

    component.ID       = components.size();
    component.entityID = objectID;

    components.push_back(&component); //Add component pointer to vector of components

    std::sort(components.begin(), components.end(), [](ComponentInterface* g1, ComponentInterface* g2) { return g1->getType() < g2->getType(); });
}

const std::vector<Scene::Entity>* const Scene::getAllEntities() const {
    return &entities;
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
bool Scene::binarySearchForEntity(int32_t id) {

    //Binary search the sorted Entity Vector
    unsigned int mid   = 0;
    unsigned int left  = 0;
    unsigned int right = entities.size();

    while (left < right) {
        mid = left + (right - left) / 2;

        if (id > entities.at(mid).id) {
            left = mid + 1;
        } else if (id < entities.at(mid).id) {
            right = mid;
        } else {
            return true;
        }
    }

    return false;
}
