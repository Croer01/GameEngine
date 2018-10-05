//
// Created by adria on 01/10/2018.
//

#include <INIReader.h>
#include <sstream>
#include "Scene.hpp"
#include "ObjectManager.hpp"

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
    INIReader reader(filename_);
    if (reader.ParseError() < 0) {
        throw std::runtime_error("Can't load '" + filename_ + "'");
    }

    std::string prototypes = reader.Get("","prototypes","");

    std::stringstream ss(prototypes);
    std::string prototypeName;
    while(std::getline(ss, prototypeName, ' '))
    {
        std::shared_ptr<GameObject> gameObject = ObjectManager::GetInstance().createGameObject(prototypeName);
        gameobjects_.push_back(gameObject);
    }
}
