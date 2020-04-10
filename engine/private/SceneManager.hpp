//
// Created by adria on 01/10/2018.
//

#ifndef SPACEINVADERS_SCENEMANAGER_HPP
#define SPACEINVADERS_SCENEMANAGER_HPP


#include <memory>
#include <unordered_map>
#include "Scene.hpp"

namespace GameEngine {
namespace Internal {

    class Game;

    class SceneManager {
        std::unordered_map<std::string, std::shared_ptr<Scene>> scenes_;
        std::shared_ptr<Scene> currentScene_;
        std::string currentSceneName_;
        std::string sceneNameToChange_;
        std::shared_ptr<Game> game_;
    public:
        void update(float elapsedTime);

        void registerScene(const std::string &name, const std::string &filename);

        void changeScene(const std::string &name);

        void changeSceneInSafeMode();

        void addObjectIntoCurrentScene(const std::shared_ptr<Internal::GameObject> &object);

        std::shared_ptr<GameObject> findObjectByName(const std::string &gameObjectName);

        void reloadScene();

        void clear();

        bool isSceneLoaded() const;

        std::shared_ptr<Camera> getCameraOfCurrentScene() const;

        void bindGame(const std::shared_ptr<Game> &game);
        void unbindGame();
    };
}
}
#endif //SPACEINVADERS_SCENEMANAGER_HPP
