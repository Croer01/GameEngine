//
// Created by adria on 14/10/2018.
//

#ifndef SPACEINVADERS_INPUTMANAGER_HPP
#define SPACEINVADERS_INPUTMANAGER_HPP


#include <map>
#include <game-engine/api.hpp>
#include <game-engine/KeyCodes.hpp>
#include <game-engine/events/Subject.hpp>
#include <memory>
#include <SDL2/SDL_events.h>

namespace GameEngine {

class geScreen;

enum class InputTextSubjectEvent
{
    INPUT,
    ERASE
};

class InputTextSubject;
typedef std::shared_ptr<InputTextSubject> InputTextSubjectRef;
class InputTextSubject : public Subject<InputTextSubjectEvent, const char *>
{

};
class InputManager
{
    enum class InputState
    {
        NONE, UP, DOWN, PRESSED
    };
    bool quit_;
    Vec2D mousePosition_;
    std::map<MouseButton, InputState> mouseState;
    std::map<KeyCode, InputState> keyboardState;
    InputTextSubjectRef inputSubject_;
    std::vector<SDL_Event> events_;
    geScreen *screen_;

    void reset();
    void processEvent(SDL_Event event);

public:
    explicit InputManager(geScreen *screen);

    bool isQuitDown();

    bool isKeyDown(KeyCode keyCode);

    bool isKeyPressed(KeyCode keyCode);

    bool isKeyUp(KeyCode keyCode);

    bool isMouseButtonDown(MouseButton mouseButton);

    bool isMouseButtonUp(MouseButton mouseButton);

    bool isMouseButtonPressed(MouseButton mouseButton);

    Vec2D getMousePosition();

    void update();
    void addEvent(SDL_Event event);

    void startRecordingTextInput(const InputTextSubjectRef &subject);
    void stopRecordingTextInput();
};
}

#endif //SPACEINVADERS_INPUTMANAGER_HPP
