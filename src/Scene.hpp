//
// Created by adria on 01/10/2018.
//

#ifndef SPACEINVADERS_SCENE_HPP
#define SPACEINVADERS_SCENE_HPP


#include <string>
#include <vector>
#include <memory>
#include "GameObject.hpp"

class Scene {
    std::string filename_;
    std::vector<std::shared_ptr<GameObject>> gameobjects_;

    void loadFile();
public:
    explicit Scene(const std::string &filename);
    void init();
    void update(float elapsedTime);
    void addGameObject(const std::shared_ptr<GameObject> &gameObject);
    std::shared_ptr<GameObject> findObjectByName(const std::string &gameObjectName) const;
    void shutDown();
};


#endif //SPACEINVADERS_SCENE_HPP
