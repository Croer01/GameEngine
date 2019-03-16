//
// Created by adria on 23/11/2018.
//

#include <iostream>
#include <game-engine/components/SpriteComponent.hpp>
#include <game-engine/components/TextComponent.hpp>
#include "MotherShipComponent.hpp"

void MotherShipComponent::init() {
    points_ = minPoints_;
    playerBullet_ = gameObject()->game().findObjectByNameInCurrentScene("PlayerBullet");
    const GameEngine::geGameObjectRef &enemyManagerGameObject = gameObject()->game().findObjectByNameInCurrentScene("EnemyManager");
    if(enemyManagerGameObject)
        enemyManager_ = enemyManagerGameObject->getComponent<EnemyManagerComponent>();
    if(auto playerBullet = playerBullet_.lock())
        playerBullet->registerObserver(this);
    scoreText_ = gameObject()->game().createFromPrototype("MotherShipScoreText");
    scoreText_.lock()->active(false);
    explosion_ = gameObject()->game().createFromPrototype("EnemyExplosion");
    explosion_ .lock()->active(false);
    motherShipSoundObserver_ = std::make_shared<MotherShipObserver>(this);
}

void MotherShipComponent::Update(float elapsedTime) {
    if(speed_ == 0)
        return;

    GameEngine::Vec2D translation;
    translation.x-=speed_;
    
    auto sprite = gameObject()->getComponent<GameEngine::SpriteComponent>().lock();

    GameEngine::Vec2D desiredPos = gameObject()->position() + (translation * elapsedTime);
    gameObject()->position(desiredPos);

    if(desiredPos.x + sprite->getWidth() < 0)
        hide();
}

GameEngine::PropertySetBase *MotherShipComponent::configureProperties() {
    auto *properties = new GameEngine::PropertySet<MotherShipComponent>(this);

    properties->add(new GameEngine::Property<MotherShipComponent, int>(
            "minPoints",
            this,
            &MotherShipComponent::minPoints,
            &MotherShipComponent::minPoints,
            0));
    properties->add(new GameEngine::Property<MotherShipComponent, int>(
            "maxPoints",
            this,
            &MotherShipComponent::maxPoints,
            &MotherShipComponent::maxPoints,
            0));
    properties->add(new GameEngine::Property<MotherShipComponent, int>(
            "pointsPerBullet",
            this,
            &MotherShipComponent::pointsPerBullet,
            &MotherShipComponent::pointsPerBullet,
            0));
    properties->add(new GameEngine::Property<MotherShipComponent, float>(
            "speed",
            this,
            &MotherShipComponent::speed,
            &MotherShipComponent::speed,
            0));
    return properties;
}

void MotherShipComponent::kill() {
    if(auto enemyManager = enemyManager_.lock())
        enemyManager->enemyKilled(points_);
    if(auto scoreText = scoreText_.lock()) {
        GameEngine::Vec2D desiredPos = gameObject()->position();
        if(desiredPos.x < 0)
            desiredPos.x = 0;
        desiredPos.y -=8;

        scoreText->position(desiredPos);
        scoreText->active(true);
        scoreText->getComponent<GameEngine::TextComponent>().lock()->text(std::to_string(points_));
    }
    if(auto explosion = explosion_.lock()){
        explosion->position(gameObject()->position());
        explosion->active(true);
    }
    hide();
}

void MotherShipComponent::onEvent(const GameEngine::Subject<GameEngine::GameObjectEvent> &target, const GameEngine::GameObjectEvent &event, void *args) {
    if(auto bullet = playerBullet_.lock()) {
        if (gameObject()->active() && event == GameEngine::GameObjectEvent::ActiveChanged && bullet->active()) {
            points_ = std::min(points_ + pointsPerBullet_, maxPoints_);
            std::cout << "current points: " << std::to_string(points_) << std::endl;
        }
    }
}

void MotherShipComponent::hide() {
    const GameEngine::geScreen &screen = gameObject()->game().screen();
    gameObject()->active(false);
    GameEngine::Vec2D vec = gameObject()->position();
    vec.x = screen.virtualWidth() + 20;
    gameObject()->position(vec);
    points_ = minPoints_;
}

void MotherShipComponent::minPoints(const int &points) {
    minPoints_ = points;
}

int MotherShipComponent::minPoints() const {
    return minPoints_;
}

void MotherShipComponent::maxPoints(const int &points) {
    maxPoints_ = points;
}

int MotherShipComponent::maxPoints() const {
    return maxPoints_;
}

void MotherShipComponent::pointsPerBullet(const int &points) {
    pointsPerBullet_ = points;
}

int MotherShipComponent::pointsPerBullet() const {
    return pointsPerBullet_;
}

void MotherShipComponent::speed(const float &speedValue) {
    speed_ = speedValue;
}

float MotherShipComponent::speed() const {
    return speed_;
}

MotherShipObserver::MotherShipObserver(MotherShipComponent *mothership) {
    sound_ = mothership->gameObject()->getComponent<GameEngine::AudioComponent>();
    mothership->gameObject()->registerObserver(this);
}

void MotherShipObserver::onEvent(const GameEngine::Subject<GameEngine::GameObjectEvent> &target, const GameEngine::GameObjectEvent &event, void *args) {
    if(event == GameEngine::GameObjectEvent::ActiveChanged){
        if(auto sound = sound_.lock()) {
            if(sound->gameObject()->active())
                sound->play();
            else
                sound->stop();
        }
    }
}