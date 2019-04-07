//
// Created by adria on 07/04/2019.
//

#ifndef ZOMBIEWAVE_CAMERA_HPP
#define ZOMBIEWAVE_CAMERA_HPP

#include <glm/glm.hpp>
#include <game-engine/geCamera.hpp>

namespace GameEngine {
namespace Internal {
    class Camera : public geCamera {
        glm::mat4 viewMatrix_;
        Vec2D position_;
    public:
        Camera();
        glm::mat4 getViewMatrix() const;

        Vec2D position() const override;

        void position(const GameEngine::Vec2D &pos) override;

    private:
        void calculateMatrix();
    };
}
}


#endif //ZOMBIEWAVE_CAMERA_HPP
