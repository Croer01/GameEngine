//
// Created by adria on 28/10/2018.
//

#ifndef SPACEINVADERS_SCREEN_HPP
#define SPACEINVADERS_SCREEN_HPP


#include <string>
#include <glm/vec3.hpp>
namespace GameEngine {
namespace Internal {
    class Screen {
        //The size of the window or screen if window is fullscreen
        int deviceWidth_;
        int deviceHeight_;

        //dimensions of the virtual screen. This is used to preserve always the same size independently of the device resolution
        int virtualWidth_;
        int virtualHeight_;

        //TODO: create a list of possible configurations to set up virtual size and add a sender to will be able the Game aware of it.
        int calculatedX_;
        int calculatedY_;
        int calculatedWidth_;
        int calculatedHeight_;

        std::string title_;
        glm::vec3 background_;
        bool pixelPerfect_;
    public:
        explicit Screen(const std::string &filePath);

        std::string getTitle() const;

        int getWindowWidth() const;

        int getWindowHeight() const;

        int getVirtualWidth() const;

        int getVirtualHeight() const;

        int getCalculatedX() const;

        int getCalculatedY() const;

        int getCalculatedWidth() const;

        int getCalculatedHeight() const;

        glm::vec3 getBackgroundColor() const;

        bool isPixelPerfect() const;
    };
}
}
#endif //SPACEINVADERS_SCREEN_HPP
