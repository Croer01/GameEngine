//
// Created by adria on 28/10/2018.
//

#ifndef SPACEINVADERS_ENEMYCOMPONENT_HPP
#define SPACEINVADERS_ENEMYCOMPONENT_HPP

#include "src/Component.hpp"

COMPONENT(EnemyComponent);
class EnemyComponent : public Component {
public:
    std::shared_ptr<Component> Clone() override;
};


#endif //SPACEINVADERS_ENEMYCOMPONENT_HPP
