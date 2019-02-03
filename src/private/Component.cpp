//
// Created by adria on 22/09/2018.
//

#include "Component.hpp"
namespace GameEngine {
namespace Internal {
    void Component::gameObject(GameEngine::geGameObject *gameObject) {
        GameEngine::geGameObject *old = gameObject_;
        gameObject_ = gameObject;
        onGameObjectChange(old, gameObject);
    }

    GameEngine::geGameObject *Component::gameObject() const {
        return gameObject_;
    }
}
}