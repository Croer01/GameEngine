//
// Created by adria on 31/12/2018.
//

#ifndef SPACEINVADERS_ENEMYEXPLOSIONCOMPONENT_HPP
#define SPACEINVADERS_ENEMYEXPLOSIONCOMPONENT_HPP


#include "src/Component.hpp"
#include "src/components/AudioComponent.hpp"

COMPONENT(EnemyExplosionComponent)
class EnemyExplosionComponent : public Component, public Observer<GameObjectEvent> {
    std::weak_ptr<AudioComponent> sound_;
    float LifeTimeAcumulator;
public:
    void init() override;

    void Update(float elapsedTime) override;

    std::shared_ptr<Component> Clone() override;

    void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;
};


#endif //SPACEINVADERS_ENEMYEXPLOSIONCOMPONENT_HPP
