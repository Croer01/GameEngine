//
// Created by adria on 19/01/2019.
//

#ifndef SPACEINVADERS_SHELTERCOMPONENT_HPP
#define SPACEINVADERS_SHELTERCOMPONENT_HPP

#include "src/Component.hpp"
#include "src/components/ColliderComponent.hpp"
#include "src/components/SpriteAnimatedComponent.hpp"
#include "src/components/AudioComponent.hpp"

COMPONENT(ShelterComponent);
class ShelterComponent : public Component{
    int maxHits_;
    int currentHits_;
    std::weak_ptr<ColliderComponent> collider_;
    std::weak_ptr<SpriteAnimatedComponent> sprite_;
    std::weak_ptr<AudioComponent> audio_;

public:
    void init() override;

    std::shared_ptr<Component> Clone() override;
};


#endif //SPACEINVADERS_SHELTERCOMPONENT_HPP
