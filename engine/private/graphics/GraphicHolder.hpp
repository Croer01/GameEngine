//
// Created by adria on 27/10/2018.
//

#ifndef SPACEINVADERS_GRAPHICHOLDER_HPP
#define SPACEINVADERS_GRAPHICHOLDER_HPP


#include <memory>
#include <game-engine/api.hpp>
#include "Graphic.hpp"

namespace GameEngine {
namespace Internal {

    class GraphicsEngine;

    enum class GraphicAnchor {
        TOP_LEFT,
        TOP_CENTER,
        TOP_RIGHT,
        MIDDLE_LEFT,
        MIDDLE_CENTER,
        MIDDLE_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_CENTER,
        BOTTOM_RIGHT,
    };

    GraphicAnchor parseStringToGraphicAnchor(const std::string &anchorName);

    glm::vec2 parseGraphicPositionToVec2D(GameEngine::Internal::GraphicAnchor anchor);

    class GraphicHolder {
        bool active_;
        std::shared_ptr<Graphic> graphic_;
        glm::mat4 modelTransform_;
        glm::vec2 cellSize_;
        glm::vec2 texOffset_;
        glm::vec2 texScale_;
        glm::vec2 anchor_;
        glm::vec4 tintColor_;
        GraphicsEngine *engine_;

    public:
        explicit GraphicHolder(const std::shared_ptr<Graphic> &graphic);

        void draw(const std::shared_ptr<Shader> &shader);

        void setModelTransform(const Vec2D &position, float rotation, const Vec2D &scale);

        void setGraphic(const std::shared_ptr<Graphic> &graphic);

        std::shared_ptr<Graphic> getGraphic() const;

        void setGrid(int columns, int rows);

        void setCellIndex(int x, int y);

        int getCellWidth() const;

        int getCellHeight() const;

        void setActive(bool active);

        bool getActive() const;

        void setAnchor(GameEngine::Internal::GraphicAnchor anchor);

        void setTintColor(geColor tint);

        void setEngine(GraphicsEngine *engine);
    };
}
}

#endif //SPACEINVADERS_GRAPHICHOLDER_HPP
