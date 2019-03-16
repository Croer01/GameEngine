#include <iostream>
#include <game-engine/geEnvironment.hpp>
#include <game-engine/geGame.hpp>
#include "componentsRegistered.hpp"

int main() {

    GameEngine::geEnvironment env;
    GameEngine::geGame game;

    try {
        env.configurationPath("conf");

        RegisterComponents(&env);

        game.init();

        //load object from file
        env.addPrototype("Alien1", "data/alien1.yaml");
        env.addPrototype("Alien2", "data/alien2.yaml");
        env.addPrototype("Alien3", "data/alien3.yaml");
        env.addPrototype("MotherShip", "data/mothership.yaml");
        env.addPrototype("Player", "data/player.yaml");
        env.addPrototype("PlayerBullet", "data/player bullet.yaml");
        env.addPrototype("EnemyBullet", "data/enemy bullet.yaml");
        env.addPrototype("EnemyManager", "data/enemyManager.yaml");
        env.addPrototype("ScoreText", "data/scoreText.yaml");
        env.addPrototype("StartGameText", "data/startGameText.yaml");
        env.addPrototype("StartGameScoreText", "data/startGameScoreText.yaml");
        env.addPrototype("MotherShipStartMenu", "data/motherShipStartMenu.yaml");
        env.addPrototype("MotherShipScoreText", "data/motherShipScoreText.yaml");
        env.addPrototype("EnemyExplosion", "data/enemyExplosion.yaml");
        env.addPrototype("LifesCounter", "data/playerLifesCounter.yaml");
        env.addPrototype("GameManager", "data/gameManager.yaml");
        env.addPrototype("Shelter", "data/shelter.yaml");
        env.addPrototype("MuteDescriptionText", "data/muteDescriptionText.yaml");

        //register scene
        env.addScene("StartMenu", "data/startMenuScene.yaml");
        env.addScene("Scene0", "data/gameplayScene.yaml");

        env.firstScene("StartMenu");

        game.configEnvironment(env);
    }
    catch (const std::exception &e){
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return game.loop();
}