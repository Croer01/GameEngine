//
// Created by adria on 14/10/2018.
//

#include <game-engine/InputManager.hpp>
#include "../private/KeyCodeMap.hpp"
#include <SDL2/SDL_events.h>

namespace GameEngine {

    Internal::KeyMap keyMap;

    InputManager::InputManager() : mousePosition_({0, 0}), quit_(false) {}


    bool InputManager::isKeyDown(KeyCode keyCode) {
        return keyboardState[keyCode] == InputState::DOWN;
    }

    bool InputManager::isKeyPressed(KeyCode keyCode) {
        return keyboardState[keyCode] == InputState::PRESSED || isKeyDown(keyCode);
    }

    bool InputManager::isKeyUp(KeyCode keyCode) {
        return keyboardState[keyCode] == InputState::UP;
    }

    bool InputManager::isMouseButtonUp(MouseButton mouseButton) {
        return mouseState[mouseButton] == InputState::UP;

    }

    bool InputManager::isMouseButtonDown(MouseButton mouseButton) {
        return mouseState[mouseButton] == InputState::DOWN;
    }

    Vec2D InputManager::getMousePosition() {
    int x,y;
        SDL_GetMouseState(&x, &y);
        mousePosition_.x = x;
        mousePosition_.y = y;
        return mousePosition_;
    }

    void InputManager::update() {
        reset();

        SDL_Event event;

        while (SDL_PollEvent(&event) != 0) {

            switch (event.type) {
                case SDL_QUIT:
                    quit_ = true;
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                        quit_ = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.repeat == 0)
                        keyboardState[keyMap[event.key.keysym.sym]] = InputState::DOWN;
                    else
                        keyboardState[keyMap[event.key.keysym.sym]] = InputState::PRESSED;
                    break;
                case SDL_KEYUP:
                    keyboardState[keyMap[event.key.keysym.sym]] = InputState::UP;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    switch (event.button.button) {
                        case SDL_BUTTON_LEFT:
                            mouseState[MouseButton::LEFT] = InputState::DOWN;
                            break;
                        case SDL_BUTTON_RIGHT:
                            mouseState[MouseButton::RIGHT] = InputState::DOWN;
                            break;
                        case SDL_BUTTON_MIDDLE:
                            mouseState[MouseButton::MIDDLE] = InputState::DOWN;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    switch (event.button.button) {
                        case SDL_BUTTON_LEFT:
                            mouseState[MouseButton::LEFT] = InputState::UP;
                            break;
                        case SDL_BUTTON_RIGHT:
                            mouseState[MouseButton::RIGHT] = InputState::UP;
                            break;
                        case SDL_BUTTON_MIDDLE:
                            mouseState[MouseButton::MIDDLE] = InputState::UP;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    }

//private implementations

        void InputManager::reset() {
            //reset keyboard state
            for (auto iterator : keyboardState) {

                if (iterator.second == InputState::DOWN || iterator.second == InputState::PRESSED)
                    keyboardState[iterator.first] = InputState::PRESSED;
                else
                    keyboardState[iterator.first] = InputState::NONE;
            }
            //reset mouse state
            for (auto iterator : mouseState) {
                mouseState[iterator.first] = InputState::NONE;
            }
        }

        bool InputManager::isQuitDown() {
            return quit_;
        }
}
