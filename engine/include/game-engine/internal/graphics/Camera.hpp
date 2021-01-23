//
// Created by adria on 07/04/2019.
//

#ifndef ZOMBIEWAVE_CAMERA_HPP
#define ZOMBIEWAVE_CAMERA_HPP

#include <glm/glm.hpp>
#include <game-engine/geCamera.hpp>

namespace GameEngine {

class Game;

namespace Internal {
    class Camera : public geCamera {
        glm::mat4 viewMatrix_;
        Vec2D position_;
        Game *game_;
    public:
        explicit Camera(Game *game);
        glm::mat4 getViewMatrix() const;

        Vec2D position() const override;

        void position(const GameEngine::Vec2D &pos) override;
        void centerCameraToPosition(const GameEngine::Vec2D &pos) override;

        Vec2D worldToScreen(const GameEngine::Vec2D &posWorld) override;
        Vec2D ScreenToWorld(const GameEngine::Vec2D &posScreen) override;
    private:
        void calculateMatrix();
    };
}
}


#endif //ZOMBIEWAVE_CAMERA_HPP
