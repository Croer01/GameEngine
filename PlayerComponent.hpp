 //
// Created by adria on 29/10/2018.
//

#ifndef SPACEINVADERS_PLAYERCOMPONENT_HPP
#define SPACEINVADERS_PLAYERCOMPONENT_HPP


#include "BulletComponent.hpp"
#include <game-engine/components/AudioComponent.hpp>

enum class PlayerEvents{
    KILLED
};

COMPONENT(PlayerComponent)
class PlayerComponent : public GameEngine::geComponentInstantiable<PlayerComponent> , public GameEngine::Subject<PlayerEvents>{
    GameEngine::geGameObjectRef bullet_;
    std::weak_ptr<GameEngine::AudioComponent> shootSound_;
    std::weak_ptr<GameEngine::SpriteComponent> sprite_;
    std::weak_ptr<GameEngine::SpriteAnimatedComponent> spriteExplosion_;
    float speed_;
    bool killed_;
public:
    virtual ~PlayerComponent() = default;

    void init() override;

    void Update(float elapsedTime) override;

    //public player functions
    void kill();

    void restore();

protected:
    GameEngine::PropertySetBase *configureProperties() override;

public:

    void speed(const float &value);
    float speed() const;
};


#endif //SPACEINVADERS_PLAYERCOMPONENT_HPP
