//
// Created by adria on 28/10/2018.
//

#ifndef SPACEINVADERS_ENEMYCOMPONENT_HPP
#define SPACEINVADERS_ENEMYCOMPONENT_HPP

#include "src/Component.hpp"

COMPONENT(EnemyComponent);
class EnemyComponent : public Component {
    int points_;
public:
    std::shared_ptr<Component> Clone() override;

    void fromFile(const YAML::Node &componentConfig) override;

    int getPoints() const;
};


#endif //SPACEINVADERS_ENEMYCOMPONENT_HPP
