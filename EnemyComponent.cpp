//
// Created by adria on 28/10/2018.
//

#include "EnemyComponent.hpp"
#include "src/InputManager.hpp"
#include "src/components/SpriteComponent.hpp"

std::shared_ptr<Component> EnemyComponent::Clone() {
    std::shared_ptr<EnemyComponent> clone = std::make_shared<EnemyComponent>();
    clone->points_ = points_;
    return clone;
}

int EnemyComponent::getPoints() const {
    return points_;
}

void EnemyComponent::fromFile(const YAML::Node &componentConfig) {
    points_ = componentConfig["points"].as<int>(0);
}