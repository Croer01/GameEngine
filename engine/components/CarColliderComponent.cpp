//
// Created by adria on 27/12/2020.
//

#include <game-engine/components/CarColliderComponent.hpp>
#include <game-engine/Game.hpp>

namespace GameEngine
{

CarColliderComponent::~CarColliderComponent()
{
    if(physicsEngine_ != nullptr && carCollider_)
        physicsEngine_->unregisterCollider(carCollider_);

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

    // create the car collider
    carCollider_ = std::make_shared<Internal::Collider>();
    carCollider_->setShape(Internal::Collider::ColliderShapes::Box);
    carCollider_->setType(Internal::Collider::ColliderTypes::Dynamic);
    carCollider_->setSensor(false);
    carCollider_->setGravityScale(0);
    carCollider_->setMass(1.f);
    carCollider_->setFixedRotation(false);

    physicsEngine_->registerCollider(carCollider_);
    carCollider_->setSize(carSize_.x / 2.f, carSize_.y / 2.f);
    carCollider_->setPosition(gameObject()->position());
    carCollider_->setRotation(gameObject()->rotation());
    carCollider_->setActive(true);


    // create the tires colliders
    for(int i = 0; i < 4; i++)
    {
        // configure tire
        auto tireCollider = std::make_shared<Internal::Collider>();
        tireCollider->setShape(Internal::Collider::ColliderShapes::Box);
        tireCollider->setType(Internal::Collider::ColliderTypes::Dynamic);
        tireCollider->setSensor(false);
        tireCollider->setGravityScale(0);
        tireCollider->setMass(1.f);
        tireCollider->setFixedRotation(false);

        physicsEngine_->registerCollider(tireCollider);
        tireCollider->setSize(tiresSize_.x / 2.f, tiresSize_.y / 2.f);
        tireCollider->setPosition(gameObject()->position());
        tireCollider->setRotation(gameObject()->rotation());
        tireCollider->setActive(true);


        float x = 0;
        float y = 0;

        if(i == 0)
        {
            x = -tiresOffset_.x;
            y = tiresOffset_.y;
        }
        else if(i == 1)
        {
            x = tiresOffset_.x;
            y = tiresOffset_.y;
        }
        else if(i == 2)
        {
            x = -tiresOffset_.x;
            y = -tiresOffset_.y;
        }
        else if(i == 3)
        {
            x = tiresOffset_.x;
            y = -tiresOffset_.y;
        }

        // configure tire joint
        b2RevoluteJoint *joint = (b2RevoluteJoint *) physicsEngine_->createRevoluteJoin(carCollider_->getBody(), tireCollider->getBody(), x, y);

        if(i == 0)
        {
            frontLeftJoint_ = joint;
        }
        else if(i == 1)
        {
            frontRightJoint_ = joint;
        }

        tires_.push_back(tireCollider);
    }

    gameObject()->registerObserver(this);
}

void CarColliderComponent::updateTireDragAndPhysics(b2Body *body)
{
    // Get lateral velocity
    b2Vec2 currentRightNormal = body->GetWorldVector(b2Vec2(1, 0));
    b2Vec2 lateralVelocity = b2Dot(currentRightNormal, body->GetLinearVelocity()) * currentRightNormal;

    // Get forward velocity
    b2Vec2 currentForwardNormal = body->GetWorldVector(b2Vec2(0, 1));
    b2Vec2 forwardVelocity = b2Dot(currentForwardNormal, body->GetLinearVelocity()) * currentForwardNormal;

    // update friction
    constexpr float maxLateralImpulse = 1.5f;
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

void CarColliderComponent::Update(float elapsedTime)
{
    InputManager *input = gameObject()->game()->input();

    // Positive is forward, negative is backward
    int direction = 0;
    if (input->isKeyPressed(KeyCode::KEY_UP))
    {
        direction = 1;
    }
    else if (input->isKeyPressed(KeyCode::KEY_DOWN))
    {
        direction = -1;
    }

    for (auto collider : tires_)
    {
        b2Body *body = collider->getBody();

        updateTireDragAndPhysics(body);
        applyForceToTire(body, direction);
    }

    float lockAngle = 40 * (3.1416f / 180.f);
    float turnSpeedPerSec = 320 * (3.1416f / 180.f);//from lock to lock in 0.25 sec
    float turnPerTimeStep = turnSpeedPerSec / 60.0f;
    float desiredAngle = 0;
    if (input->isKeyPressed(KeyCode::KEY_LEFT))
    {
        desiredAngle = -lockAngle;
    }
    else if (input->isKeyPressed(KeyCode::KEY_RIGHT))
    {
        desiredAngle = lockAngle;
    }
    float angleNow = frontLeftJoint_->GetJointAngle();
    float angleToTurn = desiredAngle - angleNow;
    angleToTurn = b2Clamp( angleToTurn, -turnPerTimeStep, turnPerTimeStep );
    float newAngle = angleNow + angleToTurn;
    frontLeftJoint_->SetLimits( newAngle, newAngle );
    frontRightJoint_->SetLimits( newAngle, newAngle );

    // Update GameObject
    Vec2D position = carCollider_->getPosition();
    float rotation = carCollider_->getRotation();
    if(auto parent = gameObject()->parent().lock())
    {
        position -= parent->position();
        rotation -= parent->rotation();
    }
    gameObject()->position(position);
    gameObject()->rotation(rotation);

}

void CarColliderComponent::applyForceToTire(b2Body *body, int direction) const
{
    float desiredSpeed = 0;
    if (direction > 0)
    {
        desiredSpeed = maxForwardSpeed_;
    }
    else if (direction < 0)
    {
        desiredSpeed = maxBackwardSpeed_;
    }

    // Get forward velocity
    b2Vec2 currentForwardNormal = body->GetWorldVector(b2Vec2(0, 1));
    b2Vec2 forwardVelocity = b2Dot(currentForwardNormal, body->GetLinearVelocity()) * currentForwardNormal;

    //find current speed in forward direction
    float currentSpeed = b2Dot(forwardVelocity, currentForwardNormal);

    //apply necessary force
    float force = 0;
    if (desiredSpeed > currentSpeed)
        force = maxDriveForce_;
    else if (desiredSpeed < currentSpeed)
        force = -maxDriveForce_;

    if(abs(force) > 1e-6)
        body->ApplyForce(force * currentForwardNormal, body->GetWorldCenter(), true);
}

PropertySetBase *CarColliderComponent::getProperties() const
{
    auto properties = new PropertySet<CarColliderComponent>();

    properties->add(new Property<CarColliderComponent, Vec2D>(
        "carSize",
        &CarColliderComponent::getCarSize,
        &CarColliderComponent::setCarSize,
        Vec2D(1.f,1.f)));

    properties->add(new Property<CarColliderComponent, Vec2D>(
        "tiresOffset",
        &CarColliderComponent::getTiresOffset,
        &CarColliderComponent::setTiresOffset,
        Vec2D(1.f,1.f)));

    properties->add(new Property<CarColliderComponent, Vec2D>(
        "tiresSize",
        &CarColliderComponent::getTiresSize,
        &CarColliderComponent::setTiresSize,
        Vec2D(1.f,1.f)));

    properties->add(new Property<CarColliderComponent, float>(
        "maxForwardSpeed",
        &CarColliderComponent::getMaxForwardSpeed,
        &CarColliderComponent::setMaxForwardSpeed,
        200.f));

    properties->add(new Property<CarColliderComponent, float>(
        "maxBackwardSpeed",
        &CarColliderComponent::getMaxBackwardSpeed,
        &CarColliderComponent::setMaxBackwardSpeed,
        -40.f));

    properties->add(new Property<CarColliderComponent, float>(
        "maxDriveForce",
        &CarColliderComponent::getMaxDriveForce,
        &CarColliderComponent::setMaxDriveForce,
        500.f));

    return properties;
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

float CarColliderComponent::getMaxForwardSpeed() const
{
    return maxForwardSpeed_;
}

void CarColliderComponent::setMaxForwardSpeed(const float &maxForwardSpeed)
{
    maxForwardSpeed_ = maxForwardSpeed;
}

float CarColliderComponent::getMaxBackwardSpeed() const
{
    return maxBackwardSpeed_;
}

void CarColliderComponent::setMaxBackwardSpeed(const float &maxBackwardSpeed)
{
    maxBackwardSpeed_ = maxBackwardSpeed;
}

float CarColliderComponent::getMaxDriveForce() const
{
    return maxDriveForce_;
}

void CarColliderComponent::setMaxDriveForce(const float &maxDriveForce)
{
    maxDriveForce_ = maxDriveForce;
}

Vec2D CarColliderComponent::getCarSize() const
{
    return carSize_;
}

void CarColliderComponent::setCarSize(const Vec2D &carSize)
{
    carSize_ = carSize;
}

Vec2D CarColliderComponent::getTiresOffset() const
{
    return tiresOffset_;
}

void CarColliderComponent::setTiresOffset(const Vec2D &tiresOffset)
{
    tiresOffset_ = tiresOffset;
}

Vec2D CarColliderComponent::getTiresSize() const
{
    return tiresSize_;
}

void CarColliderComponent::setTiresSize(const Vec2D &tiresSize)
{
    tiresSize_ = tiresSize;
}

}