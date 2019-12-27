#include <iostream>
#include <game-engine/geEnvironment.hpp>
#include <game-engine/geGame.hpp>
#include "componentsRegistered.hpp"

int main2()
{
    GameEngine::geEnvironment env;
    GameEngine::geGame game;

    try
    {
        env.configurationPath("conf");

//        RegisterData(&env);
        RegisterComponents(&env);
        game.init();

        env.addPrototype("Button", "data/button.yaml");
        env.addPrototype("Text", "data/text.yaml");
        env.addPrototype("PropertyInput", "data/propertyInput.yaml");
        env.addScene("Test", "data/testScene.yaml");

        env.firstScene("Test");
        game.configEnvironment(env);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return game.loop();
}
