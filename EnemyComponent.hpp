//
// Created by adria on 28/10/2018.
//

#ifndef SPACEINVADERS_ENEMYCOMPONENT_HPP
#define SPACEINVADERS_ENEMYCOMPONENT_HPP

#include "src/Component.hpp"
#include "EnemyManagerComponent.hpp"
#include "src/components/AudioComponent.hpp"

COMPONENT(EnemyComponent);
class EnemyComponent : public Component {
    int points_;
    std::weak_ptr<EnemyManagerComponent> enemyManager_;
    std::weak_ptr<GameObject> explosion_;
public:
    virtual ~EnemyComponent() = default;

    void init() override;

    std::shared_ptr<Component> Clone() override;

    void fromFile(const YAML::Node &componentConfig) override;

    int getPoints() const;

    void kill();
    void setEnemeyManager(const std::shared_ptr<EnemyManagerComponent> &enemyManager);
};


#endif //SPACEINVADERS_ENEMYCOMPONENT_HPP
