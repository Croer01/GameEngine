//
// Created by adria on 01/10/2018.
//

#include "SceneManager.hpp"

void SceneManager::registerScene(const std::string &name, const std::string &filename) {
    auto it = scenes_.find(name);

    if(it != scenes_.end())
        throw std::exception(("scene " + name + " already registered").c_str());

    std::unique_ptr<Scene> scene = std::make_unique<Scene>(filename);
    scenes_.insert(std::make_pair(name, std::move(scene)));
}

std::shared_ptr<Scene> SceneManager::getScene(const std::string &name) {
    auto it = scenes_.find(name);

    if(it == scenes_.end())
        throw std::exception(("scene " + name + " not found").c_str());


    return it->second;
}
