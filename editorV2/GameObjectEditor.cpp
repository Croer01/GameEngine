#include <iostream>
#include <string>
#include <game-engine/geEnvironment.hpp>
#include <game-engine/Game.hpp>

using namespace GameEngine;

int main(int argc, char **argv)
{
    const geEnvironmentRef &env = geEnvironment::createInstance();
    // Register all the components
    //env->registerComponent("StartMenuComponent", CreateComponentBuilder<StartMenuComponent>());
    //env->registerComponent("CellComponent", CreateComponentBuilder<CellComponent>());
    //env->registerComponent("GridComponent", CreateComponentBuilder<GridComponent>());


    // Register all the prototypes
    //env->addPrototype("StartMenuUI", "StartMenuUI.prototype");
    //env->addPrototype("Cell", "Cell.prototype");
    //env->addPrototype("Grid", "Grid.prototype");
    env->addPrototype("MainUI", "mainUI.prototype");

    //Register all the scenes
    env->addScene("Main", "main.scene");
    //env->addScene("Game", "Game.scene");

    env->firstScene("Main");

    const GameRef &game = Game::createInstance(env);
    game->screen()->resizable(true);

    game->init();
    while (game->isRunning())
    {
        game->update();
        game->render();
    }

    return 0;
}
