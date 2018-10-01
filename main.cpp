#include <iostream>
#include "src/Game.hpp"
#include "src/GameObject.hpp"
#include "PrintTextComponent.hpp"
#include "src/Factory.hpp"

int main() {
    try {
        Game::GetInstance().init();
        //load object from file
        ObjectManager::GetInstance().registerPrototype("Player", "data/test.ini");
        ObjectManager::GetInstance().registerPrototype("Player2", "data/test2.ini");

        //instantiate new object
        std::shared_ptr<GameObject> gameObject = ObjectManager::GetInstance().createGameObject("Player");
        std::shared_ptr<GameObject> gameObject2 = ObjectManager::GetInstance().createGameObject("Player2");
        gameObject->Init();
        gameObject2->Init();

        for (;;) {
            gameObject->Update(0);
            gameObject2->Update(0);
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