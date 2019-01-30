//
// Created by adria on 25/01/2019.
//

#ifndef SPACEINVADERS_GEGAME_HPP
#define SPACEINVADERS_GEGAME_HPP

#include <game-engine/api.hpp>
#include <game-engine/geGameObject.hpp>
#include <game-engine/geEnvironment.hpp>
#include <string>

namespace GameEngine {
    class PUBLICAPI geGame {
        geEnvironment environment_;
    public:
        int start();
        void shutdown();
        geGameObjectRef createObject(const std::string &name);
        geGameObjectRef createFromPrototype(const std::string &prototype);
        void configEnvironment(const geEnvironment &environment);
    };
}

#endif //SPACEINVADERS_GEGAME_HPP
