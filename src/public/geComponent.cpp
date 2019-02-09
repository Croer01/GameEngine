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
            properties_ = configureProperties();
        return *properties_;
    }
    void geComponent::properties(PropertySetBase &properties) {
        properties_ = &properties;
    }

    geComponentRef geComponent::clone() const {
        geComponentRef cloned = instantiate();
        PropertySetBase *propertiesCloned = cloned->configureProperties();
		cloned->properties(*propertiesCloned);
		properties_->copy(*propertiesCloned);
        return cloned;
    }

    PropertySetBase *geComponent::configureProperties() { return nullptr;}

    geComponent::~geComponent() {
        delete properties_;
    }

    geComponentRef geComponent::instantiate() const {
        return std::make_shared<geComponent>();
    }
}