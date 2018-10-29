//
// Created by adria on 01/10/2018.
//

#include <yaml-cpp/yaml.h>
#include <sstream>
#include <iostream>
#include "Scene.hpp"
#include "ObjectManager.hpp"

glm::vec3 readVector3(const YAML::Node &node, float defaultValue){
    if(!node.IsSequence() || node.size() != 3) {
        return glm::vec3(defaultValue, defaultValue, defaultValue);
    }

    return glm::vec3(node[0].as<double>(), node[1].as<double>(), node[2].as<double>());
}

Scene::Scene(const std::string &filename) : filename_(filename){
}

void Scene::init() {
    gameobjects_.clear();
    loadFile();
    for (auto &gameobject : gameobjects_) {
        gameobject->Init();
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
                    prototype["name"].as<std::string>());

            //object properties
            gameObject->setPosition(readVector3(prototype["position"], 0));
            gameObject->setRotation(readVector3(prototype["rotation"], 0));
            gameObject->setScale(readVector3(prototype["scale"], 1));

            std::cout << "object created " << prototype["name"].as<std::string>() << std::endl;
            gameobjects_.push_back(gameObject);
        }
    } catch (const std::exception &e){
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
