//
// Created by adria on 15/03/2019.
//

#ifndef SPACEINVADERS_GESCREEN_HPP
#define SPACEINVADERS_GESCREEN_HPP

#include <game-engine/api.hpp>
#include <string>

namespace GameEngine {
    class PUBLICAPI geScreen {
    public:
        virtual ~geScreen() {};
        virtual std::string title() const = 0;
        virtual void title(const std::string &value) = 0;

        virtual int windowWidth() const = 0;
        virtual void windowWidth(int value) = 0;

        virtual int windowHeight() const = 0;
        virtual void windowHeight(int value) = 0;

        virtual int virtualWidth() const = 0;
        virtual void virtualWidth(int value) = 0;

        virtual int virtualHeight() const = 0;
        virtual void virtualHeight(int value) = 0;

        virtual geColor backgroundColor() const = 0;
        virtual void backgroundColor(geColor value) = 0;

        virtual bool pixelPerfect() const = 0;
        virtual void pixelPerfect(bool value) = 0;

        virtual bool resizable() const = 0;
        virtual void resizable(bool value) = 0;

        virtual Vec2D transformWindowToScreen(const Vec2D &position) = 0;
    };
}

#endif //SPACEINVADERS_GESCREEN_HPP
