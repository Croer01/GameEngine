//
// Created by adria on 29/10/2018.
//

#include <iostream>
#include <ctime>
#include "EnemyManagerComponent.hpp"
#include "src/SceneManager.hpp"
#include "src/components/SpriteComponent.hpp"
#include "EnemyComponent.hpp"

void EnemyManagerComponent::init() {
    currentSpeed_ = speed_;
    currentPosition_ = parent_->getPosition();
    currentBulletFrequency_ = bulletFrequency_;
    //generate enemies
    int rows = rowsConfig_.enemiesType.size();
    int columns = rowsConfig_.enemiesPerRow;
    const std::shared_ptr<EnemyManagerComponent> &manager = shared_from_this();

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            const std::shared_ptr<GameObject> &alien = SceneManager::GetInstance().createGameObject(rowsConfig_.enemiesType[i]);
            enemies_.push_back(alien);
            if (auto enemy = alien->getComponent<EnemyComponent>().lock())
                enemy->setEnemeyManager(manager);
        }
    }
    if(auto firstEnemy = enemies_[0]->getComponent<SpriteComponent>().lock())
        boundingBox_ = glm::vec4(0.f, 0.f, firstEnemy->getWidth() * columns,firstEnemy->getHeight() * rows);

    //generate bullets
    for(int i = 0; i < bulletsNum_; i++){
        const std::shared_ptr<GameObject> &bullet= SceneManager::GetInstance().createGameObject("EnemyBullet");
        bullet->setActive(false);
        bullets_.push_back(bullet);
    }
    rng_ = std::mt19937(std::time(0));
}

void EnemyManagerComponent::Update(float elapsedTime) {
    float min = 0;
    float max = 224.f;

    if ((currentPosition_.x + boundingBox_.x <= min && currentSpeed_ < 0) || (currentPosition_.x + boundingBox_.z >= max && currentSpeed_ > 0)) {
        currentSpeed_ = -currentSpeed_;
        if(auto firstEnemy = enemies_[0]->getComponent<SpriteComponent>().lock())
            currentPosition_ += glm::vec3(0, firstEnemy->getHeight(), 0);
    }
    currentPosition_ += glm::vec3(currentSpeed_ * elapsedTime, 0, 0);

    int rows = rowsConfig_.enemiesType.size();
    int columns = rowsConfig_.enemiesPerRow;

    std::vector<int> enemiesActive;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            int index = i * columns + j;
            const std::shared_ptr<GameObject> &alien = enemies_[index];
            if(auto sprite = alien->getComponent<SpriteComponent>().lock()) {
                alien->setPosition(currentPosition_ + glm::vec3(
                        j * sprite->getWidth() + j * rowsConfig_.horizontalMargin,
                        i * sprite->getHeight() + i * rowsConfig_.verticalMargin,
                        0));
            }
            if(alien->isActive())
                enemiesActive.push_back(index);
        }
    }

    bulletTimeAcumulator_ += elapsedTime;
    if(currentBulletFrequency_ <= bulletTimeAcumulator_) {
        int i = 0;
        std::shared_ptr<GameObject> bullet;
        while(!bullet && i < bullets_.size()){
            if(!bullets_[i]->isActive())
                bullet = bullets_[i];
            i++;
        }

        if(bullet){
            std::uniform_int_distribution<int> rndDist(0,enemiesActive.size() - 1);
            int index = rndDist(rng_);
            bullet->setPosition(enemies_[enemiesActive[index]]->getPosition());
            bullet->setActive(true);
        }
        bulletTimeAcumulator_ = 0;
    }
}

std::shared_ptr<Component> EnemyManagerComponent::Clone() {
    std::shared_ptr<EnemyManagerComponent> clone = std::make_shared<EnemyManagerComponent>();
    clone->speed_ = speed_;
    clone->scaleFactor_ = scaleFactor_;
    clone->rowsConfig_ = rowsConfig_;
    clone->bulletsNum_ = bulletsNum_;
    clone->bulletFrequency_ = bulletFrequency_;

    return clone;
}

void EnemyManagerComponent::fromFile(const YAML::Node &componentConfig) {
    speed_ = componentConfig["speed"].as<float>();
    scaleFactor_ = componentConfig["scaleFactor"].as<float>();

    bulletsNum_ = componentConfig["bulletsNum"].as<int>();
    bulletFrequency_ = componentConfig["bulletFrequency"].as<float>();

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

void EnemyManagerComponent::enemyKilled(int EnemyKilledpoints) {
    currentSpeed_ *= 1.f + scaleFactor_;
    currentBulletFrequency_ *= 1.f - scaleFactor_;
    score_ += EnemyKilledpoints;
    notify(EnemyManagerEvents::ScoreChanged);
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

        if(min.x > enemy->getPosition().x)
            min.x = enemy->getPosition().x;

        if(max.x < enemy->getPosition().x)
            max.x = enemy->getPosition().x;


        if(min.y > enemy->getPosition().y)
            min.y = enemy->getPosition().y;

        if(max.y < enemy->getPosition().y)
            max.y = enemy->getPosition().y;
    }

    //move the calculated box to the world origin and add the width of the enemies
    min -= currentPosition_;
    max -= currentPosition_;
    if(auto firstEnemy = enemies_[0]->getComponent<SpriteComponent>().lock())
        boundingBox_ = glm::vec4(min.x , min.y,
                             max.x + firstEnemy->getWidth(),
                             max.y + firstEnemy->getHeight());
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
        currentPosition_ = parent_->getPosition();
        currentSpeed_ = std::abs(currentSpeed_);

        int rows = rowsConfig_.enemiesType.size();
        int columns = rowsConfig_.enemiesPerRow;

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                const std::shared_ptr<GameObject> &alien = enemies_[i * columns + j];
                if(auto sprite = alien->getComponent<SpriteComponent>().lock()) {
                    alien->setPosition(currentPosition_ + glm::vec3(
                            j * sprite->getWidth() + j * rowsConfig_.horizontalMargin,
                            i * sprite->getHeight() + i * rowsConfig_.verticalMargin,
                            0));
                }
                alien->setActive(true);

            }
        }
    }
}

int EnemyManagerComponent::getScore() const {
    return score_;
}

