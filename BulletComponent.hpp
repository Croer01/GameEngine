//
// Created by adria on 30/10/2018.
//

#ifndef SPACEINVADERS_BULLETCOMPONENT_HPP
#define SPACEINVADERS_BULLETCOMPONENT_HPP


#include "src/Component.hpp"
COMPONENT(BulletComponent)
class BulletComponent : public Component {
    float speed_;
public:
    void Update(float elapsedTime) override;

    std::shared_ptr<Component> Clone() override;

    void fromFile(const YAML::Node &componentConfig) override;
};


#endif //SPACEINVADERS_BULLETCOMPONENT_HPP
