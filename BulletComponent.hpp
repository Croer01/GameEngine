//
// Created by adria on 30/10/2018.
//

#ifndef SPACEINVADERS_BULLETCOMPONENT_HPP
#define SPACEINVADERS_BULLETCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/components/ColliderComponent.hpp>

COMPONENT(BulletComponent)
class BulletComponent : public GameEngine::geComponentInstantiable<BulletComponent>{
    float speed_;
    std::weak_ptr<GameEngine::ColliderComponent> collider_;
public:
    virtual ~BulletComponent() = default;
    void init() override;

    void Update(float elapsedTime) override;

    void onColliderEnter(GameEngine::ColliderComponent *other);

    void speed(const float &value);

    float speed() const;

protected:
    GameEngine::PropertySetBase *configureProperties() override;
};


#endif //SPACEINVADERS_BULLETCOMPONENT_HPP
