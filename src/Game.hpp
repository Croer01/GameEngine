//
// Created by adria on 22/09/2018.
//

#ifndef SPACEINVADERS_GAME_HPP
#define SPACEINVADERS_GAME_HPP


#include "Singleton.hpp"

class Game : public Singleton<Game> {
public:
    void init();
};


#endif //SPACEINVADERS_GAME_HPP
