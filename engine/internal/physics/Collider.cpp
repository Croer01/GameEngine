//
// Created by adria on 31/10/2018.
//

#include <string>
#include <iostream>
#include <game-engine/internal/physics/Collider.hpp>
#include <game-engine/internal/physics/PhysicsEngine.hpp>

namespace GameEngine {
namespace Internal {
    namespace {
        void dummyDeleterBody(b2Body *) {
            //dummy deleter
        };
    }

    b2BodyDef *Collider::getBodyDef() const {
        b2BodyDef *bodyDef = new b2BodyDef();
        bodyDef->userData = (void *) this;
        bodyDef->fixedRotation = fixedRotation_.get();
        bodyDef->gravityScale = gravityScale_;
        // this is to avoid sensor colliders won't detect collisions if they are statics or they don't move
        if(isSensor_)
            bodyDef->allowSleep = false;

        switch (colliderType_.get()) {
            case ColliderTypes::Static:
                bodyDef->type = b2_staticBody;
                break;
            case ColliderTypes::Dynamic:
                bodyDef->type = b2_dynamicBody;
                break;
            case ColliderTypes::Kinematic:
                bodyDef->type = b2_kinematicBody;
                break;
        }
        return bodyDef;
    }

    void Collider::setBody(b2Body *body) {
        body_ = std::unique_ptr<b2Body, decltype(&dummyDeleterBody)>(body, &dummyDeleterBody);
        addShapeToBody(1, 1);
    }


    void Collider::setBody(b2Body *body, const b2Filter &filter) {
        filter_ = filter;
        setBody(body);
    }

    void Collider::setShape(Collider::ColliderShapes shape) {
        colliderShape_ = shape;
    }

    void Collider::setType(Collider::ColliderTypes type) {
        colliderType_ = type;
    }

    Collider::ColliderTypes Collider::getType() {
        return colliderType_.get();
    }

    void Collider::setPosition(const Vec2D &pos) {
        position_ = b2Vec2(pos.x / PhysicsEngine::getScalePixelsToMeter(),
                                                     pos.y / PhysicsEngine::getScalePixelsToMeter());
    }

    Vec2D Collider::getPosition() const {
        b2Vec2 position = body_->GetPosition();

        if (position_.hasChanges())
            position = position_.get();

        return Vec2D(position.x * PhysicsEngine::getScalePixelsToMeter(),
                         position.y * PhysicsEngine::getScalePixelsToMeter());
    }

    void Collider::setSize(float extendX, float extendY) {
        addShapeToBody(extendX / PhysicsEngine::getScalePixelsToMeter(),
                       extendY / PhysicsEngine::getScalePixelsToMeter());
    }

    void Collider::setRotation(float radians)
    {
        rotation_ = radians;
    }

    float Collider::getRotation() const
    {
        float rotation = body_->GetAngle();
        if(rotation_.hasChanges())
            rotation = rotation_.get();
        return rotation;
    }

void Collider::doBeginCollision(Collider *other) {
        notify(ColliderEvent::BeginCollider, other);
    }

    void Collider::doBeginSensor(Collider *other) {
        notify(ColliderEvent::BeginSensor, other);
    }

    void Collider::doEndCollision(Collider *other) {
        notify(ColliderEvent::EndCollider, other);
    }

    void Collider::doEndSensor(Collider *other) {
        notify(ColliderEvent::EndSensor, other);
    }

    void Collider::setComponent(const std::shared_ptr<ColliderComponent> &component) {
        component_ = component;
    }

    std::weak_ptr<ColliderComponent> Collider::getComponent() {
        return component_;
    }

    void Collider::setActive(bool active) {
        active_ = active;
    }

    b2Body *Collider::getBody() {
        return body_.get();
    }

    void Collider::addShapeToBody(float extendX, float extendY) {
        //first clear all possible fixtures
        b2Fixture *fixture = body_->GetFixtureList();
        while (fixture) {
            b2Fixture *fixtureToDestroy = fixture;
            fixture = fixture->GetNext();
            body_->DestroyFixture(fixtureToDestroy);
        }

        b2FixtureDef fixtureDef;
        //create the fixture with the new shapes
        switch (colliderShape_) {
            case ColliderShapes::Box:
            {
                auto *shape = new b2PolygonShape();
                shape->SetAsBox(extendX, extendY);
                fixtureDef.shape = shape;
            }
                break;
            case ColliderShapes::Circle:
            {
                auto *circle = new b2CircleShape();
                circle->m_radius = extendX > extendY ? extendX : extendY;
                fixtureDef.shape = circle;
            }
                break;
        }

        fixtureDef.density = 1.f;
        fixtureDef.friction = 0.0f;
        fixtureDef.filter = filter_;
        fixtureDef.isSensor = isSensor_;
        body_->CreateFixture(&fixtureDef);
    }

void Collider::updateInSafeMode() {
        if (position_.update())
            body_->SetTransform(position_.get(), body_->GetAngle());

        if (rotation_.update())
            body_->SetTransform(body_->GetPosition(), rotation_.get());

        if (active_.update())
            body_->SetActive(active_.get());

        if (colliderType_.update()) {
            if(body_)
            {
                switch (colliderType_.get()) {
                    case ColliderTypes::Static:
                        body_->SetType(b2_staticBody);
                        break;
                    case ColliderTypes::Dynamic:
                        body_->SetType(b2_dynamicBody);
                        break;
                    case ColliderTypes::Kinematic:
                        body_->SetType(b2_kinematicBody);
                        break;
                }
                // awake body to ensure the engine calculates physics related to the collider
                body_->SetAwake(true);
            }
        }

        if(mass_.update())
        {
            b2MassData massData;
            body_->GetMassData(&massData);
            massData.mass = mass_.get();
            body_->SetMassData(&massData);
        }

        if(fixedRotation_.update())
        {
            body_->SetFixedRotation(fixedRotation_.get());
        }
    }

    std::string Collider::getCategory() const {
        return category_;
    }

    void Collider::setCategory(const std::string &category) {
        category_ = category;
    }

void Collider::setSensor(bool isSensor)
{
    isSensor_ = isSensor;
}

bool Collider::IsSensor() const
{
    return isSensor_;
}

void Collider::setGravityScale(float scale)
{
    gravityScale_ = scale;
}

float Collider::getGravityScale() const
{
    return gravityScale_;
}

void Collider::setMass(float value)
{
    mass_ = value;
}

float Collider::getMass() const
{
    return mass_.hasChanges()? mass_.get() : body_->GetMass();
}

void Collider::setFixedRotation(bool value)
{
    fixedRotation_ = value;
}

bool Collider::getFixedRotation() const
{
    return fixedRotation_.get();
}
}
}