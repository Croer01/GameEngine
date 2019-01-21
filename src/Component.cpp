//
// Created by adria on 22/09/2018.
//

#include "Component.hpp"
namespace GameEngine {
namespace Internal {
    void Component::gameObject(GameObject *gameObject) {
        gameObject_ = gameObject;
        onGameObjectChange(gameObject_, gameObject);
    }

    GameObject *Component::gameObject() const {
        return gameObject_;
    }
}
}