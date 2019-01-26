//
// Created by adria on 25/01/2019.
//

#ifndef SPACEINVADERS_GEGAME_HPP
#define SPACEINVADERS_GEGAME_HPP

#include <game-engine/api.hpp>
#include <game-engine/geGameObject.hpp>
#include <string>

namespace GameEngine {
    class PUBLICAPI geGame{
        public:
            geGameObjectRef createObject(const std::string &name);
    };
}

#endif //SPACEINVADERS_GEGAME_HPP
