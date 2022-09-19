//
// Created by adria on 03/11/2018.
//

#ifndef SPACEINVADERS_FONTMANAGER_HPP
#define SPACEINVADERS_FONTMANAGER_HPP

#include <game-engine/api.hpp>
#include <memory>
#include <vector>
#include <game-engine/internal/graphics/font/Font.hpp>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

namespace GameEngine {
namespace Internal {
    class PUBLICAPI FontManager {
        FT_Library ftLibrary_;
        std::map<std::string, std::shared_ptr<Font>> fonts_;
        std::string dataPath_;
    public:
        explicit FontManager(const std::string &dataPath);

        ~FontManager();

        std::shared_ptr<Font> getFont(const std::string &fontPath, int pixelsSize);
        std::shared_ptr<Font> getDefaultFont(int pixelsSize);
    };
}
}

#endif //SPACEINVADERS_FONTMANAGER_HPP
