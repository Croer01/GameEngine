//
// Created by adria on 27/12/2020.
//

#ifndef GAMEENGINE_CARCOLLIDERCOMPONENT_HPP
#define GAMEENGINE_CARCOLLIDERCOMPONENT_HPP

#include <game-engine/api.hpp>
#include <game-engine/geComponent.hpp>
#include <game-engine/InputManager.hpp>
#include <game-engine/internal/physics/Collider.hpp>
#include <game-engine/internal/physics/PhysicsEngine.hpp>
#include <game-engine/components/ComponentData.hpp>

namespace GameEngine
{

class PUBLICAPI CarColliderComponentData : public ComponentData
{
public:
    CarColliderComponentData()
    {
        createProperty<Vec2D>("carSize",Vec2D(1.f, 1.f));
        createProperty<Vec2D>("tireOffset",Vec2D(1.f, 1.f));
        createProperty<Vec2D>("tireSize",Vec2D(1.f, 1.f));
        createProperty<float>("maxForwardSpeed", 200.f);
        createProperty<float>("maxBackwardSpeed", -40.f);
        createProperty<float>("maxDriveForce", 500.f);
    }
};

class PUBLICAPI CarColliderComponent : public geComponentInstantiable<CarColliderComponent, CarColliderComponentData>, public Observer<GameObjectEvent>
{
    Internal::PhysicsEngine *physicsEngine_;
    std::vector<std::shared_ptr<Internal::Collider>> tires_;
    std::shared_ptr<Internal::Collider> carCollider_;
    b2RevoluteJoint *frontLeftJoint_;
    b2RevoluteJoint *frontRightJoint_;

    GameEngine::Vec2D carSize_;
private:
    GameEngine::Vec2D tiresOffset_;
    GameEngine::Vec2D tiresSize_;
    float maxForwardSpeed_;
    float maxBackwardSpeed_;
    float maxDriveForce_;

    void updateTireDragAndPhysics(b2Body *body);
public:
    virtual ~CarColliderComponent();
    virtual void init();
    virtual void Update(float elapsedTime);
    void onEvent(const Subject<GameObjectEvent> &target, GameObjectEvent event) override;

    void applyForceToTire(b2Body *body, int direction) const;
};
}

#endif //GAMEENGINE_CARCOLLIDERCOMPONENT_HPP
