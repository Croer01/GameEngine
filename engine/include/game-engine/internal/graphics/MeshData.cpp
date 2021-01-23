//
// Created by adria on 21/04/2019.
//

#include <GL/glew.h>
#include "MeshData.hpp"
#include <game-engine/internal/utils.hpp>

namespace GameEngine {
    namespace Internal {

        MeshData::MeshData(const std::vector<float> &verticesAndUv, const std::vector<unsigned int> &indices)
                : verticesAndUV_(verticesAndUv),
                  indices_(indices) {
            glGenVertexArrays(1, &VAO);

            //  Set vertex data
            glBindVertexArray(VAO);
            CheckGlError();

            //Create VBO
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            // the vertices and uvs are set in the same array ( 3 vertex coords + 2 uv coords)
            //TODO: review GL_STREAM_DRAW option
            glBufferData(GL_ARRAY_BUFFER, verticesAndUV_.size() * sizeof(GLfloat), &verticesAndUV_.front(), GL_STATIC_DRAW);

            //Create IBO
            if(!indices_.empty()) {
                glGenBuffers(1, &ibo);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint), &indices_.front(),
                             GL_STATIC_DRAW);
            } else{
                ibo = -1;
            }

            glBindVertexArray(0);
            CheckGlError();
        }

        MeshData::~MeshData() {
            if(ibo != -1)
                glDeleteBuffers(1, &ibo);

            glDeleteBuffers(1, &vbo);
            glDeleteVertexArrays(1, &VAO);
            verticesAndUV_.clear();
            indices_.clear();
            VAO = -1;
            ibo = -1;
            vbo = -1;
        }

        void MeshData::draw(const std::shared_ptr<Shader> &shader) const {
            glBindVertexArray(VAO);
            CheckGlError();

            shader->setAttribute(Shader::Attributes::Vertex, vbo);
            shader->setAttribute(Shader::Attributes::UV, vbo);
            if(ibo != -1)
                shader->setAttribute(Shader::Attributes::Indices, ibo);
        }

    }
}