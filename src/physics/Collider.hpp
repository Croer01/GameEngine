//
// Created by adria on 31/10/2018.
//

#ifndef SPACEINVADERS_COLLIDER_HPP
#define SPACEINVADERS_COLLIDER_HPP


#include <Box2D/Box2D.h>
#include <memory>
#include <functional>
#include <glm/vec3.hpp>
#include "../events/Subject.hpp"

class ColliderComponent;

enum class ColliderEvent{
    BeginCollider
};

class Collider : public Subject<ColliderEvent>{
public:
    enum class ColliderShapes{
        Box
    };
    enum class ColliderTypes{
        Static,
        Dynamic,
        Kinameatic
    };
private:
    std::unique_ptr<b2Body, std::function<void(b2Body*)>> body_;
    std::shared_ptr<ColliderComponent> component_;
    ColliderShapes colliderShape_;
    ColliderTypes colliderType_;

    void addShapeToBody(float extendX, float extendY);

public:
    b2BodyDef *getBodyDef() const;
    void setBody(b2Body *body);
    b2Body *getBody();
    void setShape(ColliderShapes type);
    void setType(ColliderTypes type);
    ColliderTypes getType();
    void setActive(bool active);
    void setPosition(float x, float y);
    glm::vec3 getPosition() const;
    void setSize(float extendX, float extendY);
    void doBeginCollistion(Collider *other);
    std::shared_ptr<Collider> clone();
    void setComponent(const std::shared_ptr<ColliderComponent> &component);
    std::shared_ptr<ColliderComponent> getComponent();
};


#endif //SPACEINVADERS_COLLIDER_HPP
