//
// Created by adria on 21/04/2019.
//

#ifndef ZOMBIEWAVE_MESHDATA_HPP
#define ZOMBIEWAVE_MESHDATA_HPP

#include <vector>
#include <game-engine/internal/graphics/Shader.hpp>

namespace GameEngine {
    namespace Internal {

        class MeshData {
            std::vector<float> verticesAndUV_;
            std::vector<unsigned int> indices_;
            unsigned int VAO;
            unsigned int vbo;
            unsigned int ibo;
        public:
            MeshData(const std::vector<float> &verticesAndUv, const std::vector<unsigned int> &indices);

            virtual ~MeshData();

            void draw(const std::shared_ptr<Shader> &shader) const;
        };

    }
}

#endif //ZOMBIEWAVE_MESHDATA_HPP
