//
// Created by adria on 23/11/2018.
//

#include <iostream>
#include "MotherShipComponent.hpp"
#include "src/SceneManager.hpp"
#include "src/components/SpriteComponent.hpp"
#include "src/InputManager.hpp"

void MotherShipComponent::init() {
    points_ = minPoints_;
    playerBullet_ = SceneManager::GetInstance().findObjectByName("PlayerBullet");
    const std::shared_ptr<GameObject> &enemyManagerGameObject = SceneManager::GetInstance().findObjectByName("EnemyManager");
    if(enemyManagerGameObject)
        enemyManager_ = enemyManagerGameObject->getComponent<EnemyManagerComponent>();
    if(auto playerBullet = playerBullet_.lock())
        playerBullet->registerObserver(this);
}

void MotherShipComponent::Update(float elapsedTime) {
    if(speed_ == 0)
        return;

    glm::vec3 translation(0);
    translation.x-=speed_;
    
    auto sprite = parent_->getComponent<SpriteComponent>().lock();
    
    glm::vec3 desiredPos = parent_->getPosition() + (translation * elapsedTime);
    parent_->setPosition(desiredPos);

    if(desiredPos[0] + sprite->getWidth() < 0)
        hide();
}

std::shared_ptr<Component> MotherShipComponent::Clone() {
    auto clone = std::make_shared<MotherShipComponent>();
    clone->minPoints_ = minPoints_;
    clone->maxPoints_ = maxPoints_;
    clone->pointsPerBullet_ = pointsPerBullet_;
    clone->speed_ = speed_;
    
    return clone;
}

void MotherShipComponent::fromFile(const YAML::Node &componentConfig) {
    minPoints_ = componentConfig["minPoints"].as<int>(0);
    maxPoints_ = componentConfig["maxPoints"].as<int>(0);
    pointsPerBullet_ = componentConfig["pointsPerBullet"].as<int>(0);
    speed_ = componentConfig["speed"].as<float>(0);
}

void MotherShipComponent::kill() {
    if(auto enemyManager = enemyManager_.lock())
        enemyManager->enemyKilled(points_);
    hide();
}

void MotherShipComponent::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
    if(auto bullet = playerBullet_.lock()) {
        if (parent_->isActive() && event == GameObjectEvent::ActiveChanged && bullet->isActive()) {
            points_ = std::min(points_ + pointsPerBullet_, maxPoints_);
            std::cout << "current points: " << std::to_string(points_) << std::endl;
        }
    }
}

void MotherShipComponent::hide() {
    parent_->setActive(false);
    glm::vec3 vec = parent_->getPosition();
    vec.x = 244;
    parent_->setPosition(vec);
    points_ = minPoints_;
}
