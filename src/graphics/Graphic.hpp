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

public:
    explicit Graphic(std::string filename);
    ~Graphic();
    void draw(std::shared_ptr<Shader> shader);
    glm::mat4 getTransform();
};


#endif //SPACEINVADERS_GRAPHIC_HPP
