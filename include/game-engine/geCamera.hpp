//
// Created by adria on 07/04/2019.
//

#ifndef ZOMBIEWAVE_GECAMERA_HPP
#define ZOMBIEWAVE_GECAMERA_HPP

#include <game-engine/api.hpp>

namespace GameEngine {
    class PUBLICAPI geCamera {
    public:
        virtual ~geCamera(){};
        virtual GameEngine::Vec2D position() const = 0;
        virtual void position(const GameEngine::Vec2D &pos) = 0;
        virtual Vec2D worldToScreen(const GameEngine::Vec2D &posWorld) = 0;
        virtual Vec2D ScreenToWorld(const GameEngine::Vec2D &posScreen) = 0;
    };
}

#endif //ZOMBIEWAVE_GECAMERA_HPP
