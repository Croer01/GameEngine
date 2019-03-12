//
// Created by adria on 28/10/2018.
//

#ifndef SPACEINVADERS_ENEMYCOMPONENT_HPP
#define SPACEINVADERS_ENEMYCOMPONENT_HPP

#include <game-engine/geComponent.hpp>
#include "EnemyManagerComponent.hpp"

COMPONENT(EnemyComponent);
class EnemyComponent : public GameEngine::geComponentInstantiable<EnemyComponent> {
    int points_;
    std::weak_ptr<EnemyManagerComponent> enemyManager_;
    std::weak_ptr<GameEngine::geGameObject> explosion_;
public:
    virtual ~EnemyComponent() = default;

    void init() override;

    int points() const;
    void points(const int &pointsValue);

    void kill();
    void setEnemeyManager(const std::shared_ptr<EnemyManagerComponent> &enemyManager);

protected:
    GameEngine::PropertySetBase *configureProperties() override;
};


#endif //SPACEINVADERS_ENEMYCOMPONENT_HPP
