//
// Created by adria on 31/10/2018.
//

#ifndef SPACEINVADERS_PHYSICSENGINE_HPP
#define SPACEINVADERS_PHYSICSENGINE_HPP


#include <memory>
#include <Box2D/Box2D.h>
#include "../Singleton.hpp"
#include "Collider.hpp"

class PhysicsEngine : public Singleton<PhysicsEngine>, b2ContactListener{
    float timeStep_;
    float timeAcumulator_;
    int velocityIterations_;
    int positionIterations_;
    std::unique_ptr<b2World> world_;
public:
private:
    virtual void BeginContact(b2Contact *contact);

public:
    void init(float timeStep);
    void update(float elapsedTime);
    void registerCollider(const std::shared_ptr<Collider> &collider);
    void unregisterCollider(const std::shared_ptr<Collider> &collider);
};


#endif //SPACEINVADERS_PHYSICSENGINE_HPP
