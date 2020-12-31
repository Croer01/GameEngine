//
// Created by adria on 07/04/2019.
//

#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"
#include <game-engine/Game.hpp>

namespace GameEngine {
namespace Internal {
    glm::mat4 Camera::getViewMatrix() const {
        return viewMatrix_;
    }

    Camera::Camera(Game *game)
    {
        game_ = game;
        calculateMatrix();
    }

    Vec2D Camera::position() const {
        return position_;
    }

    void Camera::position(const GameEngine::Vec2D &pos) {
        position_ = pos;
        calculateMatrix();
    }

    void Camera::centerCameraToPosition(const GameEngine::Vec2D &pos) {
        assert(game_ != nullptr);
        auto screenOffset = Vec2D(game_->screen()->virtualWidth() / 2.f, game_->screen()->virtualHeight() / 2.f);
        position_ = pos - screenOffset;
        calculateMatrix();
    }

void Camera::calculateMatrix() {
        viewMatrix_ = glm::translate(glm::mat4(1), glm::vec3(-position_.x, -position_.y,0.f));
    }

    Vec2D Camera::worldToScreen(const GameEngine::Vec2D &posWorld) {
        return posWorld - position_;
    }

    Vec2D Camera::ScreenToWorld(const GameEngine::Vec2D &posScreen) {
        return posScreen + position_;
    }
}
}
