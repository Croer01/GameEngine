//
// Created by adria on 14/10/2018.
//

#ifndef SPACEINVADERS_INPUTMANAGER_HPP
#define SPACEINVADERS_INPUTMANAGER_HPP


#include <map>
#include <glm/vec2.hpp>
#include "Singleton.hpp"
#include <SDL2/SDL_keycode.h>
namespace GameEngine {
namespace Internal {
    struct Vector2D {
        int x;
        int y;
    };

    enum class MouseButton {
        LEFT,
        RIGHT,
        MIDDLE
    };

    typedef SDL_Keycode KeyCode;

    class InputManager : public Singleton<InputManager> {
        enum class InputState {
            NONE, UP, DOWN, PRESSED
        };
        bool quit_;
        Vector2D mousePosition_;
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

        Vector2D getMousePosition();

        void update();
    };
}
}

#endif //SPACEINVADERS_INPUTMANAGER_HPP
