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
    GraphicHolder::setModelTransform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale) {
        //remember the order of matrix multiplication is from right to left
        if (GraphicsEngine::GetInstance().isPixelPerfect())
            modelTransform_ = glm::translate(glm::mat4(1), glm::round(position)) * glm::mat4_cast(glm::quat(rotation)) *
                              glm::scale(glm::mat4(1), scale);
        else
            modelTransform_ = glm::translate(glm::mat4(1), position) * glm::mat4_cast(glm::quat(rotation)) *
                              glm::scale(glm::mat4(1), scale);
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
}
}
