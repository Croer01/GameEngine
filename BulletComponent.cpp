//
// Created by adria on 30/10/2018.
//

#include "BulletComponent.hpp"
#include "EnemyComponent.hpp"
#include "PlayerComponent.hpp"
#include "MotherShipComponent.hpp"

void BulletComponent::init() {
    gameObject()->setActive(false);
    collider_ = gameObject()->getComponent<ColliderComponent>();
    if(auto collider = collider_.lock())
        collider->setOnColliderEnter(std::bind(&BulletComponent::onColliderEnter,this,std::placeholders::_1));
}

void BulletComponent::Update(float elapsedTime) {
    //TODO: create getters to get virtual screen size
    if(auto collider = collider_.lock())
        collider->setVelocity(glm::vec3(0,-speed_,0));

    const glm::vec3 &pos = gameObject()->getPosition();
    if(pos.x < 0 || 224 < pos.x || pos.y < 0 || 256 < pos.y)
        gameObject()->setActive(false);
}

std::shared_ptr<Component> BulletComponent::Clone() {
    auto clone = std::make_shared<BulletComponent>();
    clone->speed_ = speed_;
    return clone;
}

void BulletComponent::fromFile(const YAML::Node &componentConfig) {
    speed_ = componentConfig["speed"].as<float>(0);
}

void BulletComponent::onColliderEnter(ColliderComponent *other) {
    if(!gameObject()->isActive())
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

    gameObject()->setActive(false);
}

