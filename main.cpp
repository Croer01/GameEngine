#include <iostream>
#include "src/Game.hpp"
#include "src/GameObject.hpp"
#include "src/Factory.hpp"
#include "src/SceneManager.hpp"
#include "src/ObjectManager.hpp"

int main() {
    try {
        Game::GetInstance().init("conf");
        //load object from file
        ObjectManager::GetInstance().registerPrototype("Alien1", "data/alien1.yaml");
        ObjectManager::GetInstance().registerPrototype("Alien2", "data/alien2.yaml");
        ObjectManager::GetInstance().registerPrototype("Alien3", "data/alien3.yaml");
        ObjectManager::GetInstance().registerPrototype("MotherShip", "data/mothership.yaml");
        ObjectManager::GetInstance().registerPrototype("Player", "data/player.yaml");
        ObjectManager::GetInstance().registerPrototype("PlayerBullet", "data/player bullet.yaml");
        ObjectManager::GetInstance().registerPrototype("EnemyBullet", "data/enemy bullet.yaml");
        ObjectManager::GetInstance().registerPrototype("EnemyManager", "data/enemyManager.yaml");
        ObjectManager::GetInstance().registerPrototype("ScoreText", "data/scoreText.yaml");
        ObjectManager::GetInstance().registerPrototype("StartGameText", "data/startGameText.yaml");
        ObjectManager::GetInstance().registerPrototype("StartGameScoreText", "data/startGameScoreText.yaml");
        ObjectManager::GetInstance().registerPrototype("MotherShipStartMenu", "data/motherShipStartMenu.yaml");
        ObjectManager::GetInstance().registerPrototype("MotherShipScoreText", "data/motherShipScoreText.yaml");
        ObjectManager::GetInstance().registerPrototype("EnemyExplosion", "data/enemyExplosion.yaml");
        ObjectManager::GetInstance().registerPrototype("LifesCounter", "data/playerLifesCounter.yaml");
        ObjectManager::GetInstance().registerPrototype("GameManager", "data/gameManager.yaml");

        //register scene
        SceneManager::GetInstance().registerScene("StartMenu","data/startMenuScene.yaml");
        SceneManager::GetInstance().registerScene("Scene0","data/gameplayScene.yaml");

        SceneManager::GetInstance().changeScene("StartMenu");
        Game::GetInstance().loop();

        return 0;
    }
    catch (const std::exception &e){
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (...){
        std::cerr << "unexcepted error" << std::endl;
        return 1;
    }
}