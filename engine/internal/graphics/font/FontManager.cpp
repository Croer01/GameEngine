//
// Created by adria on 03/11/2018.
//

#include <stdexcept>
#include <filesystem>
#include <game-engine/internal/graphics/font/FontManager.hpp>

namespace GameEngine {
namespace Internal {
    FontManager::FontManager(const std::string &dataPath) : dataPath_(dataPath) {
        // All functions return a value different than 0 whenever an error occurred
        if (FT_Init_FreeType(&ftLibrary_))
            throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
    }

    std::shared_ptr<Font> FontManager::getFont(const std::string &fontPath, int pixelsSize) {
        std::string fontName = fontPath + "_" + std::to_string(pixelsSize);
        auto it = fonts_.find(fontName);

        if (it != fonts_.end())
            return it->second;

        std::filesystem::path fullPath(dataPath_);
        fullPath.append(fontPath).make_preferred();
        std::shared_ptr<Font> font = std::make_shared<Font>(ftLibrary_, fullPath.string(), static_cast<unsigned int>(pixelsSize));

        fonts_.insert(std::pair<std::string, std::shared_ptr<Font>>(fontName, font));

        return font;
    }

std::shared_ptr<Font> FontManager::getDefaultFont(int pixelsSize) {
    std::string fontName = "DEFAULT_FONT_" + std::to_string(pixelsSize);
    auto it = fonts_.find(fontName);

    if (it != fonts_.end())
        return it->second;

    std::shared_ptr<Font> font = std::make_shared<Font>(ftLibrary_, static_cast<unsigned int>(pixelsSize));

    fonts_.insert(std::pair<std::string, std::shared_ptr<Font>>(fontName, font));

    return font;
}

FontManager::~FontManager() {
    fonts_.clear();
        FT_Done_FreeType(ftLibrary_);
    }
}
}