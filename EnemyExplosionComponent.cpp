//
// Created by adria on 31/12/2018.
//

#include "EnemyExplosionComponent.hpp"

void EnemyExplosionComponent::init() {
    sound_ = gameObject()->getComponent<GameEngine::AudioComponent>();
    LifeTimeAcumulator = 0;
    gameObject()->registerObserver(this);
}

void EnemyExplosionComponent::Update(float elapsedTime) {
    LifeTimeAcumulator += elapsedTime;

    //150 ms
    if(LifeTimeAcumulator >= 0.15f)
        gameObject()->active(false);

}

void EnemyExplosionComponent::onEvent(const GameEngine::Subject<GameEngine::GameObjectEvent> &target, const GameEngine::GameObjectEvent &event, void *args) {
    if(event == GameEngine::GameObjectEvent::ActiveChanged && gameObject()->active()) {
        if(auto sound = sound_.lock())
            sound->play();
        LifeTimeAcumulator = 0;
    }
}
