//
// Created by adria on 01/10/2018.
//

#include <yaml-cpp/yaml.h>
#include <sstream>
#include <iostream>
#include "Scene.hpp"
#include "yamlConverters.hpp"
#include "ObjectManager.hpp"

namespace GameEngine {
namespace Internal {

    Scene::Scene(const std::string &filename) : filename_(filename) {
    }

    void Scene::init() {
        cam_.reset(new Camera());
        gameobjects_.clear();
        loadFile();

        auto size = gameobjects_.size();
        for (auto i = 0; i < size; i++) {
            gameobjects_[i]->Init();
        }
    }

    void Scene::update(float elapsedTime) {
        for (auto &gameobject : gameobjects_) {
            gameobject->Update(elapsedTime);
        }
    }

    void Scene::loadFile() {
        try {
            YAML::Node sceneConfig = YAML::LoadFile(filename_);

            YAML::Node prototypes = sceneConfig["prototypes"];

            for (auto i = 0; i < prototypes.size(); ++i) {
                YAML::Node prototype = prototypes[i];
                std::shared_ptr<GameObject> gameObject = ObjectManager::GetInstance().createGameObject(
                        prototype["prototype"].as<std::string>());

                if (prototype["name"])
                    gameObject->name(prototype["name"].as<std::string>());

                //object properties
                if (prototype["position"])
                    gameObject->position(prototype["position"].as<Vec2D>());
                if (prototype["rotation"])
                    gameObject->rotation(prototype["rotation"].as<Vec2D>());
                if (prototype["scale"])
                    gameObject->scale(prototype["scale"].as<Vec2D>());

                std::cout << "object created " << prototype["prototype"].as<std::string>() << std::endl;
                gameobjects_.push_back(gameObject);
            }
        } catch (const std::exception &e) {
            throw std::runtime_error("Can't load '" + filename_ + "'. cause: " + e.what());
        }
    }

    void Scene::shutDown() {
        gameobjects_.clear();
    }

    void Scene::addGameObject(const std::shared_ptr<GameObject> &gameObject) {
        gameobjects_.push_back(gameObject);
        gameObject->Init();
    }


    std::shared_ptr<GameObject> Scene::findObjectByName(const std::string &gameObjectName) const {

        std::shared_ptr<GameObject> found;
        int i = 0;

        while (!found && i < gameobjects_.size()) {
            if (gameobjects_[i]->name() == gameObjectName)
                found = gameobjects_[i];
            i++;
        }

        return found;
    }

    std::shared_ptr<GameEngine::geCamera> Scene::cam() const {
        return cam_;
    }
}
}