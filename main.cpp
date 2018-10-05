#include <iostream>
#include "src/Game.hpp"
#include "src/GameObject.hpp"
#include "PrintTextComponent.hpp"
#include "src/Factory.hpp"
#include "src/SceneManager.hpp"

int main() {
    try {
        Game::GetInstance().init();
        //load object from file
        ObjectManager::GetInstance().registerPrototype("Player", "data/test.ini");
        ObjectManager::GetInstance().registerPrototype("Player2", "data/test2.ini");

        //register scene
        SceneManager::GetInstance().registerScene("Scene0","data/scene0.ini");

        std::shared_ptr<Scene> scene = SceneManager::GetInstance().getScene("Scene0");
        scene->init();

        for (;;) {
            scene->update(0);
        }

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