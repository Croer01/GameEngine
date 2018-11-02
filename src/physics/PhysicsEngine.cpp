//
// Created by adria on 31/10/2018.
//

#include "PhysicsEngine.hpp"

void PhysicsEngine::init(float timeStep) {
    timeStep_ = timeStep;
    timeAcumulator_ = 0;
    velocityIterations_ = 6;
    positionIterations_ = 2;
    colliders_.clear();

    world_ = std::make_unique<b2World>(b2Vec2(0,9.8f));
    world_->SetContactListener(this);
#ifdef DEBUG
    debugView_ = std::make_unique<DebugView>();
    world_->SetDebugDraw(debugView_.get());
    debugView_->SetFlags(b2Draw::e_shapeBit);
#endif
}

void PhysicsEngine::update(float elapsedTime) {
    timeAcumulator_ +=elapsedTime;
    if(timeAcumulator_>= timeStep_) {
        world_->Step(timeStep_, velocityIterations_, positionIterations_);
        timeAcumulator_ = 0;
    }
    //update all the colliders in safe mode
    for (const std::shared_ptr<Collider> &collider : colliders_){
        collider->updateInSafeMode();
    }
}

void PhysicsEngine::registerCollider(const std::shared_ptr<Collider> &collider) {
    auto it = std::find(colliders_.begin(),colliders_.end(),collider);
    if(it != colliders_.end())
        return;

    colliders_.push_back(collider);
    b2Body* body = world_->CreateBody(collider->getBodyDef());
    collider->setBody(body);
}

void PhysicsEngine::unregisterCollider(const std::shared_ptr<Collider> &collider) {
    auto it = std::find(colliders_.begin(),colliders_.end(),collider);
    if(it == colliders_.end())
        return;

    std::swap(*it, *(--colliders_.end()));
    colliders_.pop_back();
    world_->DestroyBody(collider->getBody());
}

void PhysicsEngine::BeginContact(b2Contact *contact) {
    Collider * colliderA = nullptr;
    Collider * colliderB = nullptr;

    void *userData = contact->GetFixtureA()->GetBody()->GetUserData();
    if(userData)
        colliderA = static_cast<Collider*>(userData);

    userData = contact->GetFixtureB()->GetBody()->GetUserData();
    if(userData)
        colliderB = static_cast<Collider*>(userData);

    if(colliderA && colliderB){
        colliderA->doBeginCollistion(colliderB);
        colliderB->doBeginCollistion(colliderA);
    }
}

void PhysicsEngine::drawDebug() {
    world_->DrawDebugData();
}
