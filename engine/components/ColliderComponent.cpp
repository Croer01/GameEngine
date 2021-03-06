//
// Created by adria on 31/10/2018.
//

#include <game-engine/components/ColliderComponent.hpp>
#include <game-engine/components/GeometryComponent.hpp>
#include <iostream>
#include <game-engine/internal/GameObject.hpp>
#include <game-engine/Game.hpp>

namespace GameEngine {
namespace {
    Internal::Collider::ColliderShapes stringToColliderShape(const std::string &shapeName) {
        if(shapeName == "Box" || shapeName == "box")
            return Internal::Collider::ColliderShapes::Box;
        else if(shapeName == "Circle" || shapeName == "circle")
            return Internal::Collider::ColliderShapes::Circle;

        throw std::runtime_error("Unknown " + shapeName + " collider shape.");
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

    void ColliderComponent::init() {
        physicsEngine_ = gameObject()->game()->physicsEngine();
        if(auto sprite = gameObject()->getComponent<SpriteComponent>().lock()) {
            size_ = GameEngine::Vec2D(sprite->getWidth(), sprite->getHeight());
            glm::vec2 offset = Internal::parseGraphicAnchorToVec2D(Internal::parseStringToGraphicAnchor(sprite->anchor()));
            offsetFromRender_ = Vec2D(offset.x, offset.y) * size_;
        }

        if(auto spriteAnimated = gameObject()->getComponent<SpriteAnimatedComponent>().lock()){
            size_ = GameEngine::Vec2D(spriteAnimated->getWidth(), spriteAnimated->getHeight());
            glm::vec2 offset = Internal::parseGraphicAnchorToVec2D(Internal::parseStringToGraphicAnchor(spriteAnimated->anchor()));
            offsetFromRender_ = Vec2D(offset.x, offset.y) * size_;
        }

        if(auto geometry = gameObject()->getComponent<GeometryComponent>().lock()){
            size_ = GameEngine::Vec2D(geometry->getWidth(), geometry->getHeight());
            glm::vec2 offset = Internal::parseGraphicAnchorToVec2D(Internal::parseStringToGraphicAnchor(geometry->anchor()));
            offsetFromRender_ = Vec2D(offset.x, offset.y) * size_;
        }

        // size is 0
        if(size_.sqrMagnitude() <= 1e-6){
            size_.x = 1.f;
            size_.y = 1.f;
            std::cerr << "Can not set the size of the collider because there isn't a SpriteComponent, " <<
                      "SpriteAnimatedComponent or GeometryComponent attached in the GameObject " <<
                      gameObject()->name() <<
                      ". Set (1,1) by default" << std::endl;
        }

        updateColliderRef();
        gameObject()->registerObserver(this);
        typeChanged_ = false;
    }

    void ColliderComponent::Update(float elapsedTime) {
        if(typeChanged_ || collider_->getType() == Internal::Collider::ColliderTypes::Dynamic)
        {
            Vec2D position = collider_->getPosition();
            float rotation = collider_->getRotation();
            if(auto parent = gameObject()->parent().lock())
            {
                position -= parent->position();
                rotation -= parent->rotation();
            }
            gameObject()->position(convertPhysicsToWorldPos(position));
            gameObject()->rotation(rotation);
            typeChanged_ = false;
        }
    }

    void ColliderComponent::setOnColliderEnter(const OnColliderEventCallback &callback) {
        onColliderEnterCallback_ = callback;
    }

    void ColliderComponent::setOnSensorEnter(const OnColliderEventCallback &callback) {
        onSensorEnterCallback_ = callback;
    }

    void ColliderComponent::setOnColliderExit(const OnColliderEventCallback &callback) {
        onColliderExitCallback_ = callback;
    }

    void ColliderComponent::setOnSensorExit(const OnColliderEventCallback &callback) {
        onSensorExitCallback_ = callback;
    }

void ColliderComponent::onEvent(const Subject<Internal::ColliderEvent> &target, const Internal::ColliderEvent &event, void *args) {
        auto *collider = static_cast<Internal::Collider *>(args);
        if(auto component = collider->getComponent().lock())
        {
            if(event == Internal::ColliderEvent::BeginCollider && onColliderEnterCallback_)
                onColliderEnterCallback_(component.get());
            if(event == Internal::ColliderEvent::BeginSensor && onSensorEnterCallback_)
                onSensorEnterCallback_(component.get());
            if(event == Internal::ColliderEvent::EndCollider && onColliderExitCallback_)
                onColliderExitCallback_(component.get());
            if(event == Internal::ColliderEvent::EndSensor && onSensorExitCallback_)
                onSensorExitCallback_(component.get());
        }
    }

    void ColliderComponent::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
        if(event == GameObjectEvent::PositionChanged){
            collider_->setPosition(convertWorldToPhysicsPos(gameObject()->position()));
        }
        else if(event == GameObjectEvent::RotationChanged){
            collider_->setRotation(gameObject()->rotation());
        }
        else if(event == GameObjectEvent::ScaleChanged){
            updateColliderSize();
        }
        else if(event == GameObjectEvent::ActiveChanged){
            collider_->setActive(gameObject()->active());
        }
    }

    ColliderComponent::~ColliderComponent() {
        if(physicsEngine_ != nullptr && collider_) {
            physicsEngine_->unregisterCollider(collider_);
            collider_->unregisterObserver(this);
        }

        if(onColliderEnterCallback_)
            onColliderEnterCallback_ = nullptr;

        if(gameObject())
            gameObject()->unregisterObserver(this);
    }

