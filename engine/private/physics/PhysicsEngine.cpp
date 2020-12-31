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

    // It isn't necessary to clear all the physics objects in
    // the PhysicsEngine's destructor because Box2D destroy those objects registered.
    world_.reset(new b2World(b2Vec2(0, 9.8f)));
    world_->SetContactListener(this);
}

#ifdef DEBUG
void PhysicsEngine::init(float timeStep, Screen *screen) {
        init(timeStep);

        debugView_.reset(new DebugView(screen));
        world_->SetDebugDraw(debugView_.get());
        debugView_->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit);
    }
#endif

    void PhysicsEngine::update(float elapsedTime) {
        //update all the colliders in safe mode
        for (const std::shared_ptr<Collider> &collider : colliders_) {
            collider->updateInSafeMode();
        }

        timeAcumulator_ += elapsedTime;
        if (timeAcumulator_ >= timeStep_) {
            world_->Step(timeStep_, velocityIterations_, positionIterations_);
            timeAcumulator_ -= timeStep_;
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

    void PhysicsEngine::unregisterCollider(const std::shared_ptr<Collider> &collider)
    {
        if(colliders_.empty())
            return;

        auto it = std::find(colliders_.begin(), colliders_.end(), collider);
        if (it != colliders_.end())
        {
            colliders_.erase(it);
            world_->DestroyBody(collider->getBody());
        }
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
            if(contact->GetFixtureA()->IsSensor())
            {
                colliderA->doBeginSensor(colliderB);
            }
            else if(contact->GetFixtureB()->IsSensor())
            {
                colliderB->doBeginSensor(colliderA);
            }
            else
            {
                colliderA->doBeginCollision(colliderB);
                colliderB->doBeginCollision(colliderA);
            }
        }
    }

void PhysicsEngine::EndContact(b2Contact *contact)
{
    Collider *colliderA = nullptr;
    Collider *colliderB = nullptr;

    void *userData = contact->GetFixtureA()->GetBody()->GetUserData();
    if (userData)
        colliderA = static_cast<Collider *>(userData);

    userData = contact->GetFixtureB()->GetBody()->GetUserData();
    if (userData)
        colliderB = static_cast<Collider *>(userData);

    if (colliderA && colliderB) {
        if(contact->GetFixtureA()->IsSensor())
        {
            colliderA->doEndSensor(colliderB);
        }
        else if(contact->GetFixtureB()->IsSensor())
        {
            colliderB->doEndSensor(colliderA);
        }
        else
        {
            colliderA->doEndCollision(colliderB);
            colliderB->doEndCollision(colliderA);
        }
    }
}

#ifdef DEBUG
void PhysicsEngine::drawDebug(Camera *cam) {
    debugView_->setCamera(cam);
    world_->DrawDebugData();
}
#endif

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

            // If is 0 at this point means that this category will collide with all
            if(categories_[it.first].maskBits == 0)
            {
                // The default value of the maskBits defined by Box2D
                categories_[it.first].maskBits = 0xFFFF;
            }
        }
    }

    PhysicsEngine::~PhysicsEngine()
    {
        for(auto collider : colliders_)
        {
            world_->DestroyBody(collider->getBody());
        }
        colliders_.clear();
    }

b2Joint *PhysicsEngine::createRevoluteJoin(b2Body *bodyA, b2Body *bodyB, float xBodyA, float yBodyA)
{
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = bodyA;
    jointDef.enableLimit = true;
    jointDef.lowerAngle = 0;//with both these at zero...
    jointDef.upperAngle = 0;//...the joint will not move
    jointDef.localAnchorB.SetZero();//joint anchor in tire is always center

    jointDef.bodyB = bodyB;
    jointDef.localAnchorA.Set(xBodyA, yBodyA);
    return world_->CreateJoint( &jointDef );
}
}
}
