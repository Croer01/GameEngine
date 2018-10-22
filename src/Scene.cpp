//
// Created by adria on 01/10/2018.
//

#include <INIReader.h>
#include <sstream>
#include "Scene.hpp"
#include "ObjectManager.hpp"

glm::vec3 readVector3(const INIReader &iniFile,const std::string &prototype, const std::string &vectorName, double defaultValue){
    float x = static_cast<float>(iniFile.GetReal(prototype, vectorName + "_x", defaultValue));
    float y = static_cast<float>(iniFile.GetReal(prototype, vectorName + "_y", defaultValue));
    float z = static_cast<float>(iniFile.GetReal(prototype, vectorName + "_z", defaultValue));

    return glm::vec3(x,y,z);
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

        //object properties
        gameObject->setPosition(readVector3(reader,prototypeName,"position",0));
        gameObject->setRotation(readVector3(reader,prototypeName,"rotation",0));
        gameObject->setScale(readVector3(reader,prototypeName,"scale",1));

        gameobjects_.push_back(gameObject);
    }
}
