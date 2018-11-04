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
    int width_;
    int height_;
public:
    explicit Graphic(std::string filename);
    ~Graphic();
    void draw(std::shared_ptr<Shader> shader);

    int getWidth() const;
    int getHeight() const;
};


#endif //SPACEINVADERS_GRAPHIC_HPP
