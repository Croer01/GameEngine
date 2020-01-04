//
// Created by adria on 01/11/2018.
//

#include "DebugView.hpp"
#include "PhysicsEngine.hpp"
#include "../Game.hpp"
#include "../utils.hpp"
#include "../graphics/MeshData.hpp"

#include <GL\glew.h>
#include <game-engine/geGameObject.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GameEngine {
namespace Internal {
    //TODO: fill the rest of the methods
    void DebugView::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
        beginDraw();

        //The order of the vertices are flipped to from "n" to "u" way to deal with the inverted y axis
        //VBO data
        std::vector<float> verticesUVs;
        for (int i = 0; i < vertexCount; i++) {
            //vertex(3) | uv(2)
            verticesUVs.push_back(vertices[i].x);
            verticesUVs.push_back(vertices[i].y);
            verticesUVs.push_back(0.f);
            verticesUVs.push_back(0.f);
            verticesUVs.push_back(0.f);
        }

        MeshData mesh(verticesUVs, std::vector<unsigned int> ());
        shader_->setUniform("Color", glm::vec4(color.r, color.g, color.b, color.a));
        mesh.draw(shader_);
        shader_->draw();

        endDraw();
    }

// We just need to have these to prevent override errors, they don't actually do anything right now
    void DebugView::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {}

    void DebugView::DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color) {}

    void DebugView::DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color) {}

    void DebugView::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) {}

    void DebugView::DrawTransform(const b2Transform &xf) {}

    void DebugView::DrawPoint(const b2Vec2 &p, float32 size, const b2Color &color) {}

    void DebugView::beginDraw() {
        const Screen &screen = Game::GetInstance().screen();

        glm::mat4 transform = glm::ortho<float>(0.0f, static_cast<float>(screen.virtualWidth()), static_cast<float>(screen.virtualHeight()), 0.f, 0.f, 1.f) *
                cam_->getViewMatrix();
        transform = glm::scale(transform , glm::vec3(PhysicsEngine::getScalePixelsToMeter(), PhysicsEngine::getScalePixelsToMeter(), 1));
        shader_->bind();
        shader_->setUniform("transform", transform);
    }


    void DebugView::endDraw() {
        shader_->unbind();
    }

    DebugView::DebugView() {
        shader_ = std::make_shared<Shader>("Basic");
        shader_->setVertexShader(R"EOF(
        #version 330 core

        layout (location = 0) in vec3 aPos;

        uniform mat4 transform;

        void main() {
            gl_Position = transform * vec4(aPos, 1.0);
        }
        )EOF");
        shader_->setFragmentShader(R"EOF(
        #version 330 core
        out vec4 FragColor;

        uniform vec4 Color;

        void main() {
            FragColor = Color;
        }
        )EOF");
        shader_->build();
        shader_->setElementMode(GL_LINE_LOOP);
        CheckGlError();
    }

    void DebugView::setCamera(const std::shared_ptr<Camera> &cam) {
        cam_ = cam;
    }
}
}