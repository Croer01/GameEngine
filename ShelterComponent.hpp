//
// Created by adria on 19/01/2019.
//

#ifndef SPACEINVADERS_SHELTERCOMPONENT_HPP
#define SPACEINVADERS_SHELTERCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/components/ColliderComponent.hpp>
#include <game-engine/components/SpriteAnimatedComponent.hpp>
#include <game-engine/components/AudioComponent.hpp>

COMPONENT(ShelterComponent);
class ShelterComponent : public GameEngine::geComponentInstantiable<ShelterComponent> {
    int maxHits_;
    int currentHits_;
    std::weak_ptr<GameEngine::ColliderComponent> collider_;
    std::weak_ptr<GameEngine::SpriteAnimatedComponent> sprite_;
    std::weak_ptr<GameEngine::AudioComponent> audio_;

public:
    void init() override;
};


#endif //SPACEINVADERS_SHELTERCOMPONENT_HPP
