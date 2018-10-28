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

        //register scene
        SceneManager::GetInstance().registerScene("Scene0","data/gameplayScene.yaml");

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