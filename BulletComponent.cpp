//
// Created by adria on 30/10/2018.
//

#include "BulletComponent.hpp"
#include "EnemyComponent.hpp"

void BulletComponent::init() {
    parent_->setActive(false);
    collider_ = parent_->getComponent<ColliderComponent>();
    collider_->setOnColliderEnter(std::bind(&BulletComponent::onColliderEnter,this,std::placeholders::_1));
}

void BulletComponent::Update(float elapsedTime) {
    //TODO: create getters to get virtual screen size
    collider_->setVelocity(glm::vec3(0,-speed_,0));

    const glm::vec3 &pos = parent_->getPosition();
    if(pos.x < 0 || 224 < pos.x || pos.y < 0 || 256 < pos.y)
        parent_->setActive(false);
}

std::shared_ptr<Component> BulletComponent::Clone() {
    auto clone = std::make_shared<BulletComponent>();
    clone->speed_ = speed_;
    return clone;
}

void BulletComponent::fromFile(const YAML::Node &componentConfig) {
    speed_ = componentConfig["speed"].as<float>(0);
}

void BulletComponent::onColliderEnter(const std::shared_ptr<ColliderComponent> &other) {
    if(!parent_->isActive())
        return;

    std::shared_ptr<EnemyComponent> enemy = other->getParent()->getComponent<EnemyComponent>();

    //kill the enemy
    if(enemy){
        enemy->kill();
        parent_->setActive(false);
    }
}

