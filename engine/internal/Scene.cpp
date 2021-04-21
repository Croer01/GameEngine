//
// Created by adria on 01/10/2018.
//

#include <yaml-cpp/yaml.h>
#include <memory>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <game-engine/internal/Scene.hpp>
#include <game-engine/internal/yamlConverters.hpp>
#include <game-engine/internal/ObjectManager.hpp>
#include <game-engine/Game.hpp>
#include <game-engine/internal/WritableData.hpp>

namespace GameEngine {
namespace Internal {

    Scene::Scene(const std::string &filePath) : filePath_(filePath), initialized_(false) {
    }

    void Scene::init(Game *game) {
        cam_ = std::make_unique<Camera>(game);
        gameObjects_.clear();
        loadFile(game);

        int index = 0;
        // Doing loop this way because some objects can be added by GameObjects preInit calls
        while(index < gameObjects_.size())
        {
            gameObjects_[index]->preInit();
            index++;
        }

        //Get the current size to know the new objects added by GameObjects Init calls.
        // Moreover, these new objects required to preinitialize before initialize.
        int preinitLastIndex = gameObjects_.size() - 1;
        index = 0;
        // Doing loop this way because some objects can be added by GameObjects preInit calls
        while(index < gameObjects_.size())
        {
            if(index > preinitLastIndex)
                gameObjects_[index]->preInit();

            gameObjects_[index]->Init();
            index++;
        }
        initialized_ = true;
    }

    void Scene::update(float elapsedTime) {

        // get the current lenght of the objects list to avoid issues related to adding items
        // while iterate the list
        auto length = gameObjects_.size();

        for(auto i=0; i < length; i++){
            gameObjects_[i]->Update(elapsedTime);
        }
        removeDestroyedObjects();
    }

    void Scene::loadFile(Game *game) {
        try {
            YAML::Node sceneConfig = YAML::LoadFile(filePath_);

            name_ = sceneConfig["name"].as<std::string>();
            YAML::Node prototypes = sceneConfig["prototypes"];

            for (auto i = 0; i < prototypes.size(); ++i) {
                YAML::Node prototype = prototypes[i];
                auto newGameObject = game->createFromPrototype(prototype["prototype"].as<std::string>());
                std::shared_ptr<GameObject> gameObject = std::dynamic_pointer_cast<GameObject>(newGameObject);

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
            }

        } catch (const std::exception &e) {
            throw std::runtime_error("Can't load '" + filePath_ + "'. cause: " + e.what());
        }
    }

    void Scene::shutDown() {
        for (auto &gameobject : gameObjects_) {
            gameobject->destroy();
        }
        gameObjects_.clear();
        initialized_ = false;
    }

    void Scene::addGameObject(const std::shared_ptr<GameObject> &gameObject) {
        gameObjects_.push_back(gameObject);
        if(initialized_)
        {
            gameObject->preInit();
            gameObject->Init();
        }
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

    GameEngine::geCamera *Scene::cam() const {
        return cam_.get();
    }

geDataRef Scene::saveCurrentState() const
{
    std::shared_ptr<WritableData> data = std::make_shared<WritableData>();
    std::vector<YAML::Node> prototypes;
    prototypes.reserve(gameObjects_.size());
    for (const auto &object : gameObjects_)
    {
        WritableData objectData;
        objectData.setString("name", object->name());
        objectData.setString("prototype", object->getType());
        objectData.setVec2D("position", object->position());
        objectData.setFloat("rotation", object->rotation());
        objectData.setVec2D("scale", object->scale());

        const YAML::Node &node = objectData.yamlNode();
        prototypes.push_back(node);
    }
    data->setString("name", name_);
    data->yamlNode()["prototypes"] = prototypes;
    return data;
}

void Scene::removeDestroyedObjects()
{
    // process all the objects destroyed in this frame
    gameObjects_.erase(std::remove_if(gameObjects_.begin(), gameObjects_.end(),
                                      [](const std::shared_ptr<GameObject>& go) {
                                          return go->isDestroyed();
                                      }), gameObjects_.end());
}

}
}
