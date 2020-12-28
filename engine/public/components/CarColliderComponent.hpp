//
// Created by adria on 27/12/2020.
//

#ifndef GAMEENGINE_CARCOLLIDERCOMPONENT_HPP
#define GAMEENGINE_CARCOLLIDERCOMPONENT_HPP

#include <game-engine/geComponent.hpp>
#include "../../private/physics/Collider.hpp"
#include "../../private/physics/PhysicsEngine.hpp"

namespace GameEngine
{
class CarColliderComponent : public geComponentInstantiable<CarColliderComponent>, public Observer<GameObjectEvent>
{
    Internal::PhysicsEngine *physicsEngine_;
    std::vector<std::shared_ptr<Internal::Collider>> tires_;
public:
    virtual ~CarColliderComponent();
    virtual void init();
    virtual void Update(float elapsedTime);
    virtual PropertySetBase *getProperties() const;
    virtual void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args);
};
}

#endif //GAMEENGINE_CARCOLLIDERCOMPONENT_HPP
