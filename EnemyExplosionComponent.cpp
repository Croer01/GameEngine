//
// Created by adria on 31/12/2018.
//

#include "EnemyExplosionComponent.hpp"

void EnemyExplosionComponent::init() {
    sound_ = parent_->getComponent<AudioComponent>();
    LifeTimeAcumulator = 0;
    parent_->registerObserver(this);
}

void EnemyExplosionComponent::Update(float elapsedTime) {
    LifeTimeAcumulator += elapsedTime;

    //150 ms
    if(LifeTimeAcumulator >= 0.15f)
        parent_->setActive(false);

}

void EnemyExplosionComponent::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
    if(event == GameObjectEvent::ActiveChanged && parent_->isActive()) {
        if(auto sound = sound_.lock())
            sound->play();
        LifeTimeAcumulator = 0;
    }
}

std::shared_ptr<Component> EnemyExplosionComponent::Clone() {
    auto clone = std::make_shared<EnemyExplosionComponent>();
    return clone;
}
