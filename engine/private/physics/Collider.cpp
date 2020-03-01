//
// Created by adria on 31/10/2018.
//

#include <string>
#include <iostream>
#include "Collider.hpp"
#include "PhysicsEngine.hpp"
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
        bodyDef->fixedRotation = true;
        // this is to avoid sensor colliders won't detect collisions if they are statics or they don't move
        if(isSensor_)
            bodyDef->allowSleep = false;

        switch (colliderType_) {
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
        return colliderType_;
    }

    std::shared_ptr<Collider> Collider::clone() {
        std::shared_ptr<Collider> clone = std::make_shared<Collider>();
        clone->colliderShape_ = colliderShape_;
        clone->colliderType_ = colliderType_;
        clone->category_ = category_;

        return clone;
    }

    void Collider::setPosition(const Vec2D &pos) {
        propertiesToSetInSafeMode_.hasPosition = true;
        propertiesToSetInSafeMode_.position = b2Vec2(pos.x / PhysicsEngine::getScalePixelsToMeter(),
                                                     pos.y / PhysicsEngine::getScalePixelsToMeter());
    }

    Vec2D Collider::getPosition() const {
        b2Vec2 position = body_->GetPosition();

        if (propertiesToSetInSafeMode_.hasPosition)
            position = propertiesToSetInSafeMode_.position;

        return Vec2D(position.x * PhysicsEngine::getScalePixelsToMeter(),
                         position.y * PhysicsEngine::getScalePixelsToMeter());
    }

    void Collider::setSize(float extendX, float extendY) {
        addShapeToBody(extendX / PhysicsEngine::getScalePixelsToMeter(),
                       extendY / PhysicsEngine::getScalePixelsToMeter());
    }

    void Collider::doBeginCollistion(Collider *other) {
        notify(ColliderEvent::BeginCollider, (void *) other);
    }

    void Collider::doBeginSensor(Collider *other) {
        notify(ColliderEvent::BeginSensor, (void *) other);
    }
    void Collider::setComponent(const std::shared_ptr<ColliderComponent> &component) {
        component_ = component;
    }

    std::weak_ptr<ColliderComponent> Collider::getComponent() {
        return component_;
    }

    void Collider::setActive(bool active) {
        propertiesToSetInSafeMode_.hasActive = true;
        propertiesToSetInSafeMode_.active = reinterpret_cast<bool *>(active);
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

        //create the fixture with the new shapes
        switch (colliderShape_) {
            case ColliderShapes::Box:
                b2PolygonShape *shape = new b2PolygonShape();
                shape->SetAsBox(extendX, extendY);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = shape;
                fixtureDef.density = 1.f;
                fixtureDef.friction = 0.0f;
                fixtureDef.filter = filter_;
                fixtureDef.isSensor = isSensor_;
                body_->CreateFixture(&fixtureDef);
                break;
        }
    }

    void Collider::updateInSafeMode() {
        if (propertiesToSetInSafeMode_.hasPosition) {
            body_->SetTransform(propertiesToSetInSafeMode_.position, body_->GetAngle());
            propertiesToSetInSafeMode_.hasPosition = false;
        }

        if (propertiesToSetInSafeMode_.hasActive) {
            body_->SetActive(propertiesToSetInSafeMode_.active);
            propertiesToSetInSafeMode_.hasActive = false;
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
}
}