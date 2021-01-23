//
// Created by adria on 19/04/2019.
//

#include <algorithm>
#include "GraphicGeometry.hpp"
#include <game-engine/internal/utils.hpp>

namespace GameEngine {
    namespace Internal {
        GraphicGeometry::GraphicGeometry(const std::vector<GameEngine::Vec2D> &points)
                : points_(points) {
            const auto minMaxX = std::minmax_element(points_.begin(), points_.end(),[](const Vec2D &a, const Vec2D &b){
                return a.x < b.x;
            });

            const auto minMaxY = std::minmax_element(points_.begin(), points_.end(),[](const Vec2D &a, const Vec2D &b){
                return a.y < b.y;
            });
            width_ = std::ceil(std::abs((*minMaxX.first).x) + std::abs((*minMaxX.second).x));
            height_ = std::ceil(std::abs((*minMaxY.first).y) + std::abs((*minMaxY.second).y));
            fillGeometry_ = true;
        }

    void GraphicGeometry::internalInitializeGl()
    {
        generateMesh();
        GLubyte white[4] = {255, 255, 255, 255};
        glGenTextures(1, &textureID_);
        glBindTexture(GL_TEXTURE_2D, textureID_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white);
    }

    void GraphicGeometry::generateMesh() {

            //generate vertices and uvs
            std::vector<float> vertices; // vertex + uv
            vertices.reserve(points_.size()*5);
            for(auto & point : points_){
                // vertex
                vertices.push_back(point.x/ width_);
                vertices.push_back(point.y/ height_);
                vertices.push_back(0.f);

                // uv
                vertices.push_back(point.x / width_);
                vertices.push_back(point.y / height_);
            }

            //generate indices
            std::vector<unsigned int> indices;
            indices.reserve((points_.size() -1)*3);

            // skip the first point
            for(size_t i = 1; i < points_.size()-1; i++){
                // all the triangles start from the first point ( aka index 0)
                indices.push_back(0);
                indices.push_back(i);
                indices.push_back(i + 1);
            }

            mesh_ = std::make_unique<MeshData>(vertices, indices);
        }

        void GraphicGeometry::draw(const std::shared_ptr<Shader> &shader) const {
            glBindTexture(GL_TEXTURE_2D, textureID_);
            mesh_->draw(shader);
            if(!fillGeometry_)
            {
                shader->setElementMode(GL_LINE_LOOP);
                shader->draw();
                shader->setElementMode(GL_TRIANGLES);
            }
            else
                shader->draw();

            CheckGlError();
        }

        int GraphicGeometry::getWidth() const {
            return width_;
        }

        int GraphicGeometry::getHeight() const {
            return height_;
        }

        GraphicGeometry::~GraphicGeometry() {
            glDeleteTextures(1, &textureID_);
        }

        void GraphicGeometry::drawFillGeometry(bool fill)
        {
            fillGeometry_ = fill;
        }
    }
}
