//
// Created by adria on 22/09/2018.
//

#ifndef SPACEINVADERS_GAME_HPP
#define SPACEINVADERS_GAME_HPP


#include <SDL2/SDL_video.h>
#include <memory>
#include "Singleton.hpp"

class Game : public Singleton<Game> {
    int width_;
    int height_;
    std::shared_ptr<SDL_Window> mainWindow_;
    SDL_GLContext mainContext_;

    void initSDLWindow();

    void makeCurrentContext();
public:
    void init();
    void loop();
};


#endif //SPACEINVADERS_GAME_HPP
