//
// Created by adria on 12/10/2018.
//

#ifndef SPACEINVADERS_GRAPHIC_HPP
#define SPACEINVADERS_GRAPHIC_HPP


#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Shader.h"

class Graphic {
    unsigned int textureID_;
    glm::mat4 modelTransform_;
public:
    explicit Graphic(std::string filename);
    ~Graphic();
    void draw(std::shared_ptr<Shader> shader);
    void setModelTransform(const glm::mat4 &transform);
};


#endif //SPACEINVADERS_GRAPHIC_HPP
