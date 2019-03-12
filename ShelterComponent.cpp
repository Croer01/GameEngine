//
// Created by adria on 19/01/2019.
//

#include <iostream>
#include "ShelterComponent.hpp"

void ShelterComponent::init() {
    maxHits_ = 4;
    currentHits_ = 0;

    collider_ = gameObject()->getComponent<GameEngine::ColliderComponent>();
    sprite_ = gameObject()->getComponent<GameEngine::SpriteAnimatedComponent>();
    audio_ = gameObject()->getComponent<GameEngine::AudioComponent>();

    collider_.lock()->setOnColliderEnter([&](GameEngine::ColliderComponent*) {
        currentHits_++;
        if(currentHits_ >= maxHits_) {
            gameObject()->active(false);
        }
        else {
            sprite_.lock()->setFrame(currentHits_);
            audio_.lock()->play();
        }
    });
}
