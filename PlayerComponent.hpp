//
// Created by adria on 29/10/2018.
//

#ifndef SPACEINVADERS_PLAYERCOMPONENT_HPP
#define SPACEINVADERS_PLAYERCOMPONENT_HPP


#include "src/Component.hpp"
#include "BulletComponent.hpp"
#include "src/components/AudioComponent.hpp"
#include "src/components/TextComponent.hpp"

enum class PlayerEvents{
    KILLED
};

COMPONENT(PlayerComponent)
class PlayerComponent : public Component , public Subject<PlayerEvents>{
    std::shared_ptr<GameObject> bullet_;
    std::weak_ptr<AudioComponent> shootSound_;
    std::weak_ptr<TextComponent> lifesCounter_;
    float speed_;
public:
    virtual ~PlayerComponent() = default;

    void init() override;

    void Update(float elapsedTime) override;

    std::shared_ptr<Component> Clone() override;

    void fromFile(const YAML::Node &componentConfig) override;

    //public player functions
    void kill();
};


#endif //SPACEINVADERS_PLAYERCOMPONENT_HPP
