//
// Created by adria on 23/11/2018.
//

#include <iostream>
#include "MotherShipComponent.hpp"
#include "src/SceneManager.hpp"
#include "src/components/SpriteComponent.hpp"
#include "src/InputManager.hpp"
#include "src/components/TextComponent.hpp"

void MotherShipComponent::init() {
    points_ = minPoints_;
    playerBullet_ = SceneManager::GetInstance().findObjectByName("PlayerBullet");
    const std::shared_ptr<GameObject> &enemyManagerGameObject = SceneManager::GetInstance().findObjectByName("EnemyManager");
    if(enemyManagerGameObject)
        enemyManager_ = enemyManagerGameObject->getComponent<EnemyManagerComponent>();
    if(auto playerBullet = playerBullet_.lock())
        playerBullet->registerObserver(this);
    scoreText_ = SceneManager::GetInstance().createGameObject("MotherShipScoreText");
    scoreText_.lock()->setActive(false);
    explosion_ = SceneManager::GetInstance().createGameObject("EnemyExplosion");
    explosion_ .lock()->setActive(false);
    motherShipSoundObserver_ = std::make_shared<MotherShipObserver>(this);
}

void MotherShipComponent::Update(float elapsedTime) {
    if(speed_ == 0)
        return;

    glm::vec3 translation(0);
    translation.x-=speed_;
    
    auto sprite = gameObject()->getComponent<SpriteComponent>().lock();
    
    glm::vec3 desiredPos = gameObject()->getPosition() + (translation * elapsedTime);
    gameObject()->setPosition(desiredPos);

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
    if(auto scoreText = scoreText_.lock()) {
        glm::vec3 desiredPos = gameObject()->getPosition();
        if(desiredPos.x < 0)
            desiredPos.x = 0;
        desiredPos.y -=8;

        scoreText->setPosition(desiredPos);
        scoreText->setActive(true);
        scoreText->getComponent<TextComponent>().lock()->setText(std::to_string(points_));
    }
    if(auto explosion = explosion_.lock()){
        explosion->setPosition(gameObject()->getPosition());
        explosion->setActive(true);
    }
    hide();
}

void MotherShipComponent::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
    if(auto bullet = playerBullet_.lock()) {
        if (gameObject()->isActive() && event == GameObjectEvent::ActiveChanged && bullet->isActive()) {
            points_ = std::min(points_ + pointsPerBullet_, maxPoints_);
            std::cout << "current points: " << std::to_string(points_) << std::endl;
        }
    }
}

void MotherShipComponent::hide() {
    gameObject()->setActive(false);
    glm::vec3 vec = gameObject()->getPosition();
    vec.x = 244;
    gameObject()->setPosition(vec);
    points_ = minPoints_;
}

MotherShipObserver::MotherShipObserver(MotherShipComponent *mothership) {
    sound_ = mothership->gameObject()->getComponent<AudioComponent>();
    mothership->gameObject()->registerObserver(this);
}

void MotherShipObserver::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
    if(event == GameObjectEvent::ActiveChanged){
        if(auto sound = sound_.lock()) {
            if(sound->gameObject()->isActive())
                sound->play();
            else
                sound->stop();
        }
    }
}