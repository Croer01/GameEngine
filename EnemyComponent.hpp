//
// Created by adria on 28/10/2018.
//

#ifndef SPACEINVADERS_ENEMYCOMPONENT_HPP
#define SPACEINVADERS_ENEMYCOMPONENT_HPP

#include "src/Component.hpp"

COMPONENT(EnemyComponent);
class EnemyComponent : public Component {
    float speed_;
    float currentSpeed_;
    float scaleFactor_;
public:
    void Update(float elapsedTime) override;

    std::shared_ptr<Component> Clone() override;

    void fromFile(const YAML::Node &componentConfig) override;
};


#endif //SPACEINVADERS_ENEMYCOMPONENT_HPP
