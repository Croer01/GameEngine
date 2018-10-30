//
// Created by adria on 30/10/2018.
//

#include "BulletComponent.hpp"

void BulletComponent::Update(float elapsedTime) {
    parent_->setPosition(parent_->getPosition() + glm::vec3(0, -speed_ * elapsedTime, 0));
}

std::shared_ptr<Component> BulletComponent::Clone() {
    auto clone = std::make_shared<BulletComponent>();
    clone->speed_ = speed_;
    return clone;
}

void BulletComponent::fromFile(const YAML::Node &componentConfig) {
    speed_ = componentConfig["speed"].as<float>(0);
}
