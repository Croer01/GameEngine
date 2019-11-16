//
// Created by adria on 31/10/2018.
//

#ifndef SPACEINVADERS_COLLIDERCOMPONENT_HPP
#define SPACEINVADERS_COLLIDERCOMPONENT_HPP

#include <game-engine/geComponent.hpp>
#include <game-engine/components/SpriteComponent.hpp>
#include <game-engine/properties/PropertiesRegister.hpp>
#include "../private/physics/Collider.hpp"
#include "SpriteAnimatedComponent.hpp"

namespace GameEngine {
    class ColliderComponent;

    typedef std::function<void(ColliderComponent *)> OnColliderEventCallback;

class PUBLICAPI ColliderComponent : public geComponentInstantiable<ColliderComponent>, public Observer<Internal::ColliderEvent>, public Observer<GameObjectEvent> {
        std::shared_ptr<Internal::Collider> collider_;
        OnColliderEventCallback onColliderEnterCallback_;
        Vec2D size_;
        Vec2D offsetFromRender_;
        Vec2D extends_;
        Vec2D offset_;
        std::string colliderShape_;
        std::string colliderType_;
        std::string colliderCategory_;

        Vec2D convertWorldToPhysicsPos(const Vec2D &worldPos) const;

        Vec2D convertPhysicsToWorldPos(const Vec2D &physicsPos) const;

        void updateColliderRef();

    public:
        virtual ~ColliderComponent();

        void init() override;

        void Update(float elapsedTime) override;

        //Component collider public API
        void setOnColliderEnter(const OnColliderEventCallback &callback);

        void setVelocity(const Vec2D &velocity);

        void applyForce(const Vec2D &force);

        Vec2D extends() const;
        void extends(const Vec2D &extends);
        Vec2D offset() const;
        void offset(const Vec2D &offsetValue);
        std::string shape() const;
        void shape(const std::string &colliderShape);
        std::string type() const;
        void type(const std::string &colliderType);
        std::string category() const;
        void category(const std::string &colliderCategory);

    protected:
        void onEvent(const Subject<Internal::ColliderEvent> &target, const Internal::ColliderEvent &event, void *args) override;

        void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;

        void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;
    };

template <>
class PropertyInstantiator<ColliderComponent>
{
public:
    static std::string getTargetName()
    {
        return "ColliderComponent";
    }

    static PropertySetBase* instantiate()
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
        properties->add(new Property<ColliderComponent, std::string>(
            "colliderShape",
            &ColliderComponent::shape,
            &ColliderComponent::shape,
            "Box"));
        properties->add(new Property<ColliderComponent, std::string>(
            "colliderType",
            &ColliderComponent::type,
            &ColliderComponent::type,
            "Static"));
        properties->add(new Property<ColliderComponent, std::string>(
            "category",
            &ColliderComponent::category,
            &ColliderComponent::category,
            ""));

        return properties;
    }
};
}

#endif //SPACEINVADERS_COLLIDERCOMPONENT_HPP
