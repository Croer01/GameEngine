//
// Created by adria on 22/09/2018.
//

#ifndef SPACEINVADERS_GAME_HPP
#define SPACEINVADERS_GAME_HPP


#include <game-engine/geGame.hpp>
#include <memory>
#include "Singleton.hpp"
#include "Screen.hpp"
#include <string>

namespace GameEngine {
namespace Internal {
    class Game : public Singleton<Game> {
        bool running_;
        geGame *context_;

        std::unique_ptr<Screen> screen_;

        void initPhysics(const std::string &configFilePath);

    protected:
        void onCreateInstance() override;

    public:
        void init(const std::string &configRoot);

        void loop();

        void shutdown();

        void context(geGame *context);
        geGame &context() const;
        Screen &screen() const;
    };
}
}

#endif //SPACEINVADERS_GAME_HPP
