//
// Created by adria on 25/01/2019.
//

#include <game-engine/geGame.hpp>
#include <iostream>
#include "../private/GameObject.hpp"
#include "../private/ObjectManager.hpp"
#include "../private/Game.hpp"
#include "../private/SceneManager.hpp"

namespace GameEngine {
    geGameObjectRef geGame::createObject(const std::string &name) {
        Internal::GameObject *object = new Internal::GameObject("");
        object->name(name);
        return geGameObjectRef(object);
    }

    void geGame::configEnvironment(const geEnvironment &environment) {
        environment_ = environment;
    }

    geGameObjectRef geGame::createFromPrototype(const std::string &prototype) {
        std::shared_ptr<Internal::GameObject> object = Internal::ObjectManager::GetInstance().createGameObject(prototype);
        return object;
    }

    int geGame::start() {
        try {
            Internal::Game::GetInstance().init(environment_.configurationPath());

            if(!environment_.firstScene().empty())
                Internal::SceneManager::GetInstance().changeScene(environment_.firstScene());

            Internal::Game::GetInstance().loop();

            return 0;
        }
        catch (const std::exception &e){
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }

    void geGame::shutdown() {
        Internal::Game::GetInstance().shutdown();
    }
}