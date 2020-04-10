//
// Created by adria on 03/11/2018.
//

#ifndef SPACEINVADERS_TEXT_HPP
#define SPACEINVADERS_TEXT_HPP


#include <string>
#include <vector>
#include <array>
#include <game-engine/api.hpp>
#include "Font.hpp"
#include "../Shader.h"

namespace GameEngine {
namespace Internal {

    class GraphicsEngine;

    struct TextParameters {
        std::string text;
        std::string fontName;
        unsigned int fontSize;
        bool visible;
    };

    struct CharDef
    {
        bool isBreakLine;
        std::array<float, 20> vertices;
        unsigned int textureId;

        CharDef(bool isBreakLine, const std::array<float, 20> &vertices, unsigned int textureId) :
            isBreakLine(isBreakLine), vertices(vertices), textureId(textureId)
        {}
    };

    struct TextDef
    {
        std::vector<CharDef> characters;
        int width;
        int height;
        int lineHeight;
    };

    class Text {
        TextDef textDef_;
        glm::mat4 modelTransform_;
        bool active_;
        glm::vec4 tintColor_;
        std::shared_ptr<Font> font_;
        GraphicsEngine *engine_;
        unsigned int VAO;
        unsigned int vbo;
        unsigned int ibo;

    public:
        Text(const std::shared_ptr<Font> &font, const TextDef &textDef);

        ~Text();

        void draw(const std::shared_ptr<Shader> &shader);

        void setText(const std::string &text);

        void setModelTransform(const Vec2D &position, float rotation, const Vec2D &scale);

        void setActive(bool active);

        Vec2D getPixelPosFromTextIndex(int index) const;
        Vec2D getPixelSizeFromTextIndex(int index) const;
        int width() const;
        int height() const;
        size_t size() const;
        void setTintColor(geColor tint);

        void setEngine(GraphicsEngine *engine);
    };
}
}

#endif //SPACEINVADERS_TEXT_HPP
