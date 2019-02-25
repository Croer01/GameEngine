//
// Created by adria on 01/10/2018.
//

#include "SceneManager.hpp"
#include "ObjectManager.hpp"
namespace GameEngine {
namespace Internal {
    void SceneManager::registerScene(const std::string &name, const std::string &filename) {
        auto it = scenes_.find(name);

        if (it != scenes_.end())
            throw std::exception(("scene " + name + " already registered").c_str());

        std::unique_ptr<Scene> scene = std::make_unique<Scene>(filename);
        scenes_.insert(std::make_pair(name, std::move(scene)));
    }

    void SceneManager::changeScene(const std::string &name) {
        auto it = scenes_.find(name);

        if (it == scenes_.end())
            throw std::exception(("scene " + name + " not found").c_str());

        sceneNameToChange_ = name;
    }

    void SceneManager::update(float elapsedTime) {
        if (currentScene_)
            currentScene_->update(elapsedTime);
    }

    void SceneManager::changeSceneInSafeMode() {
        if (!sceneNameToChange_.empty()) {
            if (currentScene_)
                currentScene_->shutDown();
            currentScene_ = scenes_[sceneNameToChange_];
            currentScene_->init();
            currentSceneName_ = sceneNameToChange_;
            sceneNameToChange_.clear();
        }
    }

    void SceneManager::addObjectIntoCurrentScene(const std::shared_ptr<Internal::GameObject> &object) {
        currentScene_->addGameObject(object);
    }

    std::shared_ptr<GameObject> SceneManager::findObjectByName(const std::string &gameObjectName) {
        return currentScene_->findObjectByName(gameObjectName);
    }

    void SceneManager::reloadScene() {
        changeScene(currentSceneName_);
    }

    void SceneManager::clear() {
        if (currentScene_)
            currentScene_->shutDown();
        currentScene_.reset();
        sceneNameToChange_.clear();
        scenes_.clear();
    }

    bool SceneManager::isSceneLoaded() const {
        return (bool)currentScene_;
    }
}
}