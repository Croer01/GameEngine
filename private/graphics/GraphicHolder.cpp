//
// Created by adria on 27/10/2018.
//

#include "GraphicHolder.hpp"
#include "GraphicsEngine.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
namespace GameEngine {
namespace Internal {
    GraphicHolder::GraphicHolder(const std::shared_ptr<Graphic> &graphic) :
            graphic_(graphic),
            modelTransform_(glm::mat4(1)),
            active_(true),
            texOffset_(0),
            texScale_(1) {
    }

    void GraphicHolder::draw(const std::shared_ptr<Shader> &shader) {
        if (!active_)
            return;
        shader->setUniform("transform", modelTransform_);
        shader->setUniform("TexOffest", texOffset_);
        shader->setUniform("TexCoordScale", texScale_);
        graphic_->draw(shader);
    }

    void
    GraphicHolder::setModelTransform(const Vec2D &position, const Vec2D &rotation, const Vec2D &scale) {
        glm::vec3 desiredPosition = glm::vec3(position.x,position.y,0.f);

        if (GraphicsEngine::GetInstance().isPixelPerfect())
            desiredPosition = glm::round(desiredPosition);

        glm::mat4 anchorTransform = glm::translate(glm::mat4(1), glm::vec3(anchor_, 0.f));

        //remember the order of matrix multiplication is from right to left
        modelTransform_ = glm::translate(glm::mat4(1), desiredPosition) *
                anchorTransform *
                glm::mat4_cast(glm::quat(glm::vec3(rotation.x, rotation.y,0.f))) *
                glm::scale(glm::mat4(1), glm::vec3(graphic_->getWidth(), graphic_->getHeight(), 1.f)) *
                glm::inverse(anchorTransform);

        modelTransform_ = glm::scale(modelTransform_, glm::vec3(scale.x, scale.y, 1.f));
    }

    void GraphicHolder::setGraphic(const std::shared_ptr<Graphic> &graphic) {
        graphic_ = graphic;
    }

    std::shared_ptr<Graphic> GraphicHolder::getGraphic() const {
        return graphic_;
    }

    void GraphicHolder::setActive(bool active) {
        active_ = active;
    }

    void GraphicHolder::setGrid(int columns, int rows) {
        cellSize_ = glm::vec2(graphic_->getWidth() / columns, graphic_->getHeight() / rows);
        texScale_ = glm::vec2(cellSize_.x / graphic_->getWidth(), cellSize_.y / graphic_->getHeight());
    }

    void GraphicHolder::setCellIndex(int x, int y) {
        texOffset_ = glm::vec2((x * cellSize_.x) / graphic_->getWidth(), (y * cellSize_.y) / graphic_->getHeight());
    }

    int GraphicHolder::getCellWidth() const {
        return cellSize_.x;
    }

    int GraphicHolder::getCellHeight() const {
        return cellSize_.y;
    }

    void GraphicHolder::setAnchor(GameEngine::Internal::GraphicAnchor anchor) {
        switch (anchor){
            case GameEngine::Internal::GraphicAnchor::TOP_LEFT:
                anchor_ = glm::vec2(0.f, 0.f);
                break;
            case GameEngine::Internal::GraphicAnchor::TOP_CENTER:
                anchor_ = glm::vec2(0.5f, 0.f);
                break;
            case GameEngine::Internal::GraphicAnchor::TOP_RIGHT:
                anchor_ = glm::vec2(1.f, 0.f);
                break;
            case GameEngine::Internal::GraphicAnchor::MIDDLE_LEFT:
                anchor_ = glm::vec2(0.f, 0.5f);
                break;
            case GameEngine::Internal::GraphicAnchor::MIDDLE_CENTER:
                anchor_ = glm::vec2(0.5f, 0.5f);
                break;
            case GameEngine::Internal::GraphicAnchor::MIDDLE_RIGHT:
                anchor_ = glm::vec2(1.f, 0.5f);
                break;
            case GameEngine::Internal::GraphicAnchor::BOTTOM_LEFT:
                anchor_ = glm::vec2(0.f, 1.f);
                break;
            case GameEngine::Internal::GraphicAnchor::BOTTOM_CENTER:
                anchor_ = glm::vec2(0.5f, 1.f);
                break;
            case GameEngine::Internal::GraphicAnchor::BOTTOM_RIGHT:
                anchor_ = glm::vec2(1.f, 1.f);
                break;
        }
    }
}
}