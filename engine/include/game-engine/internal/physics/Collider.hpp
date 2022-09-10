//
// Created by adria on 31/10/2018.
//

#ifndef SPACEINVADERS_COLLIDER_HPP
#define SPACEINVADERS_COLLIDER_HPP


#if defined _WIN32
#  include <Box2D/Box2D.h>
#else
#  include <box2d/box2d.h>
#endif
#include <memory>
#include <string>
#include <functional>
#include <glm/vec3.hpp>
#include <game-engine/api.hpp>
#include <game-engine/events/Subject.hpp>
#include <game-engine/internal/DelayedSetter.hpp>

namespace GameEngine {
    class ColliderComponent;

namespace Internal {

    enum class ColliderEvent {
        BeginCollider,
        BeginSensor,
        EndCollider,
        EndSensor
    };

    class PUBLICAPI Collider : public Subject<ColliderEvent, Internal::Collider *> {
    public:
        enum class ColliderShapes {
            Box,
            Circle
        };
        enum class ColliderTypes {
            Static,
            Dynamic,
            Kinematic
        };
    private:
       DelayedSetter<b2Vec2> position_;
       DelayedSetter<float> rotation_;
       DelayedSetter<bool> active_;
       DelayedSetter<ColliderTypes> colliderType_;
       DelayedSetter<float> mass_;
       DelayedSetter<bool> fixedRotation_;


        std::unique_ptr<b2Body, std::function<void(b2Body *)>> body_;
        b2Filter filter_;
        std::weak_ptr<ColliderComponent> component_;
        ColliderShapes colliderShape_;
        std::string category_;
        bool isSensor_;
        float gravityScale_;

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

        void setSensor(bool isSensor);

        bool IsSensor() const;

        void setGravityScale(float scale);
        float getGravityScale() const;

        void setMass(float value);
        float getMass() const;

        void setFixedRotation(bool value);
        bool getFixedRotation() const;

        void setSize(float extendX, float extendY);

        void setRotation(float radians);
        float getRotation() const;

        void doBeginCollision(Collider *other);
        void doBeginSensor(Collider *other);
        void doEndCollision(Collider *other);
        void doEndSensor(Collider *other);

        void setComponent(const std::shared_ptr<ColliderComponent> &component);

        std::weak_ptr<ColliderComponent> getComponent();

        void updateInSafeMode();

    };
}
}

#endif //SPACEINVADERS_COLLIDER_HPP
