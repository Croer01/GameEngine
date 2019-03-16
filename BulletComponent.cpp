//
// Created by adria on 30/10/2018.
//

#include "BulletComponent.hpp"
#include "EnemyComponent.hpp"
#include "PlayerComponent.hpp"
#include "MotherShipComponent.hpp"

void BulletComponent::init() {
    gameObject()->active(false);
    collider_ = gameObject()->getComponent<GameEngine::ColliderComponent>();
    if(auto collider = collider_.lock())
        collider->setOnColliderEnter(std::bind(&BulletComponent::onColliderEnter,this,std::placeholders::_1));
}

void BulletComponent::Update(float elapsedTime) {
    const GameEngine::geScreen &screen = gameObject()->game().screen();

    if(auto collider = collider_.lock())
        collider->setVelocity(glm::vec3(0,-speed_,0));

    const GameEngine::Vec2D &pos = gameObject()->position();
    if(pos.x < 0 || screen.virtualWidth() < pos.x || pos.y < 0 || screen.virtualHeight() < pos.y)
        gameObject()->active(false);
}

GameEngine::PropertySetBase *BulletComponent::configureProperties() {
    auto *properties = new GameEngine::PropertySet<BulletComponent>(this);

    properties->add(new GameEngine::Property<BulletComponent, float>(
            "speed",
            this,
            &BulletComponent::speed,
            &BulletComponent::speed,
            0));

    return properties;
}

void BulletComponent::onColliderEnter(GameEngine::ColliderComponent *other) {
    if(!gameObject()->active())
        return;

    std::shared_ptr<EnemyComponent> enemy = other->gameObject()->getComponent<EnemyComponent>().lock();

    //kill the enemy
    if(enemy){
        enemy->kill();
    }

    std::shared_ptr<PlayerComponent> player = other->gameObject()->getComponent<PlayerComponent>().lock();

    //kill the player
    if(player){
        player->kill();
    }

    //kill the mothership
    std::shared_ptr<MotherShipComponent> mothership = other->gameObject()->getComponent<MotherShipComponent>().lock();
    if(mothership){
        mothership->kill();
    }

    gameObject()->active(false);
}

void BulletComponent::speed(const float &value) {
    speed_ = value;
}

float BulletComponent::speed() const {
    return speed_;
}

