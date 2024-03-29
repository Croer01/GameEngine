//
// Created by adria on 14/10/2018.
//

#include <game-engine/InputManager.hpp>
#include <game-engine/internal/KeyCodeMap.hpp>
#include <iostream>
#include <game-engine/geScreen.hpp>

namespace GameEngine {

Internal::KeyMap keyMap;

InputManager::InputManager(geScreen *screen) : mousePosition_({0, 0}), quit_(false), screen_(screen)
{}


bool InputManager::isKeyDown(KeyCode keyCode)
{
    return keyboardState[keyCode] == InputState::DOWN;
}

bool InputManager::isKeyPressed(KeyCode keyCode)
{
    return keyboardState[keyCode] == InputState::PRESSED || isKeyDown(keyCode);
}

bool InputManager::isKeyUp(KeyCode keyCode)
{
    return keyboardState[keyCode] == InputState::UP;
}

bool InputManager::isMouseButtonUp(MouseButton mouseButton)
{
    return mouseState[mouseButton] == InputState::UP;
}

bool InputManager::isMouseButtonDown(MouseButton mouseButton)
{
    return mouseState[mouseButton] == InputState::DOWN;
}


bool InputManager::isMouseButtonPressed(MouseButton mouseButton)
{
    return mouseState[mouseButton] == InputState::PRESSED || isMouseButtonDown(mouseButton);
}

Vec2D InputManager::getMousePosition()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return screen_->transformWindowToScreen(Vec2D(x,y));
}

void InputManager::update()
{
    reset();
    for (auto event : events_)
    {
        if(processEvent(event))
        {
            reset();
            break;
        }
    }

    events_.clear();
}

bool InputManager::processEvent(SDL_Event event)
{
    bool skipAllEvents = false;
    switch (event.type)
    {
        case SDL_QUIT:
            quit_ = true;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                quit_ = true;
            else if(event.window.event == SDL_WINDOWEVENT_RESTORED)
                skipAllEvents = true;
            break;
        case SDL_KEYDOWN:
            if (event.key.repeat == 0)
                keyboardState[keyMap[event.key.keysym.sym]] = InputState::DOWN;
            else
                keyboardState[keyMap[event.key.keysym.sym]] = InputState::PRESSED;

            if(inputSubject_)
            {
                if(event.key.keysym.sym == SDLK_RETURN)
                {
                    std::string NewLine = "\n";
                    inputSubject_->notify(InputTextSubjectEvent::INPUT, NewLine.c_str());
                }

                if(event.key.keysym.sym == SDLK_BACKSPACE)
                {
                    inputSubject_->notify(InputTextSubjectEvent::ERASE, nullptr);
                }
            }

            break;
        case SDL_KEYUP:
            if(keyboardState[keyMap[event.key.keysym.sym]] != InputState::NONE)
                keyboardState[keyMap[event.key.keysym.sym]] = InputState::UP;
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
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
            switch (event.button.button)
            {
                case SDL_BUTTON_LEFT:
                    if(mouseState[MouseButton::LEFT] != InputState::NONE)
                        mouseState[MouseButton::LEFT] = InputState::UP;
                    break;
                case SDL_BUTTON_RIGHT:
                    if(mouseState[MouseButton::RIGHT] != InputState::NONE)
                        mouseState[MouseButton::RIGHT] = InputState::UP;
                    break;
                case SDL_BUTTON_MIDDLE:
                    if(mouseState[MouseButton::MIDDLE] != InputState::NONE)
                        mouseState[MouseButton::MIDDLE] = InputState::UP;
                    break;
                default:
                    break;
            }
            break;
        case SDL_TEXTEDITING:
            if(inputSubject_)
                inputSubject_->notify(InputTextSubjectEvent::INPUT, event.edit.text);
            break;
        case SDL_TEXTINPUT:
            if(inputSubject_)
            {
                inputSubject_->notify(InputTextSubjectEvent::INPUT, event.text.text);
            }
            break;
        default:
            break;
    }

    return skipAllEvents;
}

//internal implementations

void InputManager::reset()
{
    //reset keyboard state
    for (auto iterator : keyboardState)
    {
        if (iterator.second == InputState::DOWN || iterator.second == InputState::PRESSED)
            keyboardState[iterator.first] = InputState::PRESSED;
        else
            keyboardState[iterator.first] = InputState::NONE;
    }
    //reset mouse state
    for (auto iterator : mouseState)
    {
        if (iterator.second == InputState::DOWN || iterator.second == InputState::PRESSED)
            mouseState[iterator.first] = InputState::PRESSED;
        else
            mouseState[iterator.first] = InputState::NONE;
    }
}

bool InputManager::isQuitDown()
{
    return quit_;
}

void InputManager::startRecordingTextInput(const InputTextSubjectRef &subject)
{
    if(!subject)
        throw std::invalid_argument("subject is required to enable \"text input\" mode");
    inputSubject_ = subject;
    SDL_StartTextInput();
}

void InputManager::stopRecordingTextInput()
{
    inputSubject_ = nullptr;
    SDL_StopTextInput();
}

void InputManager::addEvent(SDL_Event event)
{
    events_.push_back(event);
}
}
