//
// Created by adria on 27/12/2020.
//

#include "CarColliderComponent.hpp"
#include "game-engine/Game.hpp"

namespace GameEngine
{

CarColliderComponent::~CarColliderComponent()
{
    for(auto &tire : tires_)
    {
        if(physicsEngine_ != nullptr && tire) {
            physicsEngine_->unregisterCollider(tire);
        }
    }
}

void CarColliderComponent::init()
{
    physicsEngine_ = gameObject()->game()->physicsEngine();
    for(int i = 0; i < 1; i++)
    {
        auto tireCollider = std::shared_ptr<Internal::Collider>();
        tireCollider = std::make_shared<Internal::Collider>();
        tireCollider->setShape(Internal::Collider::ColliderShapes::Box);
        tireCollider->setType(Internal::Collider::ColliderTypes::Dynamic);
        tireCollider->setSensor(false);
        tireCollider->setGravityScale(0);
        tireCollider->setMass(100.f);
        tireCollider->setFixedRotation(false);
//        tireCollider->setComponent(std::dynamic_pointer_cast<ColliderComponent>(shared_from_this()));

        physicsEngine_->registerCollider(tireCollider);
        tireCollider->setSize(50.f, 125.f);
//        tireCollider->setPosition(convertWorldToPhysicsPos(gameObject()->position()));
        tireCollider->setPosition(gameObject()->position());
        tireCollider->setRotation(gameObject()->rotation());
        tireCollider->setActive(true);

        tires_.push_back(tireCollider);
    }

    gameObject()->registerObserver(this);
}

void CarColliderComponent::Update(float elapsedTime)
{
    // this is temporal code for test
    auto collider = tires_[0];





    b2Body *body = collider->getBody();

    // CONTROLS
    {
        constexpr float maxForwardSpeed = 200;
        constexpr float maxBackwardSpeed = -40;
        constexpr float maxDriveForce = 500;

        InputManager *input = gameObject()->game()->input();

        float desiredSpeed = 0;
        if (input->isKeyPressed(KeyCode::KEY_UP))
        {
            desiredSpeed = maxForwardSpeed;
        } else if (input->isKeyPressed(KeyCode::KEY_DOWN))
        {
            desiredSpeed = maxBackwardSpeed;
        }

        // Get forward velocity
        b2Vec2 currentForwardNormal = body->GetWorldVector(b2Vec2(0, 1));
        b2Vec2 forwardVelocity = b2Dot(currentForwardNormal, body->GetLinearVelocity()) * currentForwardNormal;

        //find current speed in forward direction
        float currentSpeed = b2Dot(forwardVelocity, currentForwardNormal);

        //apply necessary force
        float force = 0;
        if (desiredSpeed > currentSpeed)
            force = maxDriveForce;
        else if (desiredSpeed < currentSpeed)
            force = -maxDriveForce;

        if(std::abs(force) > 1e-6)
            body->ApplyForce(force * currentForwardNormal, body->GetWorldCenter(), true);

        float desiredTorque = 0;
        if (input->isKeyPressed(KeyCode::KEY_LEFT))
        {
            desiredTorque = -1500;
        } else if (input->isKeyPressed(KeyCode::KEY_RIGHT))
        {
            desiredTorque = 1500;
        }
        if(std::abs(desiredTorque) > 1e-6)
            body->ApplyTorque( desiredTorque, true);
    }

    // UPDATE DRAG AND FRICTION PHYSICS
    {
        // Get lateral velocity
        b2Vec2 currentRightNormal = body->GetWorldVector(b2Vec2(1, 0));
        b2Vec2 lateralVelocity = b2Dot(currentRightNormal, body->GetLinearVelocity()) * currentRightNormal;

        // Get forward velocity
        b2Vec2 currentForwardNormal = body->GetWorldVector(b2Vec2(0, 1));
        b2Vec2 forwardVelocity = b2Dot(currentForwardNormal, body->GetLinearVelocity()) * currentForwardNormal;

        // update friction
        constexpr float maxLateralImpulse = 3.f;
        b2Vec2 impulse = body->GetMass() * -lateralVelocity;
        if ( impulse.Length() > maxLateralImpulse )
            impulse *= maxLateralImpulse / impulse.Length();
        body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);
        body->ApplyAngularImpulse(0.1f * body->GetInertia() * -body->GetAngularVelocity(), true);

        // apply drag force
        float currentForwardSpeed = forwardVelocity.Normalize();
        float dragForceMagnitude = -2 * currentForwardSpeed;
        body->ApplyForce(dragForceMagnitude * forwardVelocity, body->GetWorldCenter(), true);
    }

    // Update GameObject
    Vec2D position = collider->getPosition();
    float rotation = collider->getRotation();
    if(auto parent = gameObject()->parent().lock())
    {
        position -= parent->position();
        rotation -= parent->rotation();
    }
    gameObject()->position(position);
    gameObject()->rotation(rotation);

}

PropertySetBase *CarColliderComponent::getProperties() const
{
    return new PropertySet<CarColliderComponent>();
}

void CarColliderComponent::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args)
{
    // this is temporal code for test
    auto collider = tires_[0];

    if(event == GameObjectEvent::PositionChanged){
//        collider->setPosition(convertWorldToPhysicsPos(gameObject()->position()));
    }
    else if(event == GameObjectEvent::RotationChanged){
        collider->setRotation(gameObject()->rotation());
    }
    else if(event == GameObjectEvent::ScaleChanged){
//        updateColliderSize();
    }
    else if(event == GameObjectEvent::ActiveChanged){
        collider->setActive(gameObject()->active());
    }
}

}