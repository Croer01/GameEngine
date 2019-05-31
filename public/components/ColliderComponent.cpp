//
// Created by adria on 31/10/2018.
//

#include <game-engine/components/ColliderComponent.hpp>
#include <game-engine/components/GeometryComponent.hpp>
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
                &ColliderComponent::extends,
                &ColliderComponent::extends,
                Vec2D()));
        properties->add(new Property<ColliderComponent, Vec2D>(
                "offset",
                this,
                &ColliderComponent::offset,
                &ColliderComponent::offset,
                Vec2D()));

        // Collider configuration
        properties->add(new Property<ColliderComponent, std::string>(
                "colliderShape",
                this,
                &ColliderComponent::shape,
                &ColliderComponent::shape,
                "Box"));
        properties->add(new Property<ColliderComponent, std::string>(
                "colliderType",
                this,
                &ColliderComponent::type,
                &ColliderComponent::type,
                "Static"));
        properties->add(new Property<ColliderComponent, std::string>(
                "category",
                this,
                &ColliderComponent::category,
                &ColliderComponent::category,
                ""));

        return properties;
    }

    void ColliderComponent::init() {
        if(auto sprite = gameObject()->getComponent<SpriteComponent>().lock()) {
            size_ = GameEngine::Vec2D(sprite->getWidth(), sprite->getHeight());
            offset_ = GameEngine::Vec2D(sprite->getWidth(), sprite->getHeight());
        }

        if(auto spriteAnimated = gameObject()->getComponent<SpriteAnimatedComponent>().lock())
            size_ = GameEngine::Vec2D(spriteAnimated->getWidth(), spriteAnimated->getHeight());

        if(auto geometry = gameObject()->getComponent<GeometryComponent>().lock())
            size_ = GameEngine::Vec2D(geometry->getWidth(), geometry->getHeight());

        // size is 0
        if(size_.sqrMagnitude() <= 1e-6){
            throw std::runtime_error(std::string("Can not set the size of the collider because there isn't a SpriteComponent, "
                                     "SpriteAnimatedComponent or GeometryComponent attached in the GameObject ") +
                                     gameObject()->name());
        }

        updateColliderRef();
        gameObject()->registerObserver(this);
    }

    void ColliderComponent::Update(float elapsedTime) {
        if(collider_->getType() == Internal::Collider::ColliderTypes::Dynamic)
            gameObject()->position(convertPhysicsToWorldPos(collider_->getPosition()));
    }

    void ColliderComponent::setOnColliderEnter(const OnColliderEventCallback &callback) {
        onColliderEnterCallback_ = callback;
    }

    void ColliderComponent::onEvent(const Subject<Internal::ColliderEvent> &target, const Internal::ColliderEvent &event, void *args) {
        if(event == Internal::ColliderEvent::BeginCollider && onColliderEnterCallback_) {
            auto *collider = static_cast<Internal::Collider *>(args);
            if(auto component = collider->getComponent().lock())
                onColliderEnterCallback_(component.get());
        }
    }

    void ColliderComponent::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
        if(event == GameObjectEvent::PositionChanged){
            if(!gameObject()->active() || collider_->getType() != Internal::Collider::ColliderTypes::Dynamic)
                collider_->setPosition(convertWorldToPhysicsPos(gameObject()->position()));
        }
        else if(event == GameObjectEvent::ScaleChanged){
            const Vec2D &scale = gameObject()->scale();
            collider_->setSize(std::abs(scale.x * size_.x)/2.f, std::abs(scale.y * size_.y)/2.f);
        }
        else if(event == GameObjectEvent::ActiveChanged){
            collider_->setActive(gameObject()->active());
        }
    }

    ColliderComponent::~ColliderComponent() {
        if(collider_) {
            Internal::PhysicsEngine::GetInstance().unregisterCollider(collider_);
            collider_->unregisterObserver(this);
        }

        if(onColliderEnterCallback_)
            onColliderEnterCallback_ = nullptr;

        if(gameObject())
            gameObject()->unregisterObserver(this);
    }

    void ColliderComponent::setVelocity(glm::vec3 velocity) {
        collider_->getBody()->SetLinearVelocity(b2Vec2(velocity.x,velocity.y));
    }

    void ColliderComponent::applyForce(glm::vec3 force) {
        collider_->getBody()->ApplyForceToCenter(b2Vec2(force.x,force.y), true);
    }

    Vec2D ColliderComponent::convertWorldToPhysicsPos(const Vec2D &worldPos) const {
        Vec2D result = worldPos + offset_;

        float xOffset = size_.x / 2.f;
        float yOffset = size_.y / 2.f;
        result += Vec2D(xOffset, yOffset);

        return result;
    }

    Vec2D ColliderComponent::convertPhysicsToWorldPos(const Vec2D &physicsPos) const {
        Vec2D result = physicsPos - offset_;


        float xOffset = size_.x/2.f;
        float yOffset = size_.y/2.f;
        result -= Vec2D(xOffset, yOffset);


        return result;
    }

    void ColliderComponent::onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) {
        if (oldGameObject)
            oldGameObject->unregisterObserver(this);
        if (newGameObject && collider_)
            newGameObject->registerObserver(this);
    }

    void ColliderComponent::extends(const Vec2D &extends) {
        extends_ = extends;
        if(collider_){
            if(gameObject() && extends_.x == 0 && extends_.y == 0){
                const Vec2D &scale = gameObject()->scale();
                collider_->setSize(std::abs(scale.x * size_.x)/2.f, std::abs(scale.y * size_.y)/2.f);
            }else
                collider_->setSize(extends_.x/2.f, extends_.y/2.f);
        }
    }

    Vec2D ColliderComponent::extends() const {
        return extends_;
    }

    void ColliderComponent::offset(const Vec2D &offsetValue) {
        offset_ = offsetValue;
    }

    Vec2D ColliderComponent::offset() const {
        return offset_;
    }
    void ColliderComponent::shape(const std::string &colliderShape) {
        colliderShape_ = colliderShape;
        if(collider_)
            collider_->setShape(stringToColliderShape(colliderShape));
    }

    std::string ColliderComponent::shape() const {
        return colliderShape_;
    }

    void ColliderComponent::type(const std::string &colliderType) {
        colliderType_ = colliderType;
        if(collider_)
            collider_->setType(stringToColliderType(colliderType));
    }

    std::string ColliderComponent::type() const {
        return colliderType_;
    }

    void ColliderComponent::category(const std::string &colliderCategory) {
        if(collider_)
            collider_->setCategory(colliderCategory);
        colliderCategory_ = colliderCategory;
    }

    std::string ColliderComponent::category() const {
        return colliderCategory_;
    }

    void ColliderComponent::updateColliderRef() {
        if(collider_)
            Internal::PhysicsEngine::GetInstance().unregisterCollider(collider_);

        collider_ = std::make_shared<Internal::Collider>();
        collider_->setShape(stringToColliderShape(colliderShape_));
        collider_->setType(stringToColliderType(colliderType_));
        collider_->setCategory(colliderCategory_);
        collider_->setComponent(shared_from_this());

        Internal::PhysicsEngine::GetInstance().registerCollider(collider_);
        collider_->setPosition(convertWorldToPhysicsPos(gameObject()->position()));
        collider_->setActive(gameObject()->active());

        collider_->registerObserver(this);

        extends(extends_);
    }
}

