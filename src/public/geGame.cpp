//
// Created by adria on 25/01/2019.
//

#include <game-engine/geGame.hpp>
#include "../private/GameObject.hpp"

namespace GameEngine {
    geGameObjectRef geGame::createObject(const std::string &name) {
        Internal::GameObject *object = new Internal::GameObject("");
        object->name(name);
        return geGameObjectRef(object);
    }
}