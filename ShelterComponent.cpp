//
// Created by adria on 19/01/2019.
//

#include <iostream>
#include "ShelterComponent.hpp"

void ShelterComponent::init() {
    maxHits_ = 4;
    currentHits_ = 0;

    collider_ = parent_->getComponent<ColliderComponent>();
    sprite_ = parent_->getComponent<SpriteAnimatedComponent>();

    collider_.lock()->setOnColliderEnter([&](ColliderComponent*) {
        currentHits_++;

        if(currentHits_ >= maxHits_) {
            parent_->setActive(false);
        }
        else if(auto sprite = sprite_.lock())
            sprite->setFrame(currentHits_);

    });
}

std::shared_ptr<Component> ShelterComponent::Clone() {
     return std::make_shared<ShelterComponent>();
}
