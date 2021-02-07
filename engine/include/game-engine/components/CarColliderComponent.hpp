//
// Created by adria on 27/12/2020.
//

#ifndef GAMEENGINE_CARCOLLIDERCOMPONENT_HPP
#define GAMEENGINE_CARCOLLIDERCOMPONENT_HPP

#include <game-engine/geComponent.hpp>
#include <game-engine/InputManager.hpp>
#include <game-engine/internal/physics/Collider.hpp>
#include <game-engine/internal/physics/PhysicsEngine.hpp>

namespace GameEngine
{
class CarColliderComponent : public geComponentInstantiable<CarColliderComponent>, public Observer<GameObjectEvent>
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
    virtual PropertySetBase *getProperties() const;
    void onEvent(const Subject<GameObjectEvent> &target, GameObjectEvent event) override;

    float getMaxForwardSpeed() const;
    void setMaxForwardSpeed(const float &maxForwardSpeed);
    float getMaxBackwardSpeed() const;
    void setMaxBackwardSpeed(const float &maxBackwardSpeed);
    float getMaxDriveForce() const;
    void setMaxDriveForce(const float &maxDriveForce);
    void applyForceToTire(b2Body *body, int direction) const;
    Vec2D getCarSize() const;
    void setCarSize(const Vec2D &carSize);
    Vec2D getTiresOffset() const;
    void setTiresOffset(const Vec2D &tiresOffset);
    Vec2D getTiresSize() const;
    void setTiresSize(const Vec2D &tiresSize);
};
}

#endif //GAMEENGINE_CARCOLLIDERCOMPONENT_HPP
