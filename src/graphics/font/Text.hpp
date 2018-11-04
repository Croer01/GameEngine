//
// Created by adria on 03/11/2018.
//

#ifndef SPACEINVADERS_TEXT_HPP
#define SPACEINVADERS_TEXT_HPP


#include <string>
#include <vector>
#include <array>
#include "Font.hpp"
#include "../Shader.h"


struct TextDef {
    std::vector<std::array<float, 20>> verticesByCharacter;
    std::vector<unsigned int> textureIds;
};

class Text {
    TextDef textDef_;
    glm::mat4 modelTransform_;

    unsigned int VAO;
    unsigned int vbo;
    unsigned int ibo;

public:
    explicit Text(const TextDef &textDef);
    ~Text();
    void draw(const std::shared_ptr<Shader> &shader);
    void setModelTransform(const glm::vec3 &position,const glm::vec3 &rotation ,const glm::vec3 &scale);
};


#endif //SPACEINVADERS_TEXT_HPP
