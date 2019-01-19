//
// Created by adria on 19/01/2019.
//

#include <iostream>
#include "ShelterComponent.hpp"

void ShelterComponent::init() {
    maxHits_ = 4;
    currentHits_ = 0;

    collider_ = gameObject()->getComponent<ColliderComponent>();
    sprite_ = gameObject()->getComponent<SpriteAnimatedComponent>();
    audio_ = gameObject()->getComponent<AudioComponent>();

    collider_.lock()->setOnColliderEnter([&](ColliderComponent*) {
        currentHits_++;
        if(currentHits_ >= maxHits_) {
            gameObject()->setActive(false);
        }
        else {
            sprite_.lock()->setFrame(currentHits_);
            audio_.lock()->play();
        }
    });
}

std::shared_ptr<Component> ShelterComponent::Clone() {
     return std::make_shared<ShelterComponent>();
}
