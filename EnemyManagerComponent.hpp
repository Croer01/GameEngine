//
// Created by adria on 29/10/2018.
//

#ifndef SPACEINVADERS_ENEMYMANAGERCOMPONENT_HPP
#define SPACEINVADERS_ENEMYMANAGERCOMPONENT_HPP


#include "src/Component.hpp"
COMPONENT(EnemyManagerComponent)
class EnemyManagerComponent : public Component{
    std::vector<std::shared_ptr<GameObject>> enemies_;
    int totalWidth_;
    int totalHeight_;

    float speed_;
    float currentSpeed_;
    float scaleFactor_;
public:
    ~EnemyManagerComponent() override;

    void init() override;

    void Update(float elapsedTime) override;

    std::shared_ptr<Component> Clone() override;

    void fromFile(const YAML::Node &componentConfig) override;
};


#endif //SPACEINVADERS_ENEMYMANAGERCOMPONENT_HPP
