//
// Created by adria on 27/10/2018.
//

#include "GraphicsEngine.hpp"
#include "GraphicHolder.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
namespace GameEngine {
namespace Internal {
    GraphicHolder::GraphicHolder(const std::shared_ptr<Graphic> &graphic) :
            graphic_(graphic),
            modelTransform_(glm::mat4(1)),
            active_(true),
            texOffset_(0),
            texScale_(1),
            cellSize_(-1),
            anchor_(glm::vec2(0.f)),
            tintColor_(glm::vec4(1.f))
            {
    }

    void GraphicHolder::draw(const std::shared_ptr<Shader> &shader) {
        if (!active_)
            return;
        shader->setUniform("transform", modelTransform_);
        shader->setUniform("TexOffest", texOffset_);
        shader->setUniform("TexCoordScale", texScale_);
        shader->setUniform("Color", tintColor_);
        graphic_->draw(shader);
    }

    void
    GraphicHolder::setModelTransform(const Vec2D &position, const Vec2D &rotation, const Vec2D &scale) {
        glm::vec3 desiredPosition = glm::vec3(position.x,position.y,0.f);
        glm::mat4 scaleTransform;
        if(cellSize_.x == -1 && cellSize_.y == -1)
            scaleTransform = glm::scale(glm::mat4(1), glm::vec3(graphic_->getWidth(), graphic_->getHeight(), 1.f));
        else
            scaleTransform = glm::scale(glm::mat4(1), glm::vec3(cellSize_.x, cellSize_.y, 1.f));

        if (GraphicsEngine::GetInstance().isPixelPerfect())
            desiredPosition = glm::round(desiredPosition);

        glm::mat4 anchorTransform = glm::translate(glm::mat4(1), glm::vec3(anchor_, 0.f));

        //remember the order of matrix multiplication is from right to left
        modelTransform_ = glm::translate(glm::mat4(1), desiredPosition) *
                anchorTransform *
                glm::mat4_cast(glm::quat(glm::vec3(rotation.x, rotation.y,0.f))) *
                scaleTransform *
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

    bool GraphicHolder::getActive() const
    {
        return active_;
    }

    void GraphicHolder::setGrid(int columns, int rows)
    {
        assert(columns >= 0);
        assert(rows >= 0);
        // Always must be at least 1 column and 1 row, aka the full image
        int effectiveColumns = columns == 0? 1 : columns;
        int effectiveRows = rows == 0? 1 : rows;

        cellSize_ = glm::vec2(graphic_->getWidth() / effectiveColumns, graphic_->getHeight() / effectiveRows);
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
        anchor_ = parseGraphicPositionToVec2D(anchor);
    }

void GraphicHolder::setTintColor(geColor tint)
{
   tintColor_ = glm::vec4(tint.r,tint.g,tint.b,1.f);
}

GraphicAnchor parseStringToGraphicAnchor(const std::string &anchorName) {
        GameEngine::Internal::GraphicAnchor graphicAnchor;

        if(anchorName == "TOP_LEFT")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::TOP_LEFT;
        else if(anchorName == "TOP_CENTER")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::TOP_CENTER;
        else if(anchorName == "TOP_RIGHT")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::TOP_RIGHT;
        else if(anchorName == "MIDDLE_LEFT")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::MIDDLE_LEFT;
        else if(anchorName == "MIDDLE_CENTER")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::MIDDLE_CENTER;
        else if(anchorName == "MIDDLE_RIGHT")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::MIDDLE_RIGHT;
        else if(anchorName == "BOTTOM_LEFT")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::BOTTOM_LEFT;
        else if(anchorName == "BOTTOM_CENTER")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::BOTTOM_CENTER;
        else if(anchorName == "BOTTOM_RIGHT")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::BOTTOM_RIGHT;
        else
            graphicAnchor = GameEngine::Internal::GraphicAnchor::TOP_LEFT;

        return graphicAnchor;
    }

    glm::vec2 parseGraphicPositionToVec2D(GameEngine::Internal::GraphicAnchor anchor) {
        glm::vec2 position;
        switch (anchor){
            case GameEngine::Internal::GraphicAnchor::TOP_LEFT:
                position = glm::vec2(0.f, 0.f);
                break;
            case GameEngine::Internal::GraphicAnchor::TOP_CENTER:
                position = glm::vec2(0.5f, 0.f);
                break;
            case GameEngine::Internal::GraphicAnchor::TOP_RIGHT:
                position = glm::vec2(1.f, 0.f);
                break;
            case GameEngine::Internal::GraphicAnchor::MIDDLE_LEFT:
                position = glm::vec2(0.f, 0.5f);
                break;
            case GameEngine::Internal::GraphicAnchor::MIDDLE_CENTER:
                position = glm::vec2(0.5f, 0.5f);
                break;
            case GameEngine::Internal::GraphicAnchor::MIDDLE_RIGHT:
                position = glm::vec2(1.f, 0.5f);
                break;
            case GameEngine::Internal::GraphicAnchor::BOTTOM_LEFT:
                position = glm::vec2(0.f, 1.f);
                break;
            case GameEngine::Internal::GraphicAnchor::BOTTOM_CENTER:
                position = glm::vec2(0.5f, 1.f);
                break;
            case GameEngine::Internal::GraphicAnchor::BOTTOM_RIGHT:
                position = glm::vec2(1.f, 1.f);
                break;
        }

        return position;
    }
}
}
