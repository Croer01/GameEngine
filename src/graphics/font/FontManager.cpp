//
// Created by adria on 03/11/2018.
//

#include <stdexcept>
#include "FontManager.hpp"


FontManager::FontManager() {
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ftLibrary_))
        throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
}

std::shared_ptr<Font> FontManager::getFont(const std::string &fontPath, int pixelsSize) {
    std::string fontName = fontPath + "_" + std::to_string(pixelsSize);
    auto it = fonts_.find(fontName);

    if(it != fonts_.end())
        return it->second;

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ftLibrary_, fontPath.c_str(), 0, &face))
        throw std::runtime_error("ERROR::FREETYPE: Failed to load font");

    std::shared_ptr<Font> font = std::make_shared<Font>(face,static_cast<unsigned int>(pixelsSize));
    FT_Done_Face(face);

    fonts_.insert(std::pair<std::string, std::shared_ptr<Font>>(fontName, font));

    return font;
}

FontManager::~FontManager() {
    FT_Done_FreeType(ftLibrary_);
}
