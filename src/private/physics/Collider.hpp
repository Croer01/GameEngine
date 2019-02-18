//
// Created by adria on 31/10/2018.
//

#ifndef SPACEINVADERS_COLLIDER_HPP
#define SPACEINVADERS_COLLIDER_HPP


#include <Box2D/Box2D.h>
#include <memory>
#include <functional>
#include <glm/vec3.hpp>
#include <game-engine/api.hpp>
#include "../events/Subject.hpp"
namespace GameEngine {
    class ColliderComponent;

namespace Internal {

    enum class ColliderEvent {
        BeginCollider
    };

    class Collider : public Subject<ColliderEvent> {
    public:
        enum class ColliderShapes {
            Box
        };
        enum class ColliderTypes {
            Static,
            Dynamic,
            Kinematic
        };
    private:
        struct PropertiesToSetInSafeMode {
            bool hasPosition = false;
            b2Vec2 position;
            bool hasActive = false;
            bool active;
        };


        std::unique_ptr<b2Body, std::function<void(b2Body *)>> body_;
        b2Filter filter_;
        std::weak_ptr<ColliderComponent> component_;
        ColliderShapes colliderShape_;
        ColliderTypes colliderType_;
        PropertiesToSetInSafeMode propertiesToSetInSafeMode_;
        std::string category_;

        void addShapeToBody(float extendX, float extendY);

    public:
        b2BodyDef *getBodyDef() const;

        void setBody(b2Body *body);

        void setBody(b2Body *body, const b2Filter &filter);

        b2Body *getBody();

        void setShape(ColliderShapes type);

        void setType(ColliderTypes type);

        ColliderTypes getType();

        void setActive(bool active);

        void setPosition(const Vec2D &pos);

        Vec2D getPosition() const;

        void setCategory(const std::string &category);

        std::string getCategory() const;

        void setSize(float extendX, float extendY);

        void doBeginCollistion(Collider *other);

        std::shared_ptr<Collider> clone();

        void setComponent(const std::shared_ptr<ColliderComponent> &component);

        std::weak_ptr<ColliderComponent> getComponent();

        void updateInSafeMode();

    };
}
}

#endif //SPACEINVADERS_COLLIDER_HPP
