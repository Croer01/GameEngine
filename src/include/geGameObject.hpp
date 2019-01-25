//
// Created by adria on 23/01/2019.
//

#ifndef SPACEINVADERS_GEGAMEOBJECT_HPP
#define SPACEINVADERS_GEGAMEOBJECT_HPP

#include "api.hpp"

namespace GameEngine {

    enum class GameObjectEvent{
        TransformChanged,
        PositionChanged,
        RotationChanged,
        ScaleChanged,
        ActiveChanged
    };

    class geGameObject {
    public:
        virtual ~geGameObject() {};

        virtual Vec2D position() const =0;

        virtual void position(const Vec2D &position) = 0;

        virtual Vec2D rotation() const = 0;

        virtual void rotation(const Vec2D &rotation) = 0;

        virtual Vec2D scale() const = 0;

        virtual void scale(const Vec2D &scale) = 0;
    };

}
#endif //SPACEINVADERS_GEGAMEOBJECT_HPP
