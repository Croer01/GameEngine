//
// Created by adria on 31/10/2018.
//

#include <string>
#include <iostream>
#include "Collider.hpp"
#include "PhysicsEngine.hpp"

namespace {
    void dummyDeleterBody(b2Body *){
        //dummy deleter
    };

    void dummyDeleterShape(b2PolygonShape *){
        //dummy deleter
    };
}

b2BodyDef *Collider::getBodyDef() const {
    b2BodyDef *bodyDef = new b2BodyDef();
    switch (colliderType_){
        case ColliderTypes::Static:
            bodyDef->type = b2_staticBody;
            break;
        case ColliderTypes::Dynamic:
            bodyDef->type = b2_dynamicBody;
            break;
        case ColliderTypes::Kinameatic:
            bodyDef->type = b2_kinematicBody;
            break;
    }
    return bodyDef;
}

void Collider::setBody(b2Body *body) {
    body_ = std::unique_ptr<b2Body, decltype(&dummyDeleterBody)>(body, &dummyDeleterBody);
    body_->SetUserData(this);

    switch (colliderShape_) {
        case ColliderShapes::Box:
            shape_ = std::unique_ptr<b2PolygonShape, decltype(&dummyDeleterShape)>(new b2PolygonShape(), &dummyDeleterShape);
            shape_->SetAsBox(1.0f, 1.0f);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = shape_.get();
            fixtureDef.density = 1.f;
            fixtureDef.friction = 0.3f;

            body_->CreateFixture(&fixtureDef);
            break;
    }
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

void Collider::debug() {
    b2Vec2 position = body_->GetPosition();
    float32 angle = body_->GetAngle();
    printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
}

std::shared_ptr<Collider> Collider::clone() {
    std::shared_ptr<Collider> clone = std::make_shared<Collider>();
    clone->colliderShape_ = colliderShape_;
    clone->colliderType_ = colliderType_;

    return clone;
}

void Collider::setPosition(float x, float y) {
    body_->SetTransform(b2Vec2(x,y), body_->GetAngle());
}

glm::vec3 Collider::getPosition() const {
    b2Vec2 position = body_->GetPosition();

    return glm::vec3(position.x,position.y,0);
}

void Collider::setSize(float extendX, float extendY) {
    shape_->SetAsBox(extendX,extendY);
}

void Collider::doBeginCollistion(Collider *other) {
    notify(ColliderEvent::BeginCollider, (void *)other);
}

void Collider::setComponent(const std::shared_ptr<ColliderComponent> &component) {
    component_ = component;
}

std::shared_ptr<ColliderComponent> Collider::getComponent() {
    return component_;
}

void Collider::setActive(bool active) {
    body_->SetActive(active);
}

b2Body *Collider::getBody() {
    return body_.get();
}
