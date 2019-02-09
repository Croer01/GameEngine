//
// Created by adria on 22/09/2018.
//

#ifndef SPACEINVADERS_GAME_HPP
#define SPACEINVADERS_GAME_HPP


#include <SDL2/SDL_video.h>
#include <memory>
#include "Singleton.hpp"
#include "Screen.hpp"
#include <string>
namespace GameEngine {
namespace Internal {
    class Game : public Singleton<Game> {
        std::shared_ptr<SDL_Window> mainWindow_;
        SDL_GLContext mainContext_;
        bool running_;

        std::unique_ptr<Screen> screen_;

        void initSDLWindow();

        void initPhysics(const std::string &configFilePath);

        void makeCurrentContext();

    protected:
        void onCreateInstance() override;
    public:
        void init(const std::string &configRoot);

        void loop();

        void shutdown();
    };
}
}

#endif //SPACEINVADERS_GAME_HPP
