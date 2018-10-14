//
// Created by adria on 12/10/2018.
//

#ifndef SPACEINVADERS_GRAPHICSENGINE_HPP
#define SPACEINVADERS_GRAPHICSENGINE_HPP


#include <vector>
#include <memory>
#include "Graphic.hpp"
#include "Shader.h"

class GraphicsEngine {
    struct InternalMesh {
        unsigned int VAO;
        unsigned int vbo;
        unsigned int ibo;
    };

    std::vector<std::shared_ptr<Graphic>> graphics_;
    InternalMesh mesh_;
    std::shared_ptr<Shader> spriteShader_;
public:
    ~GraphicsEngine();
    void init();
    void draw();

};


#endif //SPACEINVADERS_GRAPHICSENGINE_HPP
