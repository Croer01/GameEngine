//
// Created by adria on 29/10/2018.
//

#include "EnemyManagerComponent.hpp"
#include "src/SceneManager.hpp"
#include "src/components/SpriteComponent.hpp"

void EnemyManagerComponent::init() {
    currentSpeed_ = speed_;

    int rows = 5;
    int columns = 11;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            const std::shared_ptr<GameObject> &alien1 = SceneManager::GetInstance().createGameObject("Alien1");
            alien1->Init();
            enemies_.push_back(alien1);
            alien1->setPosition(glm::vec3(j * alien1->getComponent<SpriteComponent>()->getWidth(), i * alien1->getComponent<SpriteComponent>()->getHeight(), 0));
        }
    }
    totalWidth_ = enemies_[0]->getComponent<SpriteComponent>()->getWidth() * columns;
    totalHeight_ = enemies_[0]->getComponent<SpriteComponent>()->getHeight() * rows;
}

void EnemyManagerComponent::Update(float elapsedTime) {
    float min = 0.f;
    float max = 224.f - totalWidth_;

    int rows = 5;
    int columns = 11;

    if ((parent_->getPosition()[0] <= min && currentSpeed_ < 0) || (parent_->getPosition()[0] >= max && currentSpeed_ > 0)) {
        currentSpeed_ = -currentSpeed_ * (1.f + scaleFactor_);
        parent_->setPosition(parent_->getPosition() + glm::vec3(0, enemies_[0]->getComponent<SpriteComponent>()->getHeight(), 0));
    }
    parent_->setPosition(parent_->getPosition() + glm::vec3(currentSpeed_ * elapsedTime, 0, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            const std::shared_ptr<GameObject> &alien1 = enemies_[i * columns + j];
            alien1->setPosition(parent_->getPosition() + glm::vec3(j * alien1->getComponent<SpriteComponent>()->getWidth(), i * alien1->getComponent<SpriteComponent>()->getHeight(), 0));
        }
    }
}

std::shared_ptr<Component> EnemyManagerComponent::Clone() {
    std::shared_ptr<EnemyManagerComponent> clone = std::make_shared<EnemyManagerComponent>();
    clone->speed_ = speed_;
    clone->scaleFactor_ = scaleFactor_;
    return clone;
}

void EnemyManagerComponent::fromFile(const YAML::Node &componentConfig) {
    speed_ = componentConfig["speed"].as<float>();
    scaleFactor_ = componentConfig["scaleFactor"].as<float>();
}

EnemyManagerComponent::~EnemyManagerComponent() {
    enemies_.clear();
}

