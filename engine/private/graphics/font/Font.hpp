//
// Created by adria on 03/11/2018.
//

#ifndef SPACEINVADERS_FONT_HPP
#define SPACEINVADERS_FONT_HPP


#include <string>
#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>
// FreeType
#include <ft2build.h>
#include <memory>
#include FT_FREETYPE_H
namespace GameEngine {
namespace Internal {
    struct TextDef;

    class Text;

    struct FontCharacter {
        GLuint TextureID;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        GLuint Advance;
    };

    class Font : public std::enable_shared_from_this<Font> {
        std::map<FT_UInt, FontCharacter> characters_;
        int lineSpacing_;
        int pixelHeight_;
        FT_Face face_;
        void registerCharacter(FT_UInt glyph);
    public:
        Font(const FT_Library &ftLibrary, const std::string &fontPath, unsigned int pixelsSize);

        ~Font();

        std::shared_ptr<Text> createText(const std::string &text);

        const TextDef createTextDef(const std::string &text);
    };
}
}

#endif //SPACEINVADERS_FONT_HPP