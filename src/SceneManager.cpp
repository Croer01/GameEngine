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

void SceneManager::changeScene(const std::string &name) {
    auto it = scenes_.find(name);

    if(it == scenes_.end())
        throw std::exception(("scene " + name + " not found").c_str());

    sceneNameToChange_ = name;
}

void SceneManager::update(float elapsedTime) {
    if(currentScene_)
        currentScene_->update(elapsedTime);
}

void SceneManager::changeSceneInSafeMode() {
    if(!sceneNameToChange_.empty()) {
        if(currentScene_)
            currentScene_->shutDown();
        currentScene_ = scenes_[sceneNameToChange_];
        currentScene_->init();

        sceneNameToChange_.clear();
    }
}

    return it->second;
}
