#include <iostream>
#include "src/Game.hpp"
#include "src/GameObject.hpp"
#include "PrintTextComponent.hpp"
#include "src/Factory.hpp"

int main() {
    try {
        Game::GetInstance().init();
        std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>("Player");
        gameObject->fromFile("data/test.ini");
        gameObject->addComponent(ObjectManager::GetInstance().createComponent("PrintTextComponent"));
        gameObject->Init();

        for (;;) {
            gameObject->Update(0);
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