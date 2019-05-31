//
// Created by adria on 31/10/2018.
//

#include "PhysicsEngine.hpp"
namespace GameEngine {
namespace Internal {
    void PhysicsEngine::init(float timeStep) {
        timeStep_ = timeStep;
        timeAcumulator_ = 0;
        velocityIterations_ = 6;
        positionIterations_ = 2;
        colliders_.clear();

        world_.reset(new b2World(b2Vec2(0, 9.8f)));
        world_->SetContactListener(this);
#ifdef DEBUG
        debugView_.reset(new DebugView());
        world_->SetDebugDraw(debugView_.get());
        debugView_->SetFlags(b2Draw::e_shapeBit);
#endif
    }

    void PhysicsEngine::update(float elapsedTime) {
        //update all the colliders in safe mode
        for (const std::shared_ptr<Collider> &collider : colliders_) {
            collider->updateInSafeMode();
        }

        timeAcumulator_ += elapsedTime;
        if (timeAcumulator_ >= timeStep_) {
            world_->Step(timeStep_, velocityIterations_, positionIterations_);
            timeAcumulator_ = 0;
        }
    }

    void PhysicsEngine::registerCollider(const std::shared_ptr<Collider> &collider) {
        auto it = std::find(colliders_.begin(), colliders_.end(), collider);
        if (it != colliders_.end())
            return;

        colliders_.push_back(collider);
        b2Body *body = world_->CreateBody(collider->getBodyDef());

        // Create filter if collider has some category assigned
        const std::string &colliderCategory = collider->getCategory();
        if (colliderCategory.empty()) {
            collider->setBody(body);
        } else {
            b2Filter filter;
            filter.categoryBits = categories_[colliderCategory].categoryBit;
            filter.maskBits = categories_[colliderCategory].maskBits;
            collider->setBody(body, filter);
        }
    }

    void PhysicsEngine::unregisterCollider(const std::shared_ptr<Collider> &collider) {
        auto it = std::find(colliders_.begin(), colliders_.end(), collider);
        if (it == colliders_.end())
            return;

        std::swap(*it, *(--colliders_.end()));
        colliders_.pop_back();
        world_->DestroyBody(collider->getBody());
    }

    void PhysicsEngine::BeginContact(b2Contact *contact) {
        Collider *colliderA = nullptr;
        Collider *colliderB = nullptr;

        void *userData = contact->GetFixtureA()->GetBody()->GetUserData();
        if (userData)
            colliderA = static_cast<Collider *>(userData);

        userData = contact->GetFixtureB()->GetBody()->GetUserData();
        if (userData)
            colliderB = static_cast<Collider *>(userData);

        if (colliderA && colliderB) {
            colliderA->doBeginCollistion(colliderB);
            colliderB->doBeginCollistion(colliderA);
        }
    }

    void PhysicsEngine::drawDebug(const std::shared_ptr<Camera> &cam) {
        debugView_->setCamera(cam);
        world_->DrawDebugData();
    }

    void PhysicsEngine::createCategories(const std::vector<std::string> &categories) {
        //start the bit shifted from 1 position to avoid use the default mask as a category
        int bitsToShift = 1;
        for (const std::string &categoryName : categories) {
            ColliderCategory category;
            category.categoryBit = static_cast<uint16>(1 << bitsToShift);
            categories_.insert(std::pair<std::string, ColliderCategory>(categoryName, category));
            bitsToShift++;
        }
    }

    void PhysicsEngine::createMasks(const std::unordered_map<std::string, std::vector<std::string>> &masks) {
        for (const auto &it : masks) {
            categories_[it.first].maskBits = 0;
            for (const std::string &categoryToMask : it.second) {
                categories_[it.first].maskBits |= categories_[categoryToMask].categoryBit;
            }
        }
    }
}
}
