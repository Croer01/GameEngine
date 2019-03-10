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

    PropertySetBase &geComponent::properties() {
        if(!properties_)
            properties_ = std::unique_ptr<PropertySetBase>(configureProperties());
        return *properties_;
    }

    geComponentRef geComponent::clone() const {
        geComponentRef cloned = instantiate();
		properties_->copy(cloned->properties());
        return cloned;
    }

    PropertySetBase *geComponent::configureProperties() { return nullptr;}

    geComponentRef geComponent::instantiate() const {
        return std::make_shared<geComponent>();
    }
}