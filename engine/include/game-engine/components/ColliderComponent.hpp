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
#include <game-engine/components/ComponentData.hpp>

namespace GameEngine {
class ColliderComponentData : public ComponentData
{
public:
   ColliderComponentData()
   {
       createProperty<Vec2D>("extends",Vec2D());
       createProperty<Vec2D>("offset",Vec2D());
       createPropertyEnum("colliderShape","Box",{
           "Box",
           "Circle"
       });
       createPropertyEnum("colliderType","Static",{
           "Static",
           "Dynamic",
           "Kinematic"
       });
       createProperty<std::string>("category","");
       createProperty<bool>("isSensor",false);
       createProperty<float>("gravityScale",1.f);
       createProperty<float>("mass",1.f);
       createProperty<bool>("fixedRotation", true);
   }
};

    class ColliderComponent;

    typedef std::function<void(ColliderComponent *)> OnColliderEventCallback;

class PUBLICAPI ColliderComponent : public geComponentInstantiable<ColliderComponent, ColliderComponentData>,
    public Observer<Internal::ColliderEvent, Internal::Collider *>, public Observer<GameObjectEvent>,
public std::enable_shared_from_this<ColliderComponent>{
        std::shared_ptr<Internal::Collider> collider_;
        Internal::PhysicsEngine *physicsEngine_;
        OnColliderEventCallback onColliderEnterCallback_;
        OnColliderEventCallback onSensorEnterCallback_;
        OnColliderEventCallback onColliderExitCallback_;
        OnColliderEventCallback onSensorExitCallback_;
        Vec2D size_;
        Vec2D offsetFromRender_;
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

protected:
    void onEvent(const Subject<Internal::ColliderEvent, Internal::Collider*> &target, Internal::ColliderEvent event, Internal::Collider *other) override;

    void onEvent(const Subject<GameObjectEvent> &target, GameObjectEvent event) override;

    void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;
    };
}

#endif //SPACEINVADERS_COLLIDERCOMPONENT_HPP
