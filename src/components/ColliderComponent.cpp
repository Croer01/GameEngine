//
// Created by adria on 31/10/2018.
//

#include "ColliderComponent.hpp"
#include "../physics/PhysicsEngine.hpp"

namespace {
    Collider::ColliderShapes stringToColliderShape(const std::string &shapeName) {
        if(shapeName == "Box" || shapeName == "box")
            return Collider::ColliderShapes::Box;

        throw std::runtime_error("Unknown " + shapeName + " collider type.");
    }

    Collider::ColliderTypes stringToColliderType(const std::string &typeName) {
        if(typeName == "Static" || typeName == "static")
            return Collider::ColliderTypes::Static;
        else if(typeName == "Dynamic" || typeName == "dynamic")
            return Collider::ColliderTypes::Dynamic;
        else if(typeName == "Kinematic" || typeName == "kinematic")
            return Collider::ColliderTypes::Kinameatic;

        throw std::runtime_error("Unknown " + typeName + " collider type.");
    }
}
std::shared_ptr<Component> ColliderComponent::Clone() {
    std::shared_ptr<ColliderComponent> clone = std::make_shared<ColliderComponent>();
    clone->collider_ = collider_->clone();
    clone->collider_->setComponent(std::shared_ptr<ColliderComponent>(this));
    PhysicsEngine::GetInstance().registerCollider(clone->collider_);
    return clone;
}

void ColliderComponent::fromFile(const YAML::Node &componentConfig) {
    std::shared_ptr<Collider> collider = std::make_shared<Collider>();
    collider->setShape(stringToColliderShape(componentConfig["colliderShape"].as<std::string>("Box")));
    collider->setType(stringToColliderType(componentConfig["colliderType"].as<std::string>("Static")));
    collider_ = collider;
}

void ColliderComponent::init() {
    collider_->setPosition(parent_->getPosition().x, parent_->getPosition().y);
    collider_->registerObserver(this);
    parent_->registerObserver(this);
}

void ColliderComponent::Update(float elapsedTime) {
    if(collider_->getType() == Collider::ColliderTypes::Dynamic){
        printf("%4.2f %4.2f\n", collider_->getPosition().x, collider_->getPosition().y);
        parent_->setPosition(collider_->getPosition());
    }
//    collider_->debug();
}

void ColliderComponent::setOnColliderEnter(const OnColliderEventCallback &callback) {
    onColliderEnterCallback_ = callback;
}

void ColliderComponent::onEvent(const Subject<ColliderEvent> &target, const ColliderEvent &event, void *args) {
    if(event == ColliderEvent::BeginCollider && onColliderEnterCallback_) {
        Collider *collider = static_cast<Collider *>(args);
        onColliderEnterCallback_(collider->getComponent());
    }
}

void ColliderComponent::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
    if(event == GameObjectEvent::PositionChanged){
        if(!parent_->isActive() || collider_->getType() != Collider::ColliderTypes::Dynamic)
            collider_->setPosition(parent_->getPosition().x, parent_->getPosition().y);
    }
    else if(event == GameObjectEvent::ScaleChanged){
        collider_->setSize(parent_->getScale().x/2.f, parent_->getScale().y/2.f);
    }
    else if(event == GameObjectEvent::ActiveChanged){
        collider_->setActive(parent_->isActive());
    }
}

ColliderComponent::~ColliderComponent() {
    if(collider_)
        PhysicsEngine::GetInstance().unregisterCollider(collider_);
}

