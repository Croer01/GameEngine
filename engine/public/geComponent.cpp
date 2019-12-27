//
// Created by adria on 03/02/2019.
//

#include <game-engine/geComponent.hpp>

namespace GameEngine {
    void geComponent::gameObject(GameEngine::geGameObject *gameObject) {
        GameEngine::geGameObject *old = gameObject_;
        gameObject_ = gameObject;
        onGameObjectChange(old, gameObject);
    }

    geGameObject *geComponent::gameObject() const {
        return gameObject_;
    }

    PropertySetBase *geComponent::instantiateProperties() { return nullptr;}
}