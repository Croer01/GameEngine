//
// Created by adria on 27/01/2019.
//

#include <game-engine/geEnvironment.hpp>
#include "../private/ObjectManager.hpp"
#include "../private/SceneManager.hpp"

namespace GameEngine {
    geEnvironment::geEnvironment() : configurationPath_("conf"){
    }

    void GameEngine::geEnvironment::configurationPath(const std::string &config) {
        configurationPath_ = config;
    }

    std::string geEnvironment::configurationPath() const {
        return configurationPath_;
    }

    void geEnvironment::firstScene(const std::string &sceneName) {
        firstScene_ = sceneName;
    }

    std::string geEnvironment::firstScene() const {
        return firstScene_;
    }

    void geEnvironment::addPrototype(const std::string &name, const std::string &filePath) {
        Internal::ObjectManager::GetInstance().registerPrototype(name, filePath);
    }

    void geEnvironment::addScene(const std::string &name, const std::string &filePath) {
        Internal::SceneManager::GetInstance().registerScene(name, filePath);
    }
}

