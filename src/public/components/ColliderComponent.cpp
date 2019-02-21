//
// Created by adria on 31/10/2018.
//

#include <game-engine/components/ColliderComponent.hpp>
#include "../private/physics/PhysicsEngine.hpp"
#include "../../private/GameObject.hpp"

namespace GameEngine {
namespace {
    Internal::Collider::ColliderShapes stringToColliderShape(const std::string &shapeName) {
        if(shapeName == "Box" || shapeName == "box")
            return Internal::Collider::ColliderShapes::Box;

        throw std::runtime_error("Unknown " + shapeName + " collider type.");
    }

    Internal::Collider::ColliderTypes stringToColliderType(const std::string &typeName) {
        if(typeName == "Static" || typeName == "static")
            return Internal::Collider::ColliderTypes::Static;
        else if(typeName == "Dynamic" || typeName == "dynamic")
            return Internal::Collider::ColliderTypes::Dynamic;
        else if(typeName == "Kinematic" || typeName == "kinematic")
            return Internal::Collider::ColliderTypes::Kinematic;

        throw std::runtime_error("Unknown " + typeName + " collider type.");
    }
}

    PropertySetBase *ColliderComponent::configureProperties() {
        auto *properties = new PropertySet<ColliderComponent>(this);

        properties->add(new Property<ColliderComponent, Vec2D>(
                "extends",
                this,
                nullptr,
                &ColliderComponent::extends,
                Vec2D()));
        properties->add(new Property<ColliderComponent, Vec2D>(
                "offset",
                this,
                nullptr,
                &ColliderComponent::offset,
                Vec2D()));

        // Collider configuration
        properties->add(new Property<ColliderComponent, std::string>(
                "colliderShape",
                this,
                nullptr,
                &ColliderComponent::shape,
                "Box"));
        properties->add(new Property<ColliderComponent, std::string>(
                "colliderShape",
                this,
                nullptr,
                &ColliderComponent::type,
                "Static"));
        properties->add(new Property<ColliderComponent, std::string>(
                "category",
                this,
                nullptr,
                &ColliderComponent::category,
                ""));

        return properties;
    }

    void ColliderComponent::init() {
        Internal::PhysicsEngine::GetInstance().registerCollider(collider_);

        sprite_ = gameObject()->getComponent<SpriteComponent>();
        spriteAnimated_ = gameObject()->getComponent<SpriteAnimatedComponent>();

        collider_->setPosition(convertWorldToPhysicsPos(gameObject()->position()));
        collider_->setActive(gameObject()->active());

        collider_->registerObserver(this);
    }

    void ColliderComponent::Update(float elapsedTime) {
        if(collider_->getType() == Internal::Collider::ColliderTypes::Dynamic)
            gameObject()->position(convertPhysicsToWorldPos(collider_->getPosition()));
    }

    void ColliderComponent::setOnColliderEnter(const OnColliderEventCallback &callback) {
        onColliderEnterCallback_ = callback;
    }

    void ColliderComponent::onEvent(const Internal::Subject<Internal::ColliderEvent> &target, const Internal::ColliderEvent &event, void *args) {
        if(event == Internal::ColliderEvent::BeginCollider && onColliderEnterCallback_) {
            auto *collider = static_cast<Internal::Collider *>(args);
            if(auto component = collider->getComponent().lock())
                onColliderEnterCallback_(component.get());
        }
    }

    void ColliderComponent::onEvent(const Internal::Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
        if(event == GameObjectEvent::PositionChanged){
            if(!gameObject()->active() || collider_->getType() != Internal::Collider::ColliderTypes::Dynamic)
                collider_->setPosition(convertWorldToPhysicsPos(gameObject()->position()));
        }
        else if(event == GameObjectEvent::ScaleChanged){
            const Vec2D &scale = gameObject()->scale();
            collider_->setSize(std::abs(scale.x)/2.f, std::abs(scale.y)/2.f);
        }
        else if(event == GameObjectEvent::ActiveChanged){
            collider_->setActive(gameObject()->active());
        }
    }

    ColliderComponent::~ColliderComponent() {
        if(collider_)
            Internal::PhysicsEngine::GetInstance().unregisterCollider(collider_);
    }

    void ColliderComponent::setVelocity(glm::vec3 velocity) {
        collider_->getBody()->SetLinearVelocity(b2Vec2(velocity.x,velocity.y));
    }

    void ColliderComponent::applyForce(glm::vec3 force) {
        collider_->getBody()->ApplyForceToCenter(b2Vec2(force.x,force.y), true);
    }

    Vec2D ColliderComponent::convertWorldToPhysicsPos(const Vec2D &worldPos) const {
        Vec2D result = worldPos + offset_;

        if(auto sprite = sprite_.lock()) {
            float xOffset = sprite->getWidth() / 2.f;
            float yOffset = sprite->getHeight() / 2.f;
            result += Vec2D(xOffset, yOffset);
        }
        else if(auto sprite = spriteAnimated_.lock()) {
            float xOffset = sprite->getWidth() / 2.f;
            float yOffset = sprite->getHeight() / 2.f;
            result += Vec2D(xOffset, yOffset);
        }

        return result;
    }

    Vec2D ColliderComponent::convertPhysicsToWorldPos(const Vec2D &physicsPos) const {
        Vec2D result = physicsPos - offset_;

        if(auto sprite = sprite_.lock()) {
            float xOffset = sprite->getWidth()/2.f;
            float yOffset = sprite->getHeight()/2.f;
            result -= Vec2D(xOffset,yOffset);
        }
        else if(auto sprite = spriteAnimated_.lock()) {
            float xOffset = sprite->getWidth()/2.f;
            float yOffset = sprite->getHeight()/2.f;
            result -= Vec2D(xOffset, yOffset);
        }

        return result;
    }

    void ColliderComponent::onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) {
        if (oldGameObject)
            dynamic_cast<Internal::GameObject*>(oldGameObject)->unregisterObserver(this);
        if (newGameObject)
            dynamic_cast<Internal::GameObject*>(newGameObject)->registerObserver(this);
    }

    void ColliderComponent::extends(const Vec2D &extends) {
        extends_ = extends;
        if(gameObject() && extends_.x == 0 && extends_.y == 0){
            const Vec2D &scale = gameObject()->scale();
            collider_->setSize(std::abs(scale.x)/2.f, std::abs(scale.y)/2.f);
        }else
            collider_->setSize(extends_.x/2.f, extends_.y/2.f);
    }

    void ColliderComponent::offset(const Vec2D &offsetValue) {
        offset_ = offsetValue;
    }

    void ColliderComponent::shape(const std::string &colliderShape) {
        if(!collider_)
            collider_ = std::make_shared<Internal::Collider>();
        collider_->setShape(stringToColliderShape(colliderShape));
    }
    void ColliderComponent::type(const std::string &colliderType) {
        if(!collider_)
            collider_ = std::make_shared<Internal::Collider>();

        collider_->setType(stringToColliderType(colliderType));
    }
    void ColliderComponent::category(const std::string &colliderCategory) {
        if(!collider_)
            collider_ = std::make_shared<Internal::Collider>();
        collider_->setCategory(colliderCategory);
    }
}

