//
// Created by adria on 14/10/2018.
//

#ifndef SPACEINVADERS_INPUTMANAGER_HPP
#define SPACEINVADERS_INPUTMANAGER_HPP


#include <map>
#include "../private/Singleton.hpp"
#include <game-engine/api.hpp>
#include <game-engine/KeyCodes.hpp>

namespace GameEngine {

class InputManager : public Internal::Singleton<InputManager> {
        enum class InputState {
            NONE, UP, DOWN, PRESSED
        };
        bool quit_;
        Vec2D mousePosition_;
        std::map<MouseButton, InputState> mouseState;
        std::map<KeyCode, InputState> keyboardState;

        void reset();

    public:
        InputManager();

        bool isQuitDown();

        bool isKeyDown(KeyCode keyCode);

        bool isKeyPressed(KeyCode keyCode);

        bool isKeyUp(KeyCode keyCode);

        bool isMouseButtonDown(MouseButton mouseButton);

        bool isMouseButtonUp(MouseButton mouseButton);

        bool isMouseButtonPressed(MouseButton mouseButton);

        Vec2D getMousePosition();

        void update();
    };
}

#endif //SPACEINVADERS_INPUTMANAGER_HPP
