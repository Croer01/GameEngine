//
// Created by adria on 31/10/2018.
//

#ifndef SPACEINVADERS_COLLIDERCOMPONENT_HPP
#define SPACEINVADERS_COLLIDERCOMPONENT_HPP

#include <game-engine/geComponent.hpp>
#include "../physics/Collider.hpp"
#include "SpriteComponent.hpp"
#include "SpriteAnimatedComponent.hpp"

namespace GameEngine {
namespace Internal {
        class ColliderComponent;

        typedef std::function<void(ColliderComponent *)> OnColliderEventCallback;

        class ColliderComponent : public geComponent, public Observer<ColliderEvent>, public Observer<GameObjectEvent> {
            std::shared_ptr<Collider> collider_;
            OnColliderEventCallback onColliderEnterCallback_;
            std::weak_ptr<SpriteComponent> sprite_;
            std::weak_ptr<SpriteAnimatedComponent> spriteAnimated_;
            Vec2D extends_;
            Vec2D offset_;

            Vec2D convertWorldToPhysicsPos(const Vec2D &worldPos) const;

            Vec2D convertPhysicsToWorldPos(const Vec2D &physicsPos) const;

        public:
            virtual ~ColliderComponent();

            void init() override;

            void Update(float elapsedTime) override;

            void onEvent(const Subject<ColliderEvent> &target, const ColliderEvent &event, void *args) override;

            void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;

            //Component collider public API
            void setOnColliderEnter(const OnColliderEventCallback &callback);

            void setVelocity(glm::vec3 velocity);

            void applyForce(glm::vec3 force);

        protected:
            void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;
        };
    }
}

#endif //SPACEINVADERS_COLLIDERCOMPONENT_HPP
