//
// Created by adria on 31/12/2018.
//

#ifndef SPACEINVADERS_ENEMYEXPLOSIONCOMPONENT_HPP
#define SPACEINVADERS_ENEMYEXPLOSIONCOMPONENT_HPP


#include <game-engine/geGameObject.hpp>
#include <game-engine/geComponent.hpp>
#include <game-engine/components/AudioComponent.hpp>
#include <game-engine/events/Observer.hpp>

COMPONENT(EnemyExplosionComponent)
class EnemyExplosionComponent : public GameEngine::geComponentInstantiable<EnemyExplosionComponent>, public GameEngine::Observer<GameEngine::GameObjectEvent> {
    std::weak_ptr<GameEngine::AudioComponent> sound_;
    float LifeTimeAcumulator;
public:
    void init() override;

    void Update(float elapsedTime) override;

    void onEvent(const GameEngine::Subject<GameEngine::GameObjectEvent> &target, const GameEngine::GameObjectEvent &event, void *args) override;
};


#endif //SPACEINVADERS_ENEMYEXPLOSIONCOMPONENT_HPP
