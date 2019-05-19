//
// Created by adria on 19/04/2019.
//

#ifndef ZOMBIEWAVE_GRAPHICGEOMETRY_HPP
#define ZOMBIEWAVE_GRAPHICGEOMETRY_HPP

#include <vector>
#include <game-engine/api.hpp>
#include <memory>
#include "MeshData.hpp"
#include "Graphic.hpp"

namespace GameEngine {
    namespace Internal {
        class GraphicGeometry : public Graphic {
            std::vector<Vec2D> points_;
            std::shared_ptr<MeshData> mesh_;
            int width_;
            int height_;
            GLuint textureID_;

            void generateMesh();
        public:
            explicit GraphicGeometry(const std::vector<Vec2D> &points);
            virtual ~GraphicGeometry();

            void draw(const std::shared_ptr<Shader> &shader) const override;

            int getWidth() const override;

            int getHeight() const override;
        };
    }
}


#endif //ZOMBIEWAVE_GRAPHICGEOMETRY_HPP
