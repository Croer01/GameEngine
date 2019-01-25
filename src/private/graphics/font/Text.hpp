//
// Created by adria on 03/11/2018.
//

#ifndef SPACEINVADERS_TEXT_HPP
#define SPACEINVADERS_TEXT_HPP


#include <string>
#include <vector>
#include <array>
#include <api.hpp>
#include "Font.hpp"
#include "../Shader.h"

namespace GameEngine {
namespace Internal {
    struct TextDef {
        std::vector<std::array<float, 20>> verticesByCharacter;
        std::vector<unsigned int> textureIds;
    };

    class Text {
        TextDef textDef_;
        glm::mat4 modelTransform_;
        bool active_;
        std::shared_ptr<Font> font_;
        unsigned int VAO;
        unsigned int vbo;
        unsigned int ibo;

    public:
        Text(const std::shared_ptr<Font> &font, const TextDef &textDef);

        ~Text();

        void draw(const std::shared_ptr<Shader> &shader);

        void setText(const std::string &text);

        void setModelTransform(const Vec2D &position, const Vec2D &rotation, const Vec2D &scale);

        void setActive(bool active);
    };
}
}

#endif //SPACEINVADERS_TEXT_HPP
