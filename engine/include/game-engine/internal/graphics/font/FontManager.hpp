//
// Created by adria on 03/11/2018.
//

#ifndef SPACEINVADERS_FONTMANAGER_HPP
#define SPACEINVADERS_FONTMANAGER_HPP


#include <memory>
#include <vector>
#include "Font.hpp"
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

namespace GameEngine {
namespace Internal {
    class FontManager {
        FT_Library ftLibrary_;
        std::map<std::string, std::shared_ptr<Font>> fonts_;
    public:
        FontManager();

        ~FontManager();

        std::shared_ptr<Font> getFont(const std::string &fontPath, int pixelsSize);
        std::shared_ptr<Font> getDefaultFont(int pixelsSize);
    };
}
}

#endif //SPACEINVADERS_FONTMANAGER_HPP
