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

    geGame::geGame() {
        //Force initialize the instance
        Internal::Game::GetInstance().context(this);
        initialized_ = false;
    }

    geGame::~geGame() {
        Internal::Game::GetInstance().context(nullptr);
    }

    geGameObjectRef geGame::createObject(const std::string &name) {
        const std::shared_ptr<Internal::GameObject> &object = std::make_shared<Internal::GameObject>("");
        object->name(name);
        if(Internal::SceneManager::GetInstance().isSceneLoaded())
            Internal::SceneManager::GetInstance().addObjectIntoCurrentScene(object);
        return object;
    }

    void geGame::configEnvironment(const geEnvironment &environment) {
        environment_ = environment;
    }

    geGameObjectRef geGame::createFromPrototype(const std::string &prototype) {
        const std::shared_ptr<Internal::GameObject> &object = Internal::ObjectManager::GetInstance().createGameObject(prototype);
        if(Internal::SceneManager::GetInstance().isSceneLoaded())
            Internal::SceneManager::GetInstance().addObjectIntoCurrentScene(object);
        return object;
    }

    void geGame::init(){
        if(!initialized_) {
            Internal::Game::GetInstance().init(environment_.configurationPath());
            initialized_ = true;
        }
    }

    int geGame::loop() {
        try {
            init();

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

    geGameObjectRef geGame::findObjectByNameInCurrentScene(const std::string &gameObjectName) {
        const std::shared_ptr<Internal::GameObject> &object = Internal::SceneManager::GetInstance().findObjectByName(gameObjectName);
        return std::dynamic_pointer_cast<geGameObject>(object);
    }

    void geGame::changeScene(const std::string &name) {
        Internal::SceneManager::GetInstance().changeScene(name);
    }

    geScreen &geGame::screen() const {
        return Internal::Game::GetInstance().screen();
    }
}