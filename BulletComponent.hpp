//
// Created by adria on 30/10/2018.
//

#ifndef SPACEINVADERS_BULLETCOMPONENT_HPP
#define SPACEINVADERS_BULLETCOMPONENT_HPP


#include "src/Component.hpp"
#include "src/components/ColliderComponent.hpp"

COMPONENT(BulletComponent)
class BulletComponent : public Component{
    float speed_;
    std::weak_ptr<ColliderComponent> collider_;
public:
    virtual ~BulletComponent() = default;
    void init() override;

    void Update(float elapsedTime) override;

    std::shared_ptr<Component> Clone() override;

    void fromFile(const YAML::Node &componentConfig) override;

    void onColliderEnter(ColliderComponent *other);

};


#endif //SPACEINVADERS_BULLETCOMPONENT_HPP
