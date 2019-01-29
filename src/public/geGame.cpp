//
// Created by adria on 25/01/2019.
//

#include <game-engine/geGame.hpp>
#include "../private/GameObject.hpp"
#include "../private/ObjectManager.hpp"

namespace GameEngine {
    geGameObjectRef geGame::createObject(const std::string &name) {
        Internal::GameObject *object = new Internal::GameObject("");
        object->name(name);
        return geGameObjectRef(object);
    }

    void geGame::configEnvironment(const geEnvironment &environment) {

    }

    geGameObjectRef geGame::createFromPrototype(const std::string &prototype) {
        std::shared_ptr<Internal::GameObject> object = Internal::ObjectManager::GetInstance().createGameObject(prototype);
        return object;
    }
}