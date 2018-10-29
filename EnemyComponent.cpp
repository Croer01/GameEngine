//
// Created by adria on 28/10/2018.
//

#include "EnemyComponent.hpp"
#include "src/InputManager.hpp"
#include "src/components/SpriteComponent.hpp"

std::shared_ptr<Component> EnemyComponent::Clone() {
    return std::make_shared<EnemyComponent>();
}
