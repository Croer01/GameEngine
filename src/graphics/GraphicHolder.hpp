//
// Created by adria on 27/10/2018.
//

#ifndef SPACEINVADERS_GRAPHICHOLDER_HPP
#define SPACEINVADERS_GRAPHICHOLDER_HPP


#include <memory>
#include "Graphic.hpp"
namespace GameEngine {
namespace Internal {
    class GraphicHolder {
        bool active_;
        std::shared_ptr<Graphic> graphic_;
        glm::mat4 modelTransform_;
        glm::vec2 cellSize_;
        glm::vec2 texOffset_;
        glm::vec2 texScale_;

    public:
        explicit GraphicHolder(const std::shared_ptr<Graphic> &graphic);

        void draw(const std::shared_ptr<Shader> &shader);

        void setModelTransform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale);

        void setGraphic(const std::shared_ptr<Graphic> &graphic);

        std::shared_ptr<Graphic> getGraphic() const;

        void setGrid(int columns, int rows);

        void setCellIndex(int x, int y);

        int getCellWidth() const;

        int getCellHeight() const;

        void setActive(bool active);
    };
}
}

#endif //SPACEINVADERS_GRAPHICHOLDER_HPP
