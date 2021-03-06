//
// Created by adria on 31/10/2018.
//

#ifndef SPACEINVADERS_COLLIDERCOMPONENT_HPP
#define SPACEINVADERS_COLLIDERCOMPONENT_HPP

#include <game-engine/geComponent.hpp>
#include <game-engine/components/SpriteComponent.hpp>
#include <game-engine/internal/physics/Collider.hpp>
#include <game-engine/components/SpriteAnimatedComponent.hpp>
#include <game-engine/internal/physics/PhysicsEngine.hpp>

namespace GameEngine {
    class ColliderComponent;

    typedef std::function<void(ColliderComponent *)> OnColliderEventCallback;

class PUBLICAPI ColliderComponent : public geComponentInstantiable<ColliderComponent>, public Observer<Internal::ColliderEvent>, public Observer<GameObjectEvent> {
        std::shared_ptr<Internal::Collider> collider_;
        Internal::PhysicsEngine *physicsEngine_;
        OnColliderEventCallback onColliderEnterCallback_;
        OnColliderEventCallback onSensorEnterCallback_;
        OnColliderEventCallback onColliderExitCallback_;
        OnColliderEventCallback onSensorExitCallback_;
        Vec2D size_;
        Vec2D offsetFromRender_;
        Vec2D extends_;
        Vec2D offset_;
        std::string colliderShape_;
        std::string colliderType_;
        std::string colliderCategory_;
        bool isSensor_;
        float gravityScale_;
        float mass_;
        bool fixedRotation_;
        // This flag is to ensure the position of the collider is set to the gameObject if the type is changed.
        // For example, if change from Dynamic to Static we need to ensure the position of the collider is correctly set after change to static
        bool typeChanged_;

        Vec2D convertWorldToPhysicsPos(const Vec2D &worldPos) const;

        Vec2D convertPhysicsToWorldPos(const Vec2D &physicsPos) const;

        void updateColliderRef();

        void updateColliderSize();
    public:
        virtual ~ColliderComponent();

        void init() override;

        void Update(float elapsedTime) override;

        //Component collider public API
        void setOnColliderEnter(const OnColliderEventCallback &callback);
        void setOnSensorEnter(const OnColliderEventCallback &callback);
        void setOnColliderExit(const OnColliderEventCallback &callback);
        void setOnSensorExit(const OnColliderEventCallback &callback);

        Vec2D getVelocity() const;
        void setVelocity(const Vec2D &velocity);

        void applyForce(const Vec2D &force);

    PropertySetBase *getProperties() const override;

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
        bool isSensor() const;
        void isSensor(const bool &value);
        float gravityScale() const;
        void gravityScale(const float &value);
        float mass() const;
        void mass(const float &value);
        bool fixedRotation() const;
        void fixedRotation(const bool &value);
protected:
    void onEvent(const Subject<Internal::ColliderEvent> &target, const Internal::ColliderEvent &event, void *args) override;

        void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;

        void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;
    };
}

#endif //SPACEINVADERS_COLLIDERCOMPONENT_HPP