    Vec2D ColliderComponent::getVelocity() const
    {
        const b2Vec2 &velocity = collider_->getBody()->GetLinearVelocity();
        return Vec2D(velocity.x, velocity.y);
    }

    void ColliderComponent::setVelocity(const Vec2D &velocity) {
        collider_->getBody()->SetLinearVelocity(b2Vec2(velocity.x,velocity.y));
    }

    void ColliderComponent::applyForce(const Vec2D &force) {
        collider_->getBody()->ApplyForceToCenter(b2Vec2(force.x,force.y), true);
    }

    Vec2D ColliderComponent::convertWorldToPhysicsPos(const Vec2D &worldPos) const {
        Vec2D result = worldPos + offset_ - offsetFromRender_;

        Vec2D scale = gameObject()? gameObject()->scale() : Vec2D(1.f, 1.f);
        result += (size_ * scale) / 2.f;

        return result;
    }

    Vec2D ColliderComponent::convertPhysicsToWorldPos(const Vec2D &physicsPos) const {
        Vec2D result = physicsPos - offset_ + offsetFromRender_;

        Vec2D scale = gameObject()? gameObject()->scale() : Vec2D(1.f, 1.f);
        result -= (size_ * scale) / 2.f;

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
        updateColliderSize();
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
        typeChanged_ = true;
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
        if(physicsEngine_ == nullptr)
            return;

        if(collider_)
            physicsEngine_->unregisterCollider(collider_);

        collider_ = std::make_shared<Internal::Collider>();
        collider_->setShape(stringToColliderShape(colliderShape_));
        collider_->setType(stringToColliderType(colliderType_));
        collider_->setCategory(colliderCategory_);
        collider_->setSensor(isSensor_);
        collider_->setGravityScale(gravityScale_);
        collider_->setMass(mass_);
        collider_->setFixedRotation(fixedRotation_);
        collider_->setComponent(std::dynamic_pointer_cast<ColliderComponent>(shared_from_this()));

        physicsEngine_->registerCollider(collider_);
        collider_->setPosition(convertWorldToPhysicsPos(gameObject()->position()));
        collider_->setActive(gameObject()->active());

        collider_->registerObserver(this);

        extends(extends_);
    }

PropertySetBase *ColliderComponent::getProperties() const
{
    auto *properties = new PropertySet<ColliderComponent>();

    properties->add(new Property<ColliderComponent, Vec2D>(
            "extends",
            &ColliderComponent::extends,
            &ColliderComponent::extends,
            Vec2D()));
    properties->add(new Property<ColliderComponent, Vec2D>(
            "offset",
            &ColliderComponent::offset,
            &ColliderComponent::offset,
            Vec2D()));

    // Collider configuration
    properties->add(new PropertyEnum<ColliderComponent>(
            "colliderShape",
            &ColliderComponent::shape,
            &ColliderComponent::shape,
            "Box",
            {
                    "Box",
                    "Circle"
            }));
    properties->add(new PropertyEnum<ColliderComponent>(
            "colliderType",
            &ColliderComponent::type,
            &ColliderComponent::type,
            "Static",
            {
                    "Static",
                    "Dynamic",
                    "Kinematic"
            }));
    properties->add(new Property<ColliderComponent, std::string>(
            "category",
            &ColliderComponent::category,
            &ColliderComponent::category,
            ""));
    properties->add(new Property<ColliderComponent, bool>(
            "isSensor",
            &ColliderComponent::isSensor,
            &ColliderComponent::isSensor,
            false,
            false));
    properties->add(new Property<ColliderComponent, float>(
            "gravityScale",
            &ColliderComponent::gravityScale,
            &ColliderComponent::gravityScale,
            1.f,
            false));
    properties->add(new Property<ColliderComponent, float>(
        "mass",
        &ColliderComponent::mass,
        &ColliderComponent::mass,
        1.f,
        false));
    properties->add(new Property<ColliderComponent, bool>(
        "fixedRotation",
        &ColliderComponent::fixedRotation,
        &ColliderComponent::fixedRotation,
        true,
        false));

    return properties;
}

bool ColliderComponent::isSensor() const
{
    return isSensor_;
}
void ColliderComponent::isSensor(const bool &value)
{
    isSensor_ = value;
    if(collider_)
        collider_->setSensor(isSensor_);
}

float ColliderComponent::gravityScale() const
{
    return gravityScale_;
}

void ColliderComponent::gravityScale(const float &value)
{
    gravityScale_ = value;
    if(collider_)
        collider_->setGravityScale(gravityScale_);
}

float ColliderComponent::mass() const
{
    return mass_;
}

void ColliderComponent::mass(const float &value)
{
    mass_ = value;
    if(collider_)
        collider_->setMass(mass_);
}

bool ColliderComponent::fixedRotation() const
{
    return fixedRotation_;
}
void ColliderComponent::fixedRotation(const bool &value)
{
    fixedRotation_ = value;
    if(collider_)
        collider_->setFixedRotation(fixedRotation_);
}

void ColliderComponent::updateColliderSize()
{
    if(collider_)
    {
        Vec2D scale = gameObject()? gameObject()->scale() : Vec2D(1.f, 1.f);

        if(extends_.x == 0 && extends_.y == 0){
            collider_->setSize(std::abs(scale.x * size_.x)/2.f, std::abs(scale.y * size_.y)/2.f);
        }else
            collider_->setSize(scale.x * extends_.x/2.f, scale.y * extends_.y/2.f);
    }
}
}

