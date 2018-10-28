//
// Created by adria on 28/10/2018.
//

#include "EnemyComponent.hpp"
#include "src/InputManager.hpp"
#include "src/components/SpriteComponent.hpp"

void EnemyComponent::Update(float elapsedTime) {
    float min = 0.f;
    float max = 224.f - parent_->getComponent<SpriteComponent>()->getWidth();

    if ((parent_->getPosition()[0] <= min && currentSpeed_ < 0) || (parent_->getPosition()[0] >= max && currentSpeed_ > 0)) {
        currentSpeed_ = -currentSpeed_ * (1.f + scaleFactor_);
        parent_->setPosition(parent_->getPosition() + glm::vec3(0, parent_->getComponent<SpriteComponent>()->getHeight(), 0));
    }
        parent_->setPosition(parent_->getPosition() + glm::vec3(currentSpeed_ * elapsedTime, 0, 0));
}

std::shared_ptr<Component> EnemyComponent::Clone() {
    std::shared_ptr<EnemyComponent> clone = std::make_shared<EnemyComponent>();
    clone->speed_ = speed_;
    clone->currentSpeed_ = speed_;
    clone->scaleFactor_ = scaleFactor_;
    return clone;
}

void EnemyComponent::fromFile(const YAML::Node &componentConfig) {
    speed_ = componentConfig["speed"].as<float>();
    scaleFactor_ = componentConfig["scaleFactor"].as<float>();
}
