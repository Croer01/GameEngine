//
// Created by adria on 29/10/2018.
//

#include "EnemyManagerComponent.hpp"
#include "src/SceneManager.hpp"
#include "src/components/SpriteComponent.hpp"
#include "EnemyComponent.hpp"

void EnemyManagerComponent::init() {
    currentSpeed_ = speed_;

    int rows = rowsConfig_.enemiesType.size();
    int columns = rowsConfig_.enemiesPerRow;
    const std::shared_ptr<EnemyManagerComponent> &manager = std::shared_ptr<EnemyManagerComponent>(this);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            const std::shared_ptr<GameObject> &alien = SceneManager::GetInstance().createGameObject(rowsConfig_.enemiesType[i]);
            enemies_.push_back(alien);
            alien->getComponent<EnemyComponent>()->setEnemeyManager(manager);
        }
    }
    boundingBox_ = glm::vec4(0.f, 0.f,
            enemies_[0]->getComponent<SpriteComponent>()->getWidth() * columns,
            enemies_[0]->getComponent<SpriteComponent>()->getHeight() * rows);
}

void EnemyManagerComponent::Update(float elapsedTime) {
    float min = 0;
    float max = 224.f;

    if ((parent_->getPosition()[0] + boundingBox_.x <= min && currentSpeed_ < 0) || (parent_->getPosition()[0] + boundingBox_.z >= max && currentSpeed_ > 0)) {
        currentSpeed_ = -currentSpeed_;
        parent_->setPosition(parent_->getPosition() + glm::vec3(0, enemies_[0]->getComponent<SpriteComponent>()->getHeight(), 0));
    }
    parent_->setPosition(parent_->getPosition() + glm::vec3(currentSpeed_ * elapsedTime, 0, 0));

    int rows = rowsConfig_.enemiesType.size();
    int columns = rowsConfig_.enemiesPerRow;

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

void EnemyManagerComponent::enemyKilled() {
    currentSpeed_ *= 1.f + scaleFactor_;
    checkMoveToNextLevel();
    updateBoundingBox();
}

void EnemyManagerComponent::updateBoundingBox() {

    glm::vec3 min = glm::vec3(100000);
    glm::vec3 max = glm::vec3(0);

    //recalculate the bounding box
    for(const std::shared_ptr<GameObject> &enemy : enemies_){
        if(!enemy->isActive())
            continue;

        if(glm::all(glm::lessThanEqual(enemy->getPosition(),min)))
            min = enemy->getPosition();
        else if(glm::all(glm::greaterThanEqual(enemy->getPosition(),max)))
            max = enemy->getPosition();
    }

    //move the calculated box to the world origin and add the width of the enemies
    min -= parent_->getPosition();
    max -= parent_->getPosition();
    boundingBox_ = glm::vec4(min.x , min.y,
                             max.x + enemies_[0]->getComponent<SpriteComponent>()->getWidth(),
                             max.y + enemies_[0]->getComponent<SpriteComponent>()->getHeight());
}

void EnemyManagerComponent::checkMoveToNextLevel() {

    bool allEnemiesKilled = true;
    auto it = enemies_.begin();
    while (allEnemiesKilled && it != enemies_.end()){
        if((*it)->isActive())
            allEnemiesKilled = false;
        it++;
    }

    if(allEnemiesKilled){
        parent_->setPosition(glm::vec3(0));
        currentSpeed_ = std::abs(currentSpeed_);

        int rows = rowsConfig_.enemiesType.size();
        int columns = rowsConfig_.enemiesPerRow;

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                const std::shared_ptr<GameObject> &alien = enemies_[i * columns + j];
                alien->setPosition(parent_->getPosition() + glm::vec3(
                        j * alien->getComponent<SpriteComponent>()->getWidth() + j * rowsConfig_.horizontalMargin,
                        i * alien->getComponent<SpriteComponent>()->getHeight() + i * rowsConfig_.verticalMargin,
                        0));
                alien->setActive(true);

            }
        }
    }
}

