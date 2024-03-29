//
// Created by adria on 27/10/2018.
//

#ifndef GAMEENGINE_GRAPHICHOLDER_HPP
#define GAMEENGINE_GRAPHICHOLDER_HPP


#include <memory>
#include <game-engine/api.hpp>
#include <game-engine/internal/graphics/Graphic.hpp>
#include <game-engine/geGameObject.hpp>

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

    glm::vec2 parseGraphicAnchorToVec2D(GameEngine::Internal::GraphicAnchor anchor);

    class PUBLICAPI GraphicHolder {
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

        void initializeGl();

        void draw(const std::shared_ptr<Shader> &shader);

        void setModelTransform(geGameObject *gameObject);
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
        void setTintColor(geColor tint, float opacity);

        void setDisplacement(const Vec2D &displacement);

        void setEngine(GraphicsEngine *engine);
    };
}
}

#endif //GAMEENGINE_GRAPHICHOLDER_HPP
