//
// Created by adria on 29/10/2018.
//

#include "EnemyManagerComponent.hpp"
#include "src/SceneManager.hpp"
#include "src/components/SpriteComponent.hpp"

void EnemyManagerComponent::init() {
    currentSpeed_ = speed_;

    int rows = rowsConfig_.enemiesType.size();
    int columns = rowsConfig_.enemiesPerRow;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            const std::shared_ptr<GameObject> &alien = SceneManager::GetInstance().createGameObject(rowsConfig_.enemiesType[i]);
            enemies_.push_back(alien);
        }
    }
    totalWidth_ = enemies_[0]->getComponent<SpriteComponent>()->getWidth() * columns;
    totalHeight_ = enemies_[0]->getComponent<SpriteComponent>()->getHeight() * rows;
}

void EnemyManagerComponent::Update(float elapsedTime) {
    float min = 0.f;
    float max = 224.f - totalWidth_;

    int rows = rowsConfig_.enemiesType.size();
    int columns = rowsConfig_.enemiesPerRow;

    if ((parent_->getPosition()[0] <= min && currentSpeed_ < 0) || (parent_->getPosition()[0] >= max && currentSpeed_ > 0)) {
        currentSpeed_ = -currentSpeed_ * (1.f + scaleFactor_);
        parent_->setPosition(parent_->getPosition() + glm::vec3(0, enemies_[0]->getComponent<SpriteComponent>()->getHeight(), 0));
    }
    parent_->setPosition(parent_->getPosition() + glm::vec3(currentSpeed_ * elapsedTime, 0, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            const std::shared_ptr<GameObject> &alien = enemies_[i * columns + j];
            alien->setPosition(parent_->getPosition() + glm::vec3(
                    j * alien->getComponent<SpriteComponent>()->getWidth() + j * rowsConfig_.horizontalMargin,
                    i * alien->getComponent<SpriteComponent>()->getHeight() + i * rowsConfig_.verticalMargin,
                    0));
        }
    }
}

std::shared_ptr<Component> EnemyManagerComponent::Clone() {
    std::shared_ptr<EnemyManagerComponent> clone = std::make_shared<EnemyManagerComponent>();
    clone->speed_ = speed_;
    clone->scaleFactor_ = scaleFactor_;
    clone->rowsConfig_ = rowsConfig_;
    return clone;
}

void EnemyManagerComponent::fromFile(const YAML::Node &componentConfig) {
    speed_ = componentConfig["speed"].as<float>();
    scaleFactor_ = componentConfig["scaleFactor"].as<float>();

    rowsConfig_ = RowsConfig();
    rowsConfig_.enemiesPerRow = componentConfig["enemiesPerRow"].as<int>();
    rowsConfig_.horizontalMargin = componentConfig["margin"][0].as<int>();
    rowsConfig_.verticalMargin = componentConfig["margin"][1].as<int>();

    YAML::Node rowsConfigNode = componentConfig["rowsConfig"];

    for (int i = 0; i < rowsConfigNode.size(); ++i) {
        rowsConfig_.enemiesType.push_back(rowsConfigNode[i].as<std::string>());
    }
}

EnemyManagerComponent::~EnemyManagerComponent() {
    enemies_.clear();
}

