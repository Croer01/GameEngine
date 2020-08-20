//
// Created by adria on 01/10/2018.
//

#include <yaml-cpp/yaml.h>
#include <memory>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "Scene.hpp"
#include "yamlConverters.hpp"
#include "ObjectManager.hpp"
#include "Game.hpp"

namespace GameEngine {
namespace Internal {

    Scene::Scene(const std::string &filename) : filename_(filename) {
    }

    void Scene::init(Game *game) {
        cam_.reset(new Camera());
        gameObjects_.clear();
        loadFile(game);

        if(pathfindingConfig_)
        {
            pathfinding_ = std::make_unique<AStarPathfinding>(pathfindingConfig_->columns, pathfindingConfig_->rows);
        }

        auto size = gameObjects_.size();

        for (auto i = 0; i < size; i++) {
            gameObjects_[i]->preInit();
        }

        for (auto i = 0; i < size; i++) {
            gameObjects_[i]->Init();
        }
    }

    void Scene::update(float elapsedTime) {

        // get the current length of the objects list to avoid issues related to adding items
        // while iterate the list
        auto length = gameObjects_.size();

        for(auto i=0; i < length; i++){
            gameObjects_[i]->Update(elapsedTime);
        }

        // process all the objects destroyed in this frame
        gameObjects_.erase(std::remove_if(gameObjects_.begin(), gameObjects_.end(),
                                          [](const std::shared_ptr<GameObject>& go) {
                return go->isDestroyed();
            }), gameObjects_.end());
    }

    void Scene::loadFile(Game *game) {
        try {
            YAML::Node sceneConfig = YAML::LoadFile(filename_);

            YAML::Node prototypes = sceneConfig["prototypes"];

            for (auto i = 0; i < prototypes.size(); ++i) {
                YAML::Node prototype = prototypes[i];
                std::shared_ptr<GameObject> gameObject = game->objectManager()->createGameObject(
                        prototype["prototype"].as<std::string>(), game);

                if (prototype["name"])
                    gameObject->name(prototype["name"].as<std::string>());

                //object properties
                if (prototype["position"])
                    gameObject->position(prototype["position"].as<Vec2D>());
                if (prototype["rotation"])
                {
                    float degrees = prototype["rotation"].as<float>();
                    gameObject->rotation(glm::radians(degrees));
                }
                if (prototype["scale"])
                    gameObject->scale(prototype["scale"].as<Vec2D>());

                std::cout << "object created " << prototype["prototype"].as<std::string>() << std::endl;
                gameObjects_.push_back(gameObject);
            }

            YAML::Node pathfinding = sceneConfig["pathfinding"];

            if(pathfinding)
            {
                pathfindingConfig_ = std::make_unique<PathfindingConfig>();
                pathfindingConfig_->columns = pathfinding["columns"].as<int>();
                pathfindingConfig_->rows = pathfinding["rows"].as<int>();
            }

        } catch (const std::exception &e) {
            throw std::runtime_error("Can't load '" + filename_ + "'. cause: " + e.what());
        }
    }

    void Scene::shutDown() {
        for (auto &gameobject : gameObjects_) {
            gameobject->destroy();
        }
        gameObjects_.clear();
    }

    void Scene::addGameObject(const std::shared_ptr<GameObject> &gameObject) {
        gameObjects_.push_back(gameObject);
        gameObject->preInit();
        gameObject->Init();
    }


    std::shared_ptr<GameObject> Scene::findObjectByName(const std::string &gameObjectName) const {

        std::shared_ptr<GameObject> found;
        int i = 0;

        while (!found && i < gameObjects_.size()) {
            if (gameObjects_[i]->name() == gameObjectName)
                found = gameObjects_[i];
            i++;
        }

        return found;
    }

    std::shared_ptr<GameEngine::geCamera> Scene::cam() const {
        return cam_;
    }

    AStarPathfinding *Scene::getPathfinding() const
    {
        return pathfinding_.get();
    }
}
}