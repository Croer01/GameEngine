//
// Created by adria on 29/10/2018.
//

#include <game-engine/InputManager.hpp>
#include "PlayerComponent.hpp"

void PlayerComponent::init() {
    bullet_ = gameObject()->game().createFromPrototype("PlayerBullet");
    bullet_->name("PlayerBullet");
    shootSound_ = gameObject()->getComponent<GameEngine::AudioComponent>();
    sprite_ = gameObject()->getComponent<GameEngine::SpriteComponent>();
    spriteExplosion_ = gameObject()->getComponent<GameEngine::SpriteAnimatedComponent>();
    killed_ = false;

}

GameEngine::PropertySetBase *PlayerComponent::configureProperties() {
    auto *properties = new GameEngine::PropertySet<PlayerComponent>(this);

    properties->add(new GameEngine::Property<PlayerComponent, float>(
            "speed",
            this,
            &PlayerComponent::speed,
            &PlayerComponent::speed,
            0));

    return properties;
}

void PlayerComponent::Update(float elapsedTime) {
    if(killed_)
        return;

    GameEngine::Vec2D translation;
    auto sprite = gameObject()->getComponent<GameEngine::SpriteComponent>().lock();

    if (GameEngine::InputManager::GetInstance().isKeyPressed(GameEngine::KeyCode::KEY_RIGHT)) {
        translation.x+=speed_;
    } else if (GameEngine::InputManager::GetInstance().isKeyPressed(GameEngine::KeyCode::KEY_LEFT)) {
        translation.x-=speed_;
    }

    if(translation.x != 0){
        GameEngine::Vec2D desiredPos = gameObject()->position() + (translation * elapsedTime);

        if(desiredPos.x < 0)
            desiredPos.x = 0;
        else if(desiredPos.x + sprite->getWidth() > 224)
            desiredPos.x = 224 - sprite->getWidth();

        gameObject()->position(desiredPos);
    }

    if(!bullet_->active() && GameEngine::InputManager::GetInstance().isKeyPressed(GameEngine::KeyCode::KEY_SPACE)){
        bullet_->position(gameObject()->position() + GameEngine::Vec2D(sprite->getWidth()/2, 0));
        bullet_->active(true);
        if(auto shootSound = shootSound_.lock())
            shootSound->play();
    }
}

void PlayerComponent::kill() {
    sprite_.lock()->setVisible(false);
    spriteExplosion_.lock()->setVisible(true);
    notify(PlayerEvents::KILLED);
    killed_ = true;
    bullet_->active(false);
}

void PlayerComponent::restore() {
    sprite_.lock()->setVisible(true);
    spriteExplosion_.lock()->setVisible(false);
    gameObject()->position(GameEngine::Vec2D(106, 248));
    killed_ = false;
}

void PlayerComponent::speed(const float &value) {
    speed_ = value;
}

float PlayerComponent::speed() const {
    return speed_;
}
