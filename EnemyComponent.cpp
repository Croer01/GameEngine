//
// Created by adria on 28/10/2018.
//

#include "EnemyComponent.hpp"
#include <game-engine/components/SpriteComponent.hpp>

void EnemyComponent::init() {
    explosion_ = gameObject()->game().createFromPrototype("EnemyExplosion");
    explosion_.lock()->active(false);
}
int EnemyComponent::points() const {
    return points_;
}

void EnemyComponent::points(const int &pointsValue) {
    points_ = pointsValue;
}

void EnemyComponent::kill() {
    gameObject()->active(false);
    if(auto enemyManager = enemyManager_.lock())
        enemyManager->enemyKilled(points_);

    if(auto explosion = explosion_.lock()){
        explosion->position(gameObject()->position());
        explosion->active(true);
    }
}

void EnemyComponent::setEnemeyManager(const std::shared_ptr<EnemyManagerComponent> &enemyManager) {
    enemyManager_ = enemyManager;
}

GameEngine::PropertySetBase *EnemyComponent::configureProperties() {
    auto *properties = new GameEngine::PropertySet<EnemyComponent>(this);

    properties->add(new GameEngine::Property<EnemyComponent, int>(
            "points",
            this,
            &EnemyComponent::points,
            &EnemyComponent::points,
            0));

    return properties;
}
