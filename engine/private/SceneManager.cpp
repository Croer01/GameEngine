//
// Created by adria on 01/10/2018.
//

#include "SceneManager.hpp"
#include "ObjectManager.hpp"
#include "Game.hpp"

namespace GameEngine {
namespace Internal {
    void SceneManager::registerScene(const std::string &name, const std::string &filename) {
        auto it = scenes_.find(name);

        if (it != scenes_.end())
            throw std::runtime_error(("scene " + name + " already registered").c_str());

        std::unique_ptr<Scene> scene;
        scene.reset(new Scene(filename));
        scenes_.insert(std::make_pair(name, std::move(scene)));
    }

    void SceneManager::changeScene(const std::string &name) {
        auto it = scenes_.find(name);

        if (it == scenes_.end())
            throw std::runtime_error(("scene " + name + " not found").c_str());

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
            assert(game_);
            currentScene_->init(game_);
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
    }

    bool SceneManager::isSceneLoaded() const {
        return (bool)currentScene_;
    }

    std::shared_ptr<Camera> SceneManager::getCameraOfCurrentScene() const {
        std::shared_ptr<Camera> cam;

        if(currentScene_)
            cam = std::dynamic_pointer_cast<Camera>(currentScene_->cam());

        return cam;
    }

    void SceneManager::bindGame(Game *game)
    {
        game_ = game;
    }

    void SceneManager::unbindGame()
    {
        game_ = nullptr;
    }

    AStarPathfinding *SceneManager::pathFinder() const
    {
        return currentScene_ ? currentScene_->getPathfinding() : nullptr;
    }
}
}