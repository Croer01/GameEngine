//
// Created by adria on 12/10/2018.
//

#ifndef SPACEINVADERS_GRAPHICSENGINE_HPP
#define SPACEINVADERS_GRAPHICSENGINE_HPP


#include <vector>
#include <memory>
#include "GraphicHolder.hpp"
#include "Shader.h"
#include "../Singleton.hpp"
#include "../Screen.hpp"
#include "font/Text.hpp"

class GraphicsEngine : public Singleton<GraphicsEngine>{
    struct InternalMesh {
        unsigned int VAO;
        unsigned int vbo;
        unsigned int ibo;
    };

    std::vector<std::shared_ptr<GraphicHolder>> graphics_;
    std::vector<std::shared_ptr<Text>> texts_;
    InternalMesh mesh_;
    std::shared_ptr<Shader> spriteShader_;
    std::shared_ptr<Shader> textShader_;
    glm::mat4 projMatrix_;
    bool pixelPerfect_;
public:
    ~GraphicsEngine() override;
    void init(const Screen &screen);
    void draw();
    void registerGraphic(const std::shared_ptr<GraphicHolder> &graphic);
    bool isPixelPerfect() const;
};


#endif //SPACEINVADERS_GRAPHICSENGINE_HPP
